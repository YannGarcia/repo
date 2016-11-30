/* Beaglebone check Iperf-3.x result description
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

#include "checkIperf.h"

#include "ipcCommon.h"

namespace wifimapping {

  checkIperf::checkIperf() {
    std::clog << ">>> checkIperf::checkIperf" << std::endl;

  } // End of Constructor
  
  checkIperf::~checkIperf() {
    std::clog << ">>> checkIperf::~checkIperf" << std::endl;
    uninitialize();
  }
  
  int checkIperf::initialize() {
    std::clog << ">>> checkIperf::initialize" << std::endl;

    if (_smMgr.open(IPERF_SEGMENT_ID) != 0) {
      std::cerr << "checkIperf::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      return -1;
    }

    _result.clear();
    _values.clear();

    return 0;
  }

  int checkIperf::uninitialize() {
    std::clog << ">>> checkIperf::uninitialize" << std::endl;
    return _smMgr.close();
  }

  void checkIperf::update() {
    //    std::clog << ">>> checkIperf::update" << std::endl;

    // Read share memory
    _result.clear();
    if (_smMgr.read(_result, IPERF_DATA_LENGTH) != 0) {
      std::cerr << "checkIperf::update: Failed to access the shared memory: " << std::strerror(errno) << std::endl;
      return;
    }
 
    // Update values
    _values.clear();
    unsigned int nvalues = _result[0];
    if (nvalues != 0) {  
      //      std::ostringstream os; // TODO To be removed
      //      os << "checkIperf::update: size:" << nvalues; // TODO To be removed
      for (unsigned int i = 0; i < nvalues; i++) {
	std::vector<unsigned char> buffer_float(sizeof(float));
	std::move(_result.begin() + 1, _result.begin() + 1 + i * sizeof(float),  buffer_float.begin());
	_values.push_back(converter::getInstance().bytes2float(buffer_float));
	//	os << " - " << _values[_values.size() - 1]; // TODO To be removed
      } // End of 'for' statement
      //      std::clog << os.str() << std::endl; // TODO To be removed
    }
    //    std::clog << "<<< checkIperf::update: size=" << (int)_values.size() << std::endl;
  } // End of method update
  
  void checkIperf::serialize(archive & p_archive) {
    //    std::clog << ">>> checkIperf::serialize: " << (int)_values.size() << std::endl;

    if (_values.size() != 0) {
      // Serialize shared memory segment identifier
      p_archive.serialize((unsigned char)IPERF_SEGMENT_ID);
      // serialize xfert
      p_archive.serialize(_values[0]);
      // serialize bw
      p_archive.serialize(_values[1]);
    } // TODO What happening otherwise?

  } // End of method serialize
  
} // End of namespace wifimapping
