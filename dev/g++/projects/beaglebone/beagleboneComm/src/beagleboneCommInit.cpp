#include <unistd.h>

#include "raspberryCommInit.h"
#include "wiringPi.h"

namespace raspberryComm {

  raspberryCommInit * raspberryCommInit::Instance = new raspberryCommInit();

  raspberryCommInit::~raspberryCommInit() { 
    delete raspberryCommInit::Instance; 
    raspberryCommInit::Instance = NULL;
  }

  raspberryCommInit & raspberryCommInit::getInstance() { 
    return *raspberryCommInit::Instance; 
  }

  void raspberryCommInit::initialize() {
    // Initialize the WiringPI library
    ::wiringPiSetup();
  }

} // End of namespace raspberryComm
