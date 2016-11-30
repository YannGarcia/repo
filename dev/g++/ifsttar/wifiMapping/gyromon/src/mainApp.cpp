#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "raspberryCommEx.h"

namespace wifimapping {

  mainApp::mainApp() : _smMgr() {
    std::clog << ">>> mainApp::mainApp" << std::endl;

    // Create the shared memory
    if (_smMgr.create() != 0) {
      std::ostringstream os;
      os << "mainApp::initialize: Failed to create the shared memory: " << std::strerror(errno) << std::endl;
      throw new raspberryCommEx(os.str());
    }

    // Create the pipe (GUI communication)

  }

  mainApp::~mainApp() {
    std::clog << ">>> mainApp::~mainApp" << std::endl;
    _smMgr.close();
  }

  void mainApp::stop() {
    std::clog << ">>> mainApp::stop" << std::endl;
  }

  void mainApp::run() {
    std::clog << ">>> mainApp::run" << std::endl;

    _running = true;
    while(_running == true) {
      sleep(1);
      std::clog << ".";
    } // End of 'while' statement
    std::clog << std::endl;

    std::clog << "<<< mainApp::run" << std::endl;
  }

} // End of namespace wifimapping
