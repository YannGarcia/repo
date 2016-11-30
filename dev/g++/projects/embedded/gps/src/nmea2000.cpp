/**
 * @File    nmea2000.h
 * @brief   Implementation file for the lightweight nmea2000 library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <sstream>
#include <cstring> // Used for std::strerror

#include "nmea2000.h"

namespace gps {

  namespace parsers {

    std::string nmea2000::cmd_pattern( 
				      "\\$GP([[:alpha:]]{3}),"
				      "(.*)"
				      "\r\n$"
				       );
    std::string nmea2000::gga_pattern(
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
    std::string nmea2000::rmc_pattern(
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
  
    nmea2000::nmea2000() : 
      gps_parser(), 
      _buffer(),
      _result_code(0),
      _error_msg(),
      _regex_command(), 
      _matches(NULL), 
      _regex_gga(), 
      _regex_rmc() { 

      // Compile regular expressions
      if (regcomp(&_regex_command, nmea2000::cmd_pattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
	std::ostringstream os;
	os << "nmea2000::nmea2000: Could not compile command regex: " << std::strerror(errno) << std::endl;
	throw new std::runtime_error(os.str());
      }
      if (regcomp(&_regex_gga, nmea2000::gga_pattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
	std::ostringstream os;
	os << "nmea2000::nmea2000: Could not compile GGA regex: " << std::strerror(errno) << std::endl;
	throw new std::runtime_error(os.str());
      }
      if (regcomp(&_regex_rmc, nmea2000::rmc_pattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
	std::ostringstream os;
	os << "nmea2000::nmea2000: Could not compile RMC regex: " << std::strerror(errno) << std::endl;
	throw new std::runtime_error(os.str());
      }
    } // End of Constructor
    
    const int32_t nmea2000::process(const std::vector<uint8_t> & p_gps_frame) {
      // Sanity check
      if (p_gps_frame.size() == 0) {
	// Nothing to do
	return 0;
      }
      
      // Append to the buffer
      _buffer.append(std::string(p_gps_frame.begin(), p_gps_frame.end()));
      
      // Process current buffer
      return process_buffer();
    }
    
    const int32_t nmea2000::process(const std::string & p_gps_frame) {
      // Sanity check
      if (p_gps_frame.empty()) {
	// Nothing to do
	return 0;
      }
      
      // Reset current values
      for (std::map<uint8_t, std::string>::iterator it = _values.begin(); it != _values.end(); ++it) {
	it->second.assign("");
      } // End of 'for' statement
      
      // Append to the buffer
      _buffer.append(p_gps_frame);

      // Process current buffer
      return process_buffer();
    }
    
    const int32_t nmea2000::process_buffer() {
      //    std::clog << ">>> nmea2000::process_buffer" << std::endl;

      //      std::clog << "nmea2000::process: current buffer: '" << _buffer << "'" << std::endl;
      size_t start_command = _buffer.find("$");
      size_t end_command = _buffer.find("\r\n");
      // Validate the content of the buffer
      if (start_command == std::string::npos) {
	//      std::clog << "Clear buffer" << std::endl;
	_buffer.clear(); // No valid command into the buffer
	return 0;
      } else if (end_command == std::string::npos) {
	if (start_command != 0) {
	  _buffer = _buffer.substr(start_command);
	  //	std::clog << "New buffer(0): '" << _buffer << "'" << std::endl;
	} else {
	  size_t reverse_start_command = _buffer.rfind("$");
	  if (reverse_start_command != start_command) {
	    _buffer = _buffer.substr(reverse_start_command);
	    //	  std::clog << "New buffer(1): '" << _buffer << "'" << std::endl;
	  } else {
	    //	  std::clog << "Nothing to do" << std::endl;
	  }
	}
	return 0;
      } else if (end_command < start_command) { // Remove invalid part
	_buffer = _buffer.substr(end_command + 2);
	//      std::clog << "New buffer(2): '" << _buffer << "'" << std::endl;      
	start_command = _buffer.find("$");
	end_command = _buffer.find("\r\n");
      }
      if (end_command != std::string::npos) {
	// Extract the line from the buffer
	std::string line = _buffer.substr(start_command, end_command + 2);
	//      std::clog << "New command: '" << line << "'" << std::endl;
	_buffer = _buffer.substr(end_command + 2);
	//      std::clog << "New buffer: '" << _buffer << "'" << std::endl;
	// Extract the command
	_matches = (regmatch_t *)new uint8_t[sizeof(regmatch_t) * (1 + _regex_command.re_nsub)];
	if ((_result_code = regexec(&_regex_command, line.c_str(), 1 + _regex_command.re_nsub, _matches, 0)) != 0) {
	  delete [] _matches;
	  regerror(_result_code, &_regex_command, _error_msg, sizeof(_error_msg));
	  std::cerr << "nmea2000::process: regexec command failed: " << _error_msg << std::endl;

	  return -1;
	}
	if (_matches->rm_eo != -1) { // Matching
	  std::string cmd;
	  char *pstr = (char *)line.c_str();
	  size_t match_len = (_matches + 1)->rm_eo - (_matches + 1)->rm_so;
	  cmd.assign(pstr + (_matches + 1)->rm_so, match_len);
	  //	std::clog << "nmea2000::process: cmd = '" << cmd << "'" << std::endl;
	  std::string payload = line.substr((_matches + 2)->rm_so);
	  //	std::clog << "payload ='" << payload << "'" << std::endl;
	  delete [] _matches;

	  // Process the command
	  if (cmd.compare("GGA") == 0) {
	    return process_gga(payload);
	  } else if (cmd.compare("RMC") == 0) {
	    return process_rmc(payload);
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

    const int32_t nmea2000::process_gga(const std::string & p_gps_frame) {
      // Extract fields
      _matches = (regmatch_t *)new uint8_t[sizeof(regmatch_t) * (1 + _regex_gga.re_nsub)];
      if ((_result_code = regexec(&_regex_gga, p_gps_frame.c_str(), 1 + _regex_gga.re_nsub, _matches, 0)) != 0) {
	delete [] _matches;
	regerror(_result_code, &_regex_gga, _error_msg, sizeof(_error_msg));
	std::cerr << "nmea2000::process_gga: regexec GGA failed: " << _error_msg << std::endl;
      
	return -1;
      }

      // Match
      //    std::cout << "nmea2000::process_gga: # of matching items: " << _matches->rm_eo << std::endl;
      // 1. Extract data
      if (_matches->rm_eo != -1) { // Matching
	std::string str, ind;
	char *pstr = (char *)p_gps_frame.c_str();
	/*      // UTC time
		size_t match_len = (_matches + 1)->rm_eo - (_matches + 1)->rm_so;
		str.assign(pstr + (_matches + 1)->rm_so, match_len);
		std::cout << "nmea2000::process_gga: utcTime = '" << str << "'" << std::endl;
		// Update UTC time
		//_values[utc_time_idx] = std::stof(); // UTC of position

		// Latitude coordinate.
		match_len = (_matches + 2)->rm_eo - (_matches + 2)->rm_so;
		str.assign(pstr + (_matches + 2)->rm_so, match_len);
		match_len = (_matches + 3)->rm_eo - (_matches + 3)->rm_so;
		ind.assign(pstr + (_matches + 3)->rm_so, match_len);
		std::cout << "nmea2000::process_gga: Latitude coordinate = '" << str << " " << ind << "'" << std::endl;
		_values[latitude_idx] = ind + str;

		// Longitude coordinate.
		match_len = (_matches + 4)->rm_eo - (_matches + 4)->rm_so;
		str.assign(pstr + (_matches + 4)->rm_so, match_len);
		match_len = (_matches + 5)->rm_eo - (_matches + 5)->rm_so;
		ind.assign(pstr + (_matches + 5)->rm_so, match_len);
		std::cout << "nmea2000::process_gga: Longitude coordinate = '" << str << " " << ind << "'" << std::endl;
		_values[longitude_idx] = ind + str;
	*/
	// Altitude.
	size_t match_len = (_matches + 9)->rm_eo - (_matches + 9)->rm_so;
	str.assign(pstr + (_matches + 9)->rm_so, match_len);
	//      std::cout << "nmea2000::process_gga: Altitude = '" << str << "'" << std::endl;
	_values[altitude_idx].assign(str);
      
	delete [] _matches;
      } // no matches

      return 0;
    } // End of method process_gga

    const int32_t nmea2000::process_rmc(const std::string & p_gps_frame) {
      // Extract fields
      _matches = (regmatch_t *)new uint8_t[sizeof(regmatch_t) * (1 + _regex_rmc.re_nsub)];
      if ((_result_code = regexec(&_regex_rmc, p_gps_frame.c_str(), 1 + _regex_rmc.re_nsub, _matches, 0)) != 0) {
	regerror(_result_code, &_regex_rmc, _error_msg, sizeof(_error_msg));
	delete [] _matches;
	std::cerr << "nmea2000::process_rmc: regexec RMC failed: " << _error_msg << std::endl;
      
	return -1;
      }

      // Match
      //    std::cout << "nmea2000::process_rmc: # of matching items: " << _matches->rm_eo << std::endl;
      // 1. Extract data
      if (_matches->rm_eo != -1) { // Matching
	std::string str, ind;
	char *pstr = (char *)p_gps_frame.c_str();
	// Status indicator
	size_t match_len = (_matches + 2)->rm_eo - (_matches + 2)->rm_so;
	ind.assign(pstr + (_matches + 2)->rm_so, match_len);
	//      std::cout << "nmea2000::process_rmc: Valid data = '" << ind << "'" << std::endl;
	if (ind.compare("A") == 0) { // Valida data,
	  std::ostringstream os;

	  // Update UTC time
	  match_len = (_matches + 1)->rm_eo - (_matches + 1)->rm_so;
	  str.assign(pstr + (_matches + 1)->rm_so, match_len);
	  //	std::cout << "nmea2000::process_rmc: utcTime = '" << str << "'" << std::endl;
	  _values[utc_time_idx] = str; // UTC of position

	  // Latitude coordinate.
	  match_len = (_matches + 3)->rm_eo - (_matches + 3)->rm_so;
	  str.assign(pstr + (_matches + 3)->rm_so, match_len);
	  match_len = (_matches + 4)->rm_eo - (_matches + 4)->rm_so;
	  ind.assign(pstr + (_matches + 4)->rm_so, match_len);
	  os << str << " " << ind;
	  _values[latitude_idx] = os.str(); // Latitude
	  //	std::cout << "nmea2000::process_rmc: Latitude coordinate = '" << _values[latitude_idx] << "'" << std::endl;

	  // Longitude coordinate.
	  match_len = (_matches + 5)->rm_eo - (_matches + 5)->rm_so;
	  str.assign(pstr + (_matches + 5)->rm_so, match_len);
	  match_len = (_matches + 6)->rm_eo - (_matches + 6)->rm_so;
	  ind.assign(pstr + (_matches + 6)->rm_so, match_len);
	  os.str("");
	  os << str << " " << ind;
	  _values[longitude_idx] = os.str(); // Longitude
	  //	std::cout << "nmea2000::process_rmc: Longitude coordinate = '" << _values[longitude_idx] << "'" << std::endl;

	  // Speed.
	  match_len = (_matches + 7)->rm_eo - (_matches + 7)->rm_so;
	  str.assign(pstr + (_matches + 7)->rm_so, match_len);
	  //	std::cout << "nmea2000::process_rmc: Speed = '" << str << "'" << std::endl;
	  _values[speed_idx] = str; // Speed

	  // Heading.
	  match_len = (_matches + 8)->rm_eo - (_matches + 8)->rm_so;
	  str.assign(pstr + (_matches + 8)->rm_so, match_len);
	  //	std::cout << "nmea2000::process_rmc: Heading = '" << str << "'" << std::endl;
	  _values[heading_idx] = str; // Heading.

	  // Date
	  match_len = (_matches + 9)->rm_eo - (_matches + 9)->rm_so;
	  str.assign(pstr + (_matches + 9)->rm_so, match_len);
	  os.str("");
	  os << str << " " << _values[utc_time_idx];
	  _values[utc_time_idx] = os.str();
	  //	  std::cout << "nmea2000::process_rmc: UTC date/time = '" << _values[utc_time_idx] << "'" << std::endl;
	} // else, discard the frame
      
	delete [] _matches;
      } // no matches

      return 0;
    } // End of method process_rmc

  } // End of namespace parsers

} // End of namespace gps
