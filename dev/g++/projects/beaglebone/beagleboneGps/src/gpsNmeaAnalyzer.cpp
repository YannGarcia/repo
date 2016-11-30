/* Beaglebone GPS NMEA protocol analyzer implementation
 * Copyright (c) 2013 ygarcia, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used for std::strerror, std::memcpy
#include <iterator> // std::back_inserter
#include <algorithm> // std::copy

#include "gpsNmeaAnalyzer.h"

#include "beagleboneGpsEx.h"

// TODO Use factory to create instance of GPS protocol analyzer
namespace beagleboneGps {

#define IDX_UTC_DATETIME   0
#define IDX_LATITUDE  1
#define IDX_LONGITUDE 2
#define IDX_SPEED     3
#define IDX_ALTITUDE  4
#define IDX_HEADING   5
#define IDX_YAWRATE   6

  std::string gpsNmeaAnalyzer::CmdPattern( 
					  "\\$GP([[:alpha:]]{3}),"
					  "(.*)"
					  "\r\n$"
					   );
  std::string gpsNmeaAnalyzer::GgaPattern(
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," //  UTC time of the fix: hhmmssdd
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Latitude coordinates: xxmm.dddd
					  "([NS]){0,1}," // North or South
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Longitude coordinates: yyymm.dddd
					  "([WE]){0,1}," // West or Est
					  "([[:digit:]]*)," // Fix valid indicator (0=no fix, 1=GPS fix, 2=Dif. GPS fix)
					  "([[:digit:]]*)," // Number of satellites in use: ss
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Horizontal dilution of precision: d.d
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Antenna altitude above mean-sea-level: h.h
					  "([M]){0,1}," // Units of antenna altitude, meters
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Difference between the WGS-84 reference ellipsoid surface and the mean-sea-level altitude: g.g
					  "([M]){0,1}," // Units of geoidal separation, meters
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Age of Differential GPS data (seconds)
					  "([[:xdigit:]]*)\\*([[:xdigit:]]{2})" // Checksum
					  );
  std::string gpsNmeaAnalyzer::RmcPattern(
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Fix taken UTC time: hhmmss.dd
					  "([AV]){0,1}," // Status: Data Active or V (void)
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Latitude coordinate: xxmm.dddd
					  "([NS]){0,1}," // North or South
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Longitude coordinate: yyymm.dddd
					  "([WE]){0,1}," // West or Est
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Speed over the ground in knots: s.s
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Heading: h.h
					  "([[:digit:]]+){0,1}," // UTC Date of the fix: ddmmyy
					  "([[:digit:]]+\\.[[:digit:]]+){0,1}," // Magnetic Variation: d.d
 					  "([WE]){0,1}," // Direction of magnetic variation: West or Est
					  "([[:xdigit:]]*)\\*([[:xdigit:]]{2})" // Checksum
					  );
  
  gpsNmeaAnalyzer::gpsNmeaAnalyzer() : _buffer() { 
    //    std::clog << ">>> gpsNmeaAnalyzer::gpsNmeaAnalyzer" << std::endl;

    _buffer.reserve(2048); // To optimize std::append operation

    // Compile regular expressions
    if (regcomp(&_regexCommand, gpsNmeaAnalyzer::CmdPattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
      std::ostringstream os;
      os << "gpsNmeaAnalyzer::gpsNmeaAnalyzer: Could not compile Command regex: " << std::strerror(errno) << std::endl;
      throw new beagleboneGpsEx(os.str());
    }
    if (regcomp(&_regexGga, gpsNmeaAnalyzer::GgaPattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
      std::ostringstream os;
      os << "gpsNmeaAnalyzer::gpsNmeaAnalyzer: Could not compile GGA regex: " << std::strerror(errno) << std::endl;
      throw new beagleboneGpsEx(os.str());
    }
    if (regcomp(&_regexRmc, gpsNmeaAnalyzer::RmcPattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
      std::ostringstream os;
      os << "gpsNmeaAnalyzer::gpsNmeaAnalyzer: Could not compile RMC regex: " << std::strerror(errno) << std::endl;
      throw new beagleboneGpsEx(os.str());
    }

    // Setup default values
    _values.push_back(std::string("")); // GPS UTC of position
    _values.push_back(std::string("")); // Latitude
    _values.push_back(std::string("")); // Longitude
    _values.push_back(std::string("")); // Speed
    _values.push_back(std::string("")); // Elevation
    _values.push_back(std::string("")); // Heading
    _values.push_back(std::string("")); // Yaw rate
  } // End of Constructor
  
  gpsNmeaAnalyzer::~gpsNmeaAnalyzer() {
    std::clog << ">>> gpsNmeaAnalyzer::~gpsNmeaAnalyzer" << std::endl;
  } // End of Constructor

  int gpsNmeaAnalyzer::process(const std::vector<unsigned char> & p_data) {
    //    std::clog << ">>> gpsNmeaAnalyzer::process" << std::endl;

    // Sanity check
    if (p_data.size() == 0) {
      // Nothing to do
      return 0;
    }

    // Append to the buffer
    _buffer.append(std::string(p_data.begin(), p_data.end()));

    // Process current buffer
    return processBuffer();
  } // End of method process

  int gpsNmeaAnalyzer::process(const std::string & p_data) {
    //    std::clog << ">>> gpsNmeaAnalyzer::process (string)" << std::endl;

    // Sanity check
    if (p_data.empty()) {
      // Nothing to do
      return 0;
    }

    // Append to the buffer
    _buffer.append(p_data);

    // Process current buffer
    return processBuffer();
  } // End of method process

  void gpsNmeaAnalyzer::getResult(std::vector<unsigned char> & p_buffer) {
    //  std::clog << ">>> gpsNmeaAnalyzer::getResult" << std::endl;

    p_buffer.clear();
    p_buffer.push_back(_values.size()); // # of items
    for (unsigned int i = 0; i < _values.size(); i++) {
      std::copy(_values[i].begin(), _values[i].end(), std::back_inserter(p_buffer));
      p_buffer.push_back(0x00);      
    } // End of 'for' statement 
  } // End of method getResult

  int gpsNmeaAnalyzer::processBuffer() {
    //    std::clog << ">>> gpsNmeaAnalyzer::processBuffer" << std::endl;

    //    std::clog << "gpsNmeaAnalyzer::process: current buffer: '" << _buffer << "'" << std::endl;
    size_t startCommand = _buffer.find("$");
    size_t endCommand = _buffer.find("\r\n");
    // Validate the content of the buffer
    if (startCommand == std::string::npos) {
      //      std::clog << "Clear buffer" << std::endl;
      _buffer.clear(); // No valid command into the buffer
      return 0;
    } else if (endCommand == std::string::npos) {
      if (startCommand != 0) {
	_buffer = _buffer.substr(startCommand);
	//	std::clog << "New buffer(0): '" << _buffer << "'" << std::endl;
      } else {
	size_t reverseStartCommand = _buffer.rfind("$");
	if (reverseStartCommand != startCommand) {
	  _buffer = _buffer.substr(reverseStartCommand);
	  //	  std::clog << "New buffer(1): '" << _buffer << "'" << std::endl;
	} else {
	  //	  std::clog << "Nothing to do" << std::endl;
	}
      }
      return 0;
    } else if (endCommand < startCommand) { // Remove invalid part
      _buffer = _buffer.substr(endCommand + 2);
      //      std::clog << "New buffer(2): '" << _buffer << "'" << std::endl;      
      startCommand = _buffer.find("$");
      endCommand = _buffer.find("\r\n");
    }
    if (endCommand != std::string::npos) {
      // Extract the line from the buffer
      std::string line = _buffer.substr(startCommand, endCommand + 2);
      //      std::clog << "New command: '" << line << "'" << std::endl;
      _buffer = _buffer.substr(endCommand + 2);
      //      std::clog << "New buffer: '" << _buffer << "'" << std::endl;
      // Extract the command
      _matches = (regmatch_t *)new unsigned char[sizeof(regmatch_t) * (1 + _regexCommand.re_nsub)];
      if (regexec(&_regexCommand, line.c_str(), 1 + _regexCommand.re_nsub, _matches, 0) != 0) {
	delete [] _matches;
	std::cerr << "gpsNmeaAnalyzer::process: regexec Command failed: " << std::strerror(errno) << std::endl;

	return -1;
      }
      if (_matches->rm_eo != -1) { // Matching
	std::string cmd;
	char *pstr = (char *)line.c_str();
	size_t matchLen = (_matches + 1)->rm_eo - (_matches + 1)->rm_so;
	cmd.assign(pstr + (_matches + 1)->rm_so, matchLen);
	//	std::clog << "gpsNmeaAnalyzer::process: cmd = '" << cmd << "'" << std::endl;
	std::string payload = line.substr((_matches + 2)->rm_so);
	//	std::clog << "payload ='" << payload << "'" << std::endl;
	delete [] _matches;

	// Process the command
	if (cmd.compare("GGA") == 0) {
	  return processGga(payload);
	} else if (cmd.compare("RMC") == 0) {
	  return processRmc(payload);
	} else { // Unsupported command
	  //	  std::cerr << "Unsupported NMEA command='" << cmd << "'" << std::endl;
	  return -1;
	}
      } else {
	delete [] _matches;
      } 
    } else if (_buffer.length() >= 2048) {
      _buffer.clear();
    }

    return -1;
  } // End of method process

  // TODO Enhance with a unique processGpsCommand(const std::string & p_command, const std::string & p_payload, &_regexXXX)
  int gpsNmeaAnalyzer::processGga(const std::string & p_payload) {
    // Extract fields
    _matches = (regmatch_t *)new unsigned char[sizeof(regmatch_t) * (1 + _regexGga.re_nsub)];
    if (regexec(&_regexGga, p_payload.c_str(), 1 + _regexGga.re_nsub, _matches, 0) != 0) {
      delete [] _matches;
      std::cerr << "gpsNmeaAnalyzer::processGga: regexec GGA failed: " << std::strerror(errno) << std::endl;
      
      return -1;
    }

    // Match
    //    std::cout << "gpsNmeaAnalyzer::processGga: # of matching items: " << _matches->rm_eo << std::endl;
    // 1. Extract data
    if (_matches->rm_eo != -1) { // Matching
      std::string str, ind;
      char *pstr = (char *)p_payload.c_str();
      /*      // UTC time
      size_t matchLen = (_matches + 1)->rm_eo - (_matches + 1)->rm_so;
      str.assign(pstr + (_matches + 1)->rm_so, matchLen);
      std::cout << "gpsNmeaAnalyzer::processGga: utcTime = '" << str << "'" << std::endl;
      // Update UTC time
      //_values[IDX_UTC_DATETIME] = std::stof(); // UTC of position

      // Latitude coordinate.
      matchLen = (_matches + 2)->rm_eo - (_matches + 2)->rm_so;
      str.assign(pstr + (_matches + 2)->rm_so, matchLen);
      matchLen = (_matches + 3)->rm_eo - (_matches + 3)->rm_so;
      ind.assign(pstr + (_matches + 3)->rm_so, matchLen);
      std::cout << "gpsNmeaAnalyzer::processGga: Latitude coordinate = '" << str << " " << ind << "'" << std::endl;
      _values[IDX_LATITUDE] = ind + str;

      // Longitude coordinate.
      matchLen = (_matches + 4)->rm_eo - (_matches + 4)->rm_so;
      str.assign(pstr + (_matches + 4)->rm_so, matchLen);
      matchLen = (_matches + 5)->rm_eo - (_matches + 5)->rm_so;
      ind.assign(pstr + (_matches + 5)->rm_so, matchLen);
      std::cout << "gpsNmeaAnalyzer::processGga: Longitude coordinate = '" << str << " " << ind << "'" << std::endl;
      _values[IDX_LONGITUDE] = ind + str;
      */
      // Altitude.
      size_t matchLen = (_matches + 9)->rm_eo - (_matches + 9)->rm_so;
      str.assign(pstr + (_matches + 9)->rm_so, matchLen);
      //      std::cout << "gpsNmeaAnalyzer::processGga: Altitude = '" << str << "'" << std::endl;
      _values[IDX_ALTITUDE] = str;
      
      delete [] _matches;
    } // no matches

    return 0;
  } // End of method processGga

  int gpsNmeaAnalyzer::processRmc(const std::string & p_payload) {
    // Extract fields
    _matches = (regmatch_t *)new unsigned char[sizeof(regmatch_t) * (1 + _regexRmc.re_nsub)];
    if (regexec(&_regexRmc, p_payload.c_str(), 1 + _regexRmc.re_nsub, _matches, 0) != 0) {
      delete [] _matches;
      std::cerr << "gpsNmeaAnalyzer::processRmc: regexec RMC failed: " << std::strerror(errno) << std::endl;
      
      return -1;
    }

    // Match
    //    std::cout << "gpsNmeaAnalyzer::processRmc: # of matching items: " << _matches->rm_eo << std::endl;
    // 1. Extract data
    if (_matches->rm_eo != -1) { // Matching
      std::string str, ind;
      char *pstr = (char *)p_payload.c_str();
      // Status indicator
      size_t matchLen = (_matches + 2)->rm_eo - (_matches + 2)->rm_so;
      ind.assign(pstr + (_matches + 2)->rm_so, matchLen);
      //      std::cout << "gpsNmeaAnalyzer::processRmc: Valid data = '" << ind << "'" << std::endl;
      if (ind.compare("A") == 0) { // Valida data,
	std::ostringstream os;

	// Update UTC time
	matchLen = (_matches + 1)->rm_eo - (_matches + 1)->rm_so;
	str.assign(pstr + (_matches + 1)->rm_so, matchLen);
	//	std::cout << "gpsNmeaAnalyzer::processRmc: utcTime = '" << str << "'" << std::endl;
	_values[IDX_UTC_DATETIME].assign(str); // UTC of position

 	// Latitude coordinate.
	matchLen = (_matches + 3)->rm_eo - (_matches + 3)->rm_so;
	str.assign(pstr + (_matches + 3)->rm_so, matchLen);
	matchLen = (_matches + 4)->rm_eo - (_matches + 4)->rm_so;
	ind.assign(pstr + (_matches + 4)->rm_so, matchLen);
	os << str << " " << ind;
	_values[IDX_LATITUDE].assign(os.str()); // Latitude
	//	std::cout << "gpsNmeaAnalyzer::processRmc: Latitude coordinate = '" << _values[IDX_LATITUDE] << "'" << std::endl;

 	// Longitude coordinate.
	matchLen = (_matches + 5)->rm_eo - (_matches + 5)->rm_so;
	str.assign(pstr + (_matches + 5)->rm_so, matchLen);
	matchLen = (_matches + 6)->rm_eo - (_matches + 6)->rm_so;
	ind.assign(pstr + (_matches + 6)->rm_so, matchLen);
	os.str("");
	os << str << " " << ind;
	_values[IDX_LONGITUDE].assign(os.str()); // Longitude
	//	std::cout << "gpsNmeaAnalyzer::processRmc: Longitude coordinate = '" << _values[IDX_LONGITUDE] << "'" << std::endl;

 	// Speed.
	matchLen = (_matches + 7)->rm_eo - (_matches + 7)->rm_so;
	str.assign(pstr + (_matches + 7)->rm_so, matchLen);
	//	std::cout << "gpsNmeaAnalyzer::processRmc: Speed = '" << str << "'" << std::endl;
	_values[IDX_SPEED].assign(str); // Speed

 	// Heading.
	matchLen = (_matches + 8)->rm_eo - (_matches + 8)->rm_so;
	str.assign(pstr + (_matches + 8)->rm_so, matchLen);
	//	std::cout << "gpsNmeaAnalyzer::processRmc: Heading = '" << str << "'" << std::endl;
	_values[IDX_HEADING].assign(str); // Heading.

	// Date
	matchLen = (_matches + 9)->rm_eo - (_matches + 9)->rm_so;
	str.assign(pstr + (_matches + 9)->rm_so, matchLen);
	os.str("");
	os << str << " " << _values[IDX_UTC_DATETIME];
	_values[IDX_UTC_DATETIME].assign(os.str());
	//	std::cout << "gpsNmeaAnalyzer::processRmc: UTC date/time = '" << _values[IDX_UTC_DATETIME] << "'" << std::endl;
      } // else, discard the frame
      
      delete [] _matches;
    } // no matches

    return 0;
  } // End of method processRmc

} // End of namespace beagleboneGps
