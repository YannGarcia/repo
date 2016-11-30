#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

#include "beagleboneHwInit.h" // Used to initialize the wiringBbb

#include "ipcCommon.h" // IPC constants

#include "hwconfig.h" // Hardware configuration

namespace rtcmon {

  mainApp::mainApp(const std::string & p_host, const std::string & p_remote) : _smMgr(), _mcp7941x(nullptr), enableClock(true), _ntp(nullptr), _samplePeriod(8.0)/*8 seconds*/ {
    std::clog << ">>> rtcmon::mainApp::mainApp" << std::endl;

    // Create the shared memory
    if (_smMgr.open(RTC_SEGMENT_ID) != 0) {
      std::ostringstream os;
      os << "rtcmon::mainApp::initialize: Failed to open the shared memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Initialize the wiringBbb
    beagleboneHwInit::getInstance().initialize();
    
    // Setup RTC device
    _mcp7941x.reset(new mcp7941x_i2c); // i2cdetect -y 1 returns 0x6f
    _mcp7941x->setHourMode();
    _mcp7941x->enableClock(enableClock);
    _mcp7941x->setTime(time(NULL));

    // Setup Ntp client
    _ntp.reset(new network::ntpClient(p_remote, p_host));
  }

  mainApp::~mainApp() {
    std::clog << ">>> rtcmon::mainApp::~mainApp" << std::endl;
    _smMgr.close();
    _ntp.reset(nullptr);
    _mcp7941x.reset(nullptr);
  }

  void mainApp::stop() {
    std::clog << ">>> rtcmon::mainApp::stop" << std::endl;
    runnable::stop();
  }

  void mainApp::run() {
    std::clog << ">>> rtcmon::mainApp::run" << std::endl;

    _running = true;

    // Check NTP server
    if (_ntp->setTime() == 0) {
      _mcp7941x->setTime(time(NULL));
    }

    int timeCounter = 0;
    do {
      if (timeCounter == 3600) { // One hour expiry counter
	// Check NTP server
	if (_ntp->setTime() == 0) {
	  _mcp7941x->setTime(time(NULL));
	}
	// Reset time counter
	timeCounter = 0;
      }
      sleep(_samplePeriod);
      // Increment time counter
      timeCounter += 2;
    } while(_running == true); // End of 'do-while' statement

    std::clog << "<<< rtcmon::mainApp::run" << std::endl;
  }

} // End of namespace rtcmon
