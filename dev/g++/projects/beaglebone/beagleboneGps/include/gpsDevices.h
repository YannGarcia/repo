#if !defined(__GPS_DEVICES_H__)
#define __GPS_DEVICES_H__

#include <stdexcept> // used for std::runtime_error
#include <memory> // Used for std::unique_ptr
#include <vector>

#include "runnable.h" // Thread implementation

#include "gpsProtocolsAnalyzer.h"

namespace beagleboneGps {
  
  class gpsDevices : public runnable {
    
  protected:
    gpsProtocolsAnalyzer *_protocol; // Do not delete it in uninitialize method 
    int _gps;
    
  public:
    gpsDevices();
    virtual ~gpsDevices();
    
    virtual int initialize(std::unique_ptr<gpsProtocolsAnalyzer> & p_protocol);
    virtual int uninitialize() { throw new std::runtime_error("Method shall be overwritten"); };
      
    virtual void getResult(std::vector<unsigned char> & p_buffer) { if (_protocol != NULL) { _protocol->getResult(p_buffer); } else { p_buffer.clear(); }; };
      
    virtual void stop() { throw new std::runtime_error("Method shall be overwritten"); };

  private:
    virtual void run() { throw new std::runtime_error("Method shall be overwritten"); };
  }; // End of class gpsDevices

} // End of namespace beagleboneGps

using namespace beagleboneGps;

#endif // __GPS_DEVICES_H__
