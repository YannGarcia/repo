/* Beaglebone check Tshark description
 * Copyright (c) 2014 ygarcia, MIT License
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
#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

#include "checkTshark.h"

#include "ipcCommon.h"

#include "dump.h" // TODO To be removed

namespace wifimapping {

  checkTshark::checkTshark() {
    std::clog << ">>> checkTshark::checkTshark" << std::endl;

  } // End of Constructor
  
  checkTshark::~checkTshark() {
    std::clog << ">>> checkTshark::~checkTshark" << std::endl;
    uninitialize();
  }
  
  int checkTshark::initialize() {
    std::clog << ">>> checkTshark::initialize" << std::endl;

    if (_smMgr.open(WLAN_SEGMENT_ID) != 0) {
      std::cerr << "checkTshark::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      return -1;
    }

    _result.clear();
    _values.clear();

    return 0;
  }

  int checkTshark::uninitialize() {
    std::clog << ">>> checkTshark::uninitialize" << std::endl;
    return _smMgr.close();
  }

  void checkTshark::update() {
    //    std::clog << ">>> checkTshark::update" << std::endl;

    // Read share memory
    _result.clear();
    if (_smMgr.read(_result, WLAN_DATA_LENGTH) != 0) {
      std::cerr << "checkTshark::update: Failed to access the shared memory: " << std::strerror(errno) << std::endl;
      return;
    }
    //    std::clog << "checkTshark::update: " << std::endl;
    //    dump::hexaDump(_result.data(), _result.size());
    
    // Update values
    _values.clear();
    unsigned int bssids = _result[0]; // # of items in _values (== # of BSSID)
    //    std::clog << "checkTshark::update #bssid=" << bssids << std::endl;
    if (bssids != 0) {  
      int counter = 1;
      std::ostringstream os;
      for (unsigned int bssid = 0; bssid < bssids; bssid++) {
	// Read # of fields in the BSSID description
	unsigned int fields = _result[counter++];
	//	std::clog << "checkTshark::update #fields=" << fields << std::endl;
	std::vector<std::string> v;
	for (unsigned int field = 0; field < fields; field++) { 
	  while (_result[counter] != 0x00) { // NULL character
	    os << _result[counter++];
	  } // End of 'while' statement
	  counter += 1; // Skip NULL character
	  v.push_back(os.str());
	  os.str("");
	} // End of 'for' statement
	//	std::clog << "checkTshark::update: Insert key " << v[0] << ":" << v.size() << std::endl;
	_values.insert(std::pair<std::string, std::vector<std::string> >(v[0], v));
      } // End of 'for' statement
    }
    //    std::clog << "<<< checkTshark::update: size=" << (int)_values.size() << std::endl;
  } // End of method update
  
  void checkTshark::serialize(archive & p_archive) {
    //    std::clog << ">>> checkTshark::serialize: " << static_cast<unsigned int>(_values.size()) << std::endl;

    if (_values.size() != 0) {
      // Serialize shared memory segment identifier
      p_archive.serialize(static_cast<unsigned char>(WLAN_SEGMENT_ID));
      // Serialize #bssids
      //      std::clog << "checkTshark::serialize: #bssid=" << static_cast<unsigned int>(_values.size()) << std::endl;
      p_archive.serialize(static_cast<unsigned char>(_values.size()));
      // Serialize #bssids
      for (std::map<std::string, std::vector<std::string> >::const_iterator it = _values.begin(); it != _values.end(); ++it) {
	// Serialize # of fields in the BSSID description
	//	std::clog << "checkTshark::update #fields=" << it->second.size() << std::endl;
	p_archive.serialize(static_cast<unsigned char>(it->second.size()));
	// Serialize fields
	for (std::vector<std::string>::const_iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1) {	
	  p_archive.serialize(*it1);
	} // End of 'for' statement
      } // End of 'for' statement
    } // TODO What happening otherwise?

  } // End of method serialize
  
} // End of namespace wifimapping
