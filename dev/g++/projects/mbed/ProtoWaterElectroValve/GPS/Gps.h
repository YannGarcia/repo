#pragma once

#include "mbed.h"

namespace gps { // See MODGPS
    
    class gps : Serial {
        
        double _latitude;
        double _longitude;
        double _elevation;
        InterruptIn * _gpsPps;

        
    public:
        gps(const PinName p_gpsRx, const PinName p_gpsTx, const PinName p_gpsPps = NC);
        virtual ~gps();
        
        inline double latitude() const { return _latitude; };
        inline double longitude() const { return _longitude; };
        inline double elevation() const { return _elevation; };
        
    }; // End of class gps
    
}; // End of namespace gps

using namespace gps;    