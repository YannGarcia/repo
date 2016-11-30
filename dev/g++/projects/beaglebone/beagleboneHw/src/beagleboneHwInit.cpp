#include <unistd.h>

#include "beagleboneHwInit.h"
#include "wiringBbb.h"

namespace beagleboneHw {

  beagleboneHwInit * beagleboneHwInit::Instance = new beagleboneHwInit();

  beagleboneHwInit::~beagleboneHwInit() { 
    delete beagleboneHwInit::Instance; 
    beagleboneHwInit::Instance = NULL;

    // Initialize the WiringPI library
    ::wiringBbbShutdownSys();
  }

  beagleboneHwInit & beagleboneHwInit::getInstance() { 
    return *beagleboneHwInit::Instance; 
  }

  void beagleboneHwInit::initialize() {
    // Initialize the WiringPI library
    ::wiringBbbSetupSys();
  }

} // End of namespace beagleboneHw
