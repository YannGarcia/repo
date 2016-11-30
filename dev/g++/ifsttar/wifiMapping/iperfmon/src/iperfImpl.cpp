/* Beaglebone iperf-3.0 client implementation
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
#include <cstring> // Used for std::strerror, memcpy

#include <unistd.h> // Used for usleep
#include <fcntl.h> // Used ioctl

#include "iperfImpl.h"

#include "ipcCommon.h" // Used for IPC settings values

#include "beagleboneCommEx.h"

namespace iperfmon {

  std::string iperfImpl::Pattern = "([[:digit:]]+\\.[[:digit:]]+)\\sMBytes\\s*([[:digit:]]+[\\.[[:digit:]]+]{0,1})\\sMbits/sec";

  iperfImpl::iperfImpl(const std::string & p_client, const std::string & p_host, const int p_port, const int p_duration) : _cmd(), _values(2), _xfers(), _bandwidths() {
    //    std::clog << "iperfImpl::iperfImpl: " << p_client << " - " << p_host << " - " << std::dec << p_port << " - " << p_duration << std::endl;

    // Initialize fields
    _pipe = NULL;

    // Setup iperf-3.x client command line
    std::ostringstream os;
    os << p_client << " -c " << p_host << " -p " << std::dec << p_port;// << " -r";
    if (p_duration > 0) {
      os << " -t " << p_duration;
    } // else, no expiration
    os << " -f m 2>&1 3>&1"; // Format output im Mbit/s
    _cmd = os.str();

    // Compile regular expression
    if (regcomp(&_regex, iperfImpl::Pattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
      std::ostringstream os;
      os << "iperfImpl::iperfImpl: Could not compile regex: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());      
    }
    
    std::clog << "iperfImpl::iperfImpl: command: " << _cmd << std::endl;
    if ((_pipe = popen(_cmd.c_str(), "r")) == NULL) {
      std::ostringstream os;
      os << "iperfImpl::iperfImpl: Failed to create pipe: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());      
    }
    // Set mode asynchronous
    int fd = fileno(_pipe); // Convert FILE * handle into file descriptor
    int flags; // Get current flag value
    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK; // Set asynchronous mode
    if (fcntl(fd, F_SETFL, flags) < 0) {
      std::cerr << "iperfImpl::iperfImpl: 'fcntl' failure: " << std::strerror(errno) << std::endl;
    }
  } // End of Constructor

  iperfImpl::~iperfImpl() {
    std::clog << ">>> iperfImpl::~iperfImpl" << std::endl;
    close();
  }

  int iperfImpl::clear() {
    if (fflush(_pipe) != 0) {
      std::cerr << "iperfImpl::iperfImpl: Failed to create pipe: " << std::strerror(errno) << std::endl;
      return -1;
    }

    return 0;
  }

  int iperfImpl::close() {
    std::clog << ">>> iperfImpl::close" << std::endl;
    if (_pipe != NULL) {
      if (pclose(_pipe) == -1) { // Terminate iperf client side
	std::cerr << "iperfImpl::~iperfImpl: 'pclose' failure: " << std::strerror(errno) << std::endl;
      }
      _pipe = NULL;
    }

    return 0;
  }

  int iperfImpl::acquire() {
    //    std::clog << ">>> iperfImpl::acquire" << std::endl;

    char line[512];
    if ((_pipe != NULL) && (fgets(line, 511, _pipe) != NULL)) {
      // Process input
      //      std::clog << "iperfImpl::acquire: " << line << std::endl;
      // Extract Transfert and Bandwidth
      setResult(line);

      //    std::clog << "<<< iperfImpl::acquire (0)" << std::endl;
      return 0;
    } else  if (errno != EWOULDBLOCK) { // No data available
      std::cerr << "iperfImpl::acquire: fgets failed: " << std::ferror(_pipe) << std::endl;

      //    std::clog << "<<< iperfImpl::acquire (-1)" << std::endl;
      return -1;
    } // else, no data available{

    //    std::clog << "<<< iperfImpl::acquire (-2)" << std::endl;
    return -2;
  } // End of function acquire

  void iperfImpl::setResult(const std::string & p_line) {
    //    std::clog << ">>> iperfImpl::setResult: " << p_line << std::endl;
    
    // Extract information
    _pmatches = (regmatch_t *)new unsigned char[sizeof(regmatch_t) * (1 + _regex.re_nsub)];
    if (regexec(&_regex, p_line.c_str(), 1 + _regex.re_nsub, _pmatches, 0) != 0) {
      std::cerr << "iperfImpl::setResult: regexec failed: " << std::strerror(errno) << std::endl;
      delete [] _pmatches;
      return;
    }
    // Match
    //    std::ostringstream os; // TODO To be removed
    //    os << "iperfImpl::setResult: "; // TODO To be removed
    // 1. Extract data
    if (_pmatches->rm_eo != -1) { // Matching
      std::string xfer, bw;
      char *pstr = (char *)p_line.c_str();
      // Xfert
      size_t matchLen = (_pmatches + 1)->rm_eo - (_pmatches + 1)->rm_so;
      xfer.assign(pstr + (_pmatches + 1)->rm_so, matchLen + 1);
      //      std::cout << "xfer = '" << xfer << "'" << std::endl;
      try {
      _xfers.push_back((float)std::stof(xfer));
      } catch (std::invalid_argument & i) {
	_xfers.push_back(0.0);
      }	
      //      os << (float)std::stof(xfer) << " - "; // TODO To be removed

      // Bandwidth
      matchLen = (_pmatches + 2)->rm_eo - (_pmatches + 2)->rm_so;
      bw.assign(pstr + (_pmatches + 2)->rm_so, matchLen + 1);
      try {
      _bandwidths.push_back((float)std::stof(bw));
      } catch (std::invalid_argument & i) {
	_bandwidths.push_back(0.0);
      }	
      //      std::cout << "bandwidth = '" << bw << "'" << std::endl;
      //      os << (float)std::stof(bw) << " - "; // TODO To be removed
      delete [] _pmatches;

      // 2. Compute average
      //      std::clog << "iperfImpl::setResult: size=" << (int)_xfers.size() << std::endl;
      if (_xfers.size() == 4/*Average based on 4 samples*/) {
	// Xfer
	_values[0] = 0.0;
	for (std::vector<float>::iterator it = _xfers.begin(); it != _xfers.end(); ++it) {
	  _values[0] += *it;
	} // End of 'switch' statement
	_values[0] /= 4;
	_xfers.clear();
	// Bandwidth
        _values[1] = 0.0;
	for (std::vector<float>::iterator it = _bandwidths.begin(); it != _bandwidths.end(); ++it) {
	  _values[1] += *it;
	} // End of 'switch' statement
	_values[1] /= 4;
	_bandwidths.clear();
      }
    } // else, no match

    //    std::clog << os.str() << std::endl; // TODO To be removed
    //    std::clog << "<<< iperfImpl::setResult: "  << _values.size() << std::endl;
  } // End of function setResult

  void iperfImpl::getResult(std::vector<unsigned char> & p_result) {
    //    std::clog << ">>> iperfImpl::getResult: "  << _values.size() << std::endl;

    std::ostringstream os; // TODO To be removed
    //    os << "iperfImpl::getResult: " << (float)_values[0] << " - " << (float)_values[1]; // TODO To be removed
    // Convert float into bytes array
    float *pfloat = _values.data();
    unsigned char *buffer = reinterpret_cast<unsigned char *>(pfloat);
    // Reserve space into the buffer
    p_result.assign(1 + _values.size() * sizeof(float), 0);
    p_result[0] = _values.size(); // # of items
    std::copy(buffer, buffer + _values.size() * sizeof(float), p_result.begin() + 1); // Values

    //    std::clog << os.str() << std::endl; // TODO To be removed
    //    std::clog << "<<< iperfImpl::getResult: "  << p_result.size() << std::endl;
  } // End of function getResult

} // End of namespace iperfmon
