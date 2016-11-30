#include <iostream>
#include <iomanip>

#include "gpsDevicesMgr.h"

#include "gpsDeviceUblox.h"

namespace beagleboneGps {
	  
  std::unique_ptr<gpsDevicesMgr> gpsDevicesMgr::_instance(new gpsDevicesMgr());
	      
  gpsDevicesMgr::gpsDevicesMgr() { 
    // Nothing to do
  } // End of Constructor

  gpsDevicesMgr::~gpsDevicesMgr() { 
    // Nothing to do
  } // End of Destructor

  void gpsDevicesMgr::createGpsDevice(const gpsDevicesMgr::Devices p_device, std::unique_ptr<gpsDevices> & p_newInstance) {
    std::clog << ">>> gpsDevicesMgr::createGpsDevice: " << (int)p_device << std::endl;

    switch (p_device) {
    case gpsDevicesMgr::ublox:
      std::clog << "gpsDevicesMgr::createGpsDevice: process ublox" << std::endl;
      p_newInstance.reset(new gpsDeviceUblox());
      break;
    } // End of 'switch' statement

    std::clog << "<<< gpsDevicesMgr::createGpsDevice" << std::endl;
    return;
  }

} // End of namespace beagleboneGps

