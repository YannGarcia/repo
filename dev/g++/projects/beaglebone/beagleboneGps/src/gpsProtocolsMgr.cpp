#include "gpsProtocolsMgr.h"

#include "gpsNmeaAnalyzer.h"

namespace beagleboneGps {
	  
  std::unique_ptr<gpsProtocolsMgr> gpsProtocolsMgr::_instance(new gpsProtocolsMgr());
	      
  gpsProtocolsMgr::gpsProtocolsMgr() { 
    // Nothing to do
  } // End of Constructor

  gpsProtocolsMgr::~gpsProtocolsMgr() { 
    // Nothing to do
  } // End of Destructor

  void gpsProtocolsMgr::createProtocolAnalyzer(const gpsProtocolsMgr::Protocols p_protocol, std::unique_ptr<gpsProtocolsAnalyzer> & p_newInstance) {
    std::clog << ">>> gpsProtocolsMgr::createProtocolAnalyzer: " << (int)p_protocol << std::endl;

    std::unique_ptr<gpsProtocolsAnalyzer> ptr(nullptr);
    switch (p_protocol) {
    case gpsProtocolsMgr::nmea2000:
      std::clog << "gpsProtocolsMgr::createProtocolAnalyzer: process nmea2000" << std::endl;
      p_newInstance.reset(new gpsNmeaAnalyzer());
      break;
    } // End of 'switch' statement

    std::clog << "<<< gpsProtocolsMgr::createProtocolAnalyzer" << std::endl;
    return;
  }

} // End of namespace beagleboneGps

