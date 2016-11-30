#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

namespace accmon {

  mainApp::mainApp() : _smMgr() {
    std::clog << ">>> accmon::mainApp::mainApp" << std::endl;

    // Create the shared memory
    if (_smMgr.create() != 0) {
      std::ostringstream os;
      os << " accmon::mainApp::initialize: Failed to create the shared memory: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Create the pipe (GUI communication)

  }

  mainApp::~mainApp() {
    std::clog << ">>>  accmon::mainApp::~mainApp" << std::endl;
    _smMgr.close();
  }

  void mainApp::stop() {
    std::clog << ">>>  accmon::mainApp::stop" << std::endl;
  }

  void mainApp::run() {
    std::clog << ">>>  accmon::mainApp::run" << std::endl;

    _running = true;
    while(_running == true) {
      sleep(1);
    } // End of 'while' statement
    std::clog << std::endl;

    std::clog << "<<<  accmon::mainApp::run" << std::endl;
  }

} // End of namespace accmon
