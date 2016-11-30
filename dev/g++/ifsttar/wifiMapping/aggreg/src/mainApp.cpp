/* Beaglebone aggreg application implementation
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
#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "abstractStorage.h"
#include "storageManager.h"

#include "beagleboneCommEx.h"

#include "datum.h"

//#include "dump.h" // TODO To be removed

namespace aggreg {

  mainApp::mainApp(std::string & p_storageType, std::string & p_storageDevice) : _mqMgr(), _storing(nullptr), _storageType(p_storageType), _storageDevice(p_storageDevice), _samplePeriod(1.0/*1 seconds*/) {
    std::clog << ">>> aggreg::mainApp::mainApp: " <<  _storageType << " - " << _storageDevice << std::endl;

    // Create the shared memory
    if (_mqMgr.open(mqMgr::dsMq) != 0) {
      std::ostringstream os;
      os << "aggreg::mainApp::initialize: Failed to open theData Storage message queue: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    abstractStorage::StorageFamily storageType = abstractStorage::binFile;
    if (_storageType.compare("txt") == 0) {
      storageType = abstractStorage::txtFile;
    } else if (_storageType.compare("kml") == 0) {
      storageType = abstractStorage::kmlFile;
    }

    storageManager::getInstance().createStorageDevice(storageType, _storing);
    _storing->initialize(_storageDevice);
  }

  mainApp::~mainApp() {
    std::clog << ">>> aggreg::mainApp::~mainApp" << std::endl;
    _storing->uninitialize();
    _storing.reset(nullptr);
    _mqMgr.close();
  }

  void mainApp::stop() {
    std::clog << ">>> aggreg::mainApp::stop" << std::endl;
    runnable::stop();
  }

  void mainApp::run() {
    std::clog << ">>> aggreg::mainApp::run" << std::endl;

    _running = true;

    _storing->open();
    std::vector<unsigned char> buffer;
    while(_running == true) {
      //      std::clog << "aggreg::mainApp::run: Call read" << std::endl;
      int result = _mqMgr.read(buffer, DS_MQ_SIZE);
      //      std::clog << "aggreg::mainApp::run: read returns " << result << std::endl;
      if (result > 0) { 
	//	dump::hexaDump(buffer);
	_storing->store(buffer);
      }
      buffer.clear();

      sleep(_samplePeriod);
    } // End of 'while' statement
    _storing->close();

    std::clog << "<<< aggreg::mainApp::run" << std::endl;
  }

} // End of namespace aggreg
