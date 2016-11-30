
#include "Gps.h"

namespace gps { // See MODGPS
    
    gps::gps(const PinName p_gpsRx, const PinName p_gpsTx, const PinName p_gpsPps) : 
        Serial(p_gpsTx, p_gpsRx),
        _latitude(0.0),
        _longitude(0.0),
        _elevation(0.0),
        _gpsPps(NULL)
    {
        baud(4800);
        if (p_gpsPps != NC) {
            _gpsPps = new InterruptIn(p_gpsPps);
            _gpsPps->enable_irq();
        }
    }
    
    gps::~gps() {
        if (_gpsPps != NULL) {
            delete _gpsPps;
        }
    }
    
}; // End of namespace gps
