#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

#include "converter.h"

//#include "dump.h" // TODO To be removed

namespace gpsmon {

  mainApp::mainApp() : _smMgr(), _parmMgr(), _gps(nullptr), _nmea(nullptr), _samplePeriod(2e6) {
    std::clog << ">>> gpsmon::mainApp::mainApp" << std::endl;

    if (_parmMgr.open(PARAMETER_SEGMENT_ID) < 0) {
      std::ostringstream os;
      os << "gpsmon::mainApp::mainApp: Failed to open the shared memory (PARAM): " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    if (_smMgr.open(GPS_SEGMENT_ID) < 0) {
      std::ostringstream os;
      os << "gpsmon::mainApp::mainApp: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Create GPS protocol analyzer
    gpsProtocolsMgr::getInstance().createProtocolAnalyzer(gpsProtocolsMgr::nmea2000, _nmea);
    // Create an instance og GPS
    gpsDevicesMgr::getInstance().createGpsDevice(gpsDevicesMgr::ublox, _gps);
    _gps->initialize(_nmea);
  } // End of constructor

  mainApp::~mainApp() {
    std::clog << ">>> mainApp::~mainApp" << std::endl;

    _gps.reset(nullptr);
    _nmea.reset(nullptr);
    _parmMgr.close();
    _smMgr.close();
  }

  void mainApp::start() {
    std::clog << ">>> mainApp::start" << std::endl;
    // Start GPS processing
    _gps->start();
    // Start the application
    runnable::start();
  }

  void mainApp::stop() {
    std::clog << ">>> mainApp::stop" << std::endl;
    // Stop the application
    runnable::stop();
    // Stop GPS processing
    _gps->stop();
  }

  void mainApp::run() {
    std::clog << ">>> mainApp::run" << std::endl;

    _running = true;
    while(_running == true) {
      // Get GPS data
      std::vector<unsigned char> result;
      _gps->getResult(result);
      //      dump::hexaDump(result);
      // Store it into the shared memory
      if (_smMgr.write(result, result.size()) < 0) {
	std::cerr << "gpsmon::mainApp::run: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      }

      std::vector<unsigned char> buffer;
      if (_parmMgr.read(buffer, PARAMETER_DATA_LENGTH) < 0) {
	std::cerr << "iperfmon::mainApp::run: Failed to access the shared memory: " << std::strerror(errno) << std::endl;
      } else {
	_samplePeriod = converter::getInstance().bytes2float(buffer) * 0.8; // Use 80% of the main _samplePeriod to get data ready
	//	std::clog << "gpsmon::mainApp::run: new _samplePeriod (80%): " << _samplePeriod << std::endl;
      }
      usleep(_samplePeriod);
    } // End of 'while' statement

    std::clog << "<<< mainApp::run" << std::endl;
  }

} // End of namespace gpsmon
