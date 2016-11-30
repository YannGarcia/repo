/* Beaglebone check TEMP result implementation
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

#include "checkTemp.h"

#include "ipcCommon.h"

namespace wifimapping {

  checkTemp::checkTemp() : _values() {
    std::clog << ">>> checkTemp::checkTemp" << std::endl;

  } // End of Constructor
  
  checkTemp::~checkTemp() {
    std::clog << ">>> checkTemp::~checkTemp" << std::endl;
    uninitialize();
  }
  
  int checkTemp::initialize() {
    std::clog << ">>> checkTemp::initialize" << std::endl;

    if (_smMgr.open(TEMP_SEGMENT_ID) != 0) {
      std::cerr << "checkTemp::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      return -1;
    }

    _result.clear();
    _values.clear();

    return 0;
  }

  int checkTemp::uninitialize() {
    std::clog << ">>> checkTemp::uninitialize" << std::endl;
    return _smMgr.close();
  }

  void checkTemp::update() {
    //    std::clog << ">>> checkTemp::update" << std::endl;

    // Read share memory
    _result.clear();
    if (_smMgr.read(_result, TEMP_DATA_LENGTH) != 0) {
      std::cerr << "checkTemp::update: Failed to access the shared memory: " << std::strerror(errno) << std::endl;
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

    //    std::clog << "<<< checkTemp::update: size=" << (int)_values.size() << std::endl;
  } // End of method update
  
  void checkTemp::serialize(archive & p_archive) {
    //    std::clog << ">>> checkTemp::serialize: " << (int)_values.size() << std::endl;

    if (_values.size() != 0) {
      // Serialize shared memory segment identifier
      p_archive.serialize((unsigned char)TEMP_SEGMENT_ID);
      // Ambiant temperature
      p_archive.serialize(_values[0]);
      // Duty cycle
      p_archive.serialize(_values[1]);
    } // TODO What happening otherwise?

  } // End of method serialize
  
} // End of namespace wifimapping
