/* Beaglebone check GPS result implementation
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
#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

#include "checkGps.h"

#include "ipcCommon.h"

namespace wifimapping {

  checkGps::checkGps() : _values() {
    std::clog << ">>> checkGps::checkGps" << std::endl;

  } // End of Constructor
  
  checkGps::~checkGps() {
    std::clog << ">>> checkGps::~checkGps" << std::endl;
    uninitialize();
  }
  
  int checkGps::initialize() {
    std::clog << ">>> checkGps::initialize" << std::endl;

    if (_smMgr.open(GPS_SEGMENT_ID) != 0) {
      std::cerr << "checkGps::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      return -1;
    }

    _result.clear();
    _values.clear();

    return 0;
  }

  int checkGps::uninitialize() {
    std::clog << ">>> checkGps::uninitialize" << std::endl;
    return _smMgr.close();
  }

  void checkGps::update() {
    //    std::clog << ">>> checkGps::update" << std::endl;

    // Read share memory
    _result.clear();
    if (_smMgr.read(_result, GPS_DATA_LENGTH) != 0) {
      std::cerr << "checkGps::update: Failed to access the shared memory: " << std::strerror(errno) << std::endl;
      return;
    }
 
    // Update values
    _values.clear();
    unsigned int nvalues = _result[0];
    if (nvalues != 0) {  
      unsigned int offset = sizeof(unsigned char);
      for (unsigned int i = 0; i < nvalues; i++) {
	std::string str;
	while (_result[offset] != 0x00) {
	  str += _result[offset++];
	} // End of 'while' statement
	//    	std::clog << ">>> checkGps::store '" << str << "'" << std::endl;
	_values.push_back(str);
	offset += 1; // Skip NULL character
      } // End of 'for' statement
    }

    //    std::clog << "<<< checkGps::update: size=" << (int)_values.size() << std::endl;
  } // End of method update
  
  void checkGps::serialize(archive & p_archive) {
    //    std::clog << ">>> checkGps::serialize: " << (int)_values.size() << std::endl;

    if (_values.size() != 0) {
      // Serialize shared memory segment identifier
      p_archive.serialize((unsigned char)GPS_SEGMENT_ID);
      // Serialize UTC date/time
      p_archive.serialize(_values[IDX_UTC_DATETIME]);
      // Serialize latitude
      p_archive.serialize(_values[IDX_LATITUDE]);
      // Serialize longiude
      p_archive.serialize(_values[IDX_LONGITUDE]);
      // Serialize speed
      p_archive.serialize(_values[IDX_SPEED]);
      // Serialize altitude
      p_archive.serialize(_values[IDX_ALTITUDE]);
      // Serialize heading
      p_archive.serialize(_values[IDX_HEADING]);
      // Serialize yawrate
      p_archive.serialize(_values[IDX_YAWRATE]);
    } // TODO What happening otherwise?

  } // End of method serialize
  
} // End of namespace wifimapping
