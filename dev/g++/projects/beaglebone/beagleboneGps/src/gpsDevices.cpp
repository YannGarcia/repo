#include <iostream>

#include "gpsDevices.h"

namespace beagleboneGps {
  
  gpsDevices::gpsDevices() {
    std::clog << ">>> gpsDevices::gpsDevices" << std::endl;
    _protocol = NULL;
    _gps = -1;
  } // End of Constructor
  
  gpsDevices::~gpsDevices() {
    std::clog << ">>> gpsDevices::~gpsDevices" << std::endl;
  } // End of Constructor
  
  int gpsDevices::initialize(std::unique_ptr<gpsProtocolsAnalyzer> & p_protocol) { 
    std::clog << ">>> gpsDevices::initialize" << std::endl;
    _protocol = NULL;
    _gps = -1;

    return 0;
  }

} // End of namespace beagleboneGps
