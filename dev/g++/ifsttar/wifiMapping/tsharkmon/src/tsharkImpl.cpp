/* Beaglebone tshark implementation
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
#include <algorithm> // Used for std::for_each

#include <unistd.h> // Used for usleep
#include <fcntl.h> // Used ioctl

#include "tsharkImpl.h"

#include "ipcCommon.h" // Used for IPC settings values

#include "beagleboneCommEx.h"

namespace tsharkmon {

  std::string tsharkImpl::Pattern = "([[:xdigit:]]{2}:[[:xdigit:]]{2}:[[:xdigit:]]{2}:[[:xdigit:]]{2}:[[:xdigit:]]{2}:[[:xdigit:]]{2})\t([[:digit:]]+)\t(-{0,1}[[:digit:]]+)\t([[:digit:]]+)\t(.*)\n";
  
  tsharkImpl::tsharkImpl(const std::string & p_client, const std::string & p_interface) : _cmd(), _pipe(NULL), _values(), _history() {
    //    std::clog << "tsharkImpl::tsharkImpl: " << p_client << " - " << p_interface << std::endl;

    // Setup tshark-3.x client command line
    std::ostringstream os;
    os << p_client << " -i" << p_interface << " -p -I -y IEEE802_11_RADIO -T fields -e wlan.bssid -e radiotap.datarate -e radiotap.dbm_antsignal -e wlan_mgt.ds.current_channel -e wlan_mgt.ssid wlan[0] == 0x80 2>&1 3>&1";
    _cmd = os.str();

    // Compile regular expression
    if (regcomp(&_regex, tsharkImpl::Pattern.c_str(), REG_EXTENDED | REG_NEWLINE) != 0) {
      std::ostringstream os;
      os << "tsharkImpl::tsharkImpl: Could not compile regex: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());      
    }
    
    std::clog << "tsharkImpl::tsharkImpl: command: " << _cmd << std::endl;
    if ((_pipe = popen(_cmd.c_str(), "r")) == NULL) {
      std::ostringstream os;
      os << "tsharkImpl::tsharkImpl: Failed to create pipe: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());      
    }
    // Set mode asynchronous
    int fd = fileno(_pipe); // Convert FILE * handle into file descriptor
    int flags; // Get current flag value
    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK; // Set asynchronous mode
    fcntl(fd, F_SETFL, flags);
  } // End of Constructor

  tsharkImpl::~tsharkImpl() {
    std::clog << ">>> tsharkImpl::~tsharkImpl" << std::endl;
    close();
  }

  int tsharkImpl::clear() {
    if (fflush(_pipe) != 0) {
      std::cerr << "tsharkImpl::tsharkImpl: Failed to create pipe: " << std::strerror(errno) << std::endl;
      return -1;
    }

    return 0;
  }

  int tsharkImpl::close() {
    std::clog << ">>> tsharkImpl::close" << std::endl;
    if (_pipe != NULL) {
      if (pclose(_pipe) == -1) { // Terminate tshark client side
	std::cerr << "tsharkImpl::~tsharkImpl: 'pclose' failure: " << std::strerror(errno) << std::endl;
      }
      _pipe = NULL;
    }

    return 0;
  }

  int tsharkImpl::acquire() {
    //    std::clog << ">>> tsharkImpl::acquire" << std::endl;

    char line[512];
    if ((_pipe != NULL) && (fgets(line, 511, _pipe) != NULL)) {
      // Process input
      //      std::clog << "tsharkImpl::acquire: " << line << std::endl;
      // Extract Transfert and Bandwidth
      setResult(line);

      //      std::clog << "<<< tsharkImpl::acquire (0)" << std::endl;
      return 0;
    } else  if (errno != EWOULDBLOCK) { // No data available
      std::cerr << "tsharkImpl::acquire: fgets failed: " << std::ferror(_pipe) << std::endl;

      //      std::clog << "<<< tsharkImpl::acquire (-1)" << std::endl;
      return -1;
    } // else, no data available{

    //    std::clog << "<<< tsharkImpl::acquire (-2)" << std::endl;
    return -2;
  } // End of function acquire

  void tsharkImpl::setResult(const std::string & p_line) {
    //    std::clog << ">>> tsharkImpl::setResult: '" << p_line << "'" << std::endl;

    // Purge the current vector
    _values.clear();

    // Extract information
    _pmatches = (regmatch_t *)new unsigned char[sizeof(regmatch_t) * (1 + _regex.re_nsub)];
    if (regexec(&_regex, p_line.c_str(), 1 + _regex.re_nsub, _pmatches, 0) != 0) {
      std::cerr << "tsharkImpl::setResult: regexec failed: " << std::strerror(errno) << std::endl;
      delete [] _pmatches;
      return;
    }
    // Match
    // 1. Extract data
    if (_pmatches->rm_eo != -1) { // Matching
      std::string bssid, pwr, dr, channel, essid;
      char *pstr = (char *)p_line.c_str();
      // BSSID
      size_t matchLen = (_pmatches + 1)->rm_eo - (_pmatches + 1)->rm_so;
      bssid.assign(pstr + (_pmatches + 1)->rm_so, matchLen);
      //      std::cout << "BSSID = '" << bssid << "' - " << bssid.length() << std::endl;

      // Data Rate
      matchLen = (_pmatches + 2)->rm_eo - (_pmatches + 2)->rm_so;
      dr.assign(pstr + (_pmatches + 2)->rm_so, matchLen);
      //      std::cout << "Data Rate = '" << dr << "' - " << dr.length() << std::endl;

      // Power
      matchLen = (_pmatches + 3)->rm_eo - (_pmatches + 3)->rm_so;
      pwr.assign(pstr + (_pmatches + 3)->rm_so, matchLen);
      //      std::cout << "Pwr = '" << pwr << "' - " << pwr.length() << std::endl;

      // Channel
      matchLen = (_pmatches + 4)->rm_eo - (_pmatches + 4)->rm_so;
      channel.assign(pstr + (_pmatches +4)->rm_so, matchLen);
      //      std::cout << "channel = '" << channel << "' - " << channel.length() << std::endl;

      // ESSID
      matchLen = (_pmatches + 5)->rm_eo - (_pmatches + 5)->rm_so;
      essid.assign(pstr + (_pmatches + 5)->rm_so, matchLen);
      //      std::cout << "essid = '" << essid << "' - " << essid.length() << std::endl;

      if (_values.find(bssid) == _values.end()) { // New item
	// Add new item
	std::vector<std::string> v;
	v.push_back(dr);
	v.push_back(pwr);
	v.push_back(channel);
	v.push_back(essid);
	_values.insert(std::pair<std::string, std::vector<std::string> >(bssid, v));
      } else { // Update item
	_values[bssid][0] = dr;
	_values[bssid][1] = pwr;
	_values[bssid][2] = channel;
	_values[bssid][3] = essid;
      } 
      /*if (_history.find(bssid) == _history.end()) { // New item
	_history.push_back(bssid);
	}*/

      delete [] _pmatches;
    } // else, no mathin

    //    std::clog << "<<< tsharkImpl::setResult: "  << _values.size() << std::endl;
  } // End of function setResult

  void tsharkImpl::getResult(std::vector<unsigned char> & p_result) {
    //    std::clog << ">>> tsharkImpl::getResult: "  << _values.size() << std::endl;

    // Add number of records
    p_result.clear();
    p_result.push_back(_values.size()); // # of items in _values (== # of BSSID)
    for (std::map<std::string, std::vector<std::string> >::iterator it = _values.begin(); it != _values.end(); ++it) {
      p_result.push_back(it->second.size() + 1); // # of fields in the BSSID description
      std::copy(it->first.begin(), it->first.end(), std::back_inserter(p_result)); // Add BSSID
      p_result.push_back(0x00);      
      for (unsigned int i = 0; i < it->second.size(); i++) { // Add all fields of the BSSID description
	std::copy(it->second[i].begin(), it->second[i].end(), std::back_inserter(p_result));
	p_result.push_back(0x00);      
      } // End of 'for' statement 
    } // End of 'for' statement
    
    //    std::clog << "<<< tsharkImpl::getResult: "  << p_result.size() << std::endl;
  } // End of function getResult

} // End of namespace tsharkmon
