#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

//#include "dump.h" // TODO To be removed

namespace tsharkmon {

  mainApp::mainApp(const std::string & p_client, const std::string & p_interface) : _smMgr(), _tsharkImpl(p_client, p_interface), _samplePeriod(500000) {
    //    std::clog << ">>> tsharkmon::mainApp::mainApp: " << p_client << " - " << p_interface << std::endl;

    if (_smMgr.open(WLAN_SEGMENT_ID) < 0) {
      std::ostringstream os;
      os << "mainApp::mainApp: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }
  } // End of constructor

  mainApp::~mainApp() {
    std::clog << "tsharkmon::mainApp::~mainApp" << std::endl;
    _smMgr.close();
  } // End of Destructor

  void mainApp::stop() {
    std::clog << ">>> tsharkmon::mainApp::stop" << std::endl;
    _tsharkImpl.close();
    runnable::stop();
  }

  void mainApp::run() {
    std::clog << ">>> tsharkmon::mainApp::run" << std::endl;

    _running = true;
    _tsharkImpl.clear();
    int result = 0;
    while(_running == true) {
      // Launch a measure
      result = _tsharkImpl.acquire();
      if (result == 0) { // Got data
	// Save it
	std::vector<unsigned char> result;
	_tsharkImpl.getResult(result);
	//	std::clog << "tsharkmon::mainApp::run" << std::endl;
	//	dump::hexaDump(result.data(), result.size());
	if (_smMgr.write(result, result.size()) < 0) {
	  std::cerr << "tsharkmon::mainApp::run: Failed to write into the shared memory: " << std::strerror(errno) << std::endl;
	}
      } else if (result == -1) { // Exit loop
	break;
      } // else, continue
      usleep(_samplePeriod);
    } // End of 'while' statement

    std::clog << "<<< tsharkmon::mainApp::run" << std::endl;
  }

} // End of namespace tsharkmon
