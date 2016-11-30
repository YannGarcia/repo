/* Beaglebone check RTC result implementation
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

#include "checkRtc.h"

#include "ipcCommon.h"

namespace wifimapping {

  checkRtc::checkRtc() : _status(0x00), _timestamp(0) {
    std::clog << ">>> checkRtc::checkRtc" << std::endl;

  } // End of Constructor
  
  checkRtc::~checkRtc() {
    std::clog << ">>> checkRtc::~checkRtc" << std::endl;
    uninitialize();
  }
  
  int checkRtc::initialize() {
    std::clog << ">>> checkRtc::initialize" << std::endl;

    if (_smMgr.open(RTC_SEGMENT_ID) != 0) {
      std::cerr << "checkRtc::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      return -1;
    }

    _result.clear();

    return 0;
  }

  int checkRtc::uninitialize() {
    std::clog << ">>> checkRtc::uninitialize" << std::endl;
    return _smMgr.close();
  }

  void checkRtc::update() {
    //    std::clog << ">>> checkRtc::update" << std::endl;

    _status = 0x00;
    _timestamp = time(NULL); // Current time

    /**
    // Read share memory
    _result.clear();
    if (_smMgr.read(_result, RTC_DATA_LENGTH) != 0) {
      std::cerr << "checkRtc::update: Failed to access the shared memory: " << std::strerror(errno) << std::endl;
      return;
    }
 
    // Update values
    _values.clear();
    unsigned int nvalues = _result[0];
    if (nvalues != 0) {  
      unsigned int offset = sizeof(unsigned char);
      for (unsigned int i = 0; i < nvalues; i++) {
	float value = *((float *)(_result.data() + offset));
	_values.push_back(value);
	  
	offset += sizeof(float);
      } // End of 'for' statement
    }
    */
    //    std::clog << "<<< checkRtc::update: size=" << (int)_values.size() << std::endl;
  } // End of method update
  
  void checkRtc::serialize(archive & p_archive) {
    //    std::clog << ">>> checkRtc::serialize: " << (int)_values.size() << std::endl;

    // Serialize shared memory segment identifier
    p_archive.serialize(static_cast<unsigned char>(RTC_SEGMENT_ID));
    // Serialize the status
    p_archive.serialize(_status);
    // Serialize the time stamp
    p_archive.serialize(_timestamp);
    
  } // End of method serialize
  
  std::string & checkRtc::toString() const {
    static std::string str;
    std::ostringstream os;

    os << "status: " << static_cast<unsigned int>(_status) << " - timestamp: " << _timestamp << std::endl;
    str = os.str();

    return str;
  } // End of method toString
  
} // End of namespace wifimapping
