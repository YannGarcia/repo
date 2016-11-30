#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

#include "converter.h"

//#include "dump.h" // TODO To be removed

namespace iperfmon {

  mainApp::mainApp(const std::string & p_client, const std::string & p_host, const int p_port, const int p_duration) : _smMgr(), _parmMgr(), _iperfImpl(p_client, p_host, p_port, p_duration), _samplePeriod(2e6) {
    std::clog << ">>> iperfmon::mainApp::mainApp: " << p_client << " - " << p_host << " - " << std::dec << p_port << " - " << p_duration << std::endl;

    if (_parmMgr.open(PARAMETER_SEGMENT_ID) < 0) {
      std::ostringstream os;
      os << "mainApp::mainApp: Failed to open the shared memory (PARAM): " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    if (_smMgr.open(IPERF_SEGMENT_ID) < 0) {
      std::ostringstream os;
      os << "mainApp::mainApp: Failed to open the shared memory (IPERF): " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

  } // End of constructor

  mainApp::~mainApp() {
    std::clog << "iperfmon::mainApp::~mainApp" << std::endl;
    _parmMgr.close();
    _smMgr.close();
  } // End of Destructor

  void mainApp::stop() {
    std::clog << ">>> iperfmon::mainApp::stop" << std::endl;
    _iperfImpl.close();
    runnable::stop();
  }

  void mainApp::run() {
    std::clog << ">>> iperfmon::mainApp::run" << std::endl;

    _running = true;
    _iperfImpl.clear();
    int result = 0;
    while(_running == true) {
      // Launch a measure
      result = _iperfImpl.acquire();
      if (result == 0) { // Got data
	// Save it
	std::vector<unsigned char> result;
	_iperfImpl.getResult(result);
	//      std::clog << "iperfmon::mainApp::run" << std::endl;
	//      dump::hexaDump(result.data(), result.size());
	if (_smMgr.write(result, result.size()) < 0) {
	  std::cerr << "iperfmonmon::mainApp::storeData: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
	}
      } else if (result == -1) { // Exit loop
	break;
      } // else, continue
      std::vector<unsigned char> buffer;
      if (_parmMgr.read(buffer, PARAMETER_DATA_LENGTH) < 0) {
	std::cerr << "iperfmon::mainApp::run: Failed to access the shared memory: " << std::strerror(errno) << std::endl;
      } else {
	_samplePeriod = converter::getInstance().bytes2float(buffer) * 0.8; // Use 80% of the main _samplePeriod to get data ready
	//	std::clog << "iperfmon::mainApp::run: new _samplePeriod (80%): " << _samplePeriod << std::endl;
      }
      usleep(_samplePeriod);
    } // End of 'while' statement

    std::clog << "<<< iperfmon::mainApp::run" << std::endl;
  }

} // End of namespace iperfmon
