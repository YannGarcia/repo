#pragma once

#include "mbed.h"

class LedMgmt {
private:
   DigitalOut _ledPort;
public:
    LedMgmt(PinName p_ledPort);
    
    void On();
    void Off();
    void flash(float p_msec);
    LedMgmt& operator !=(const LedMgmt& p_ledMgmt);
}; // End of class LedMgmt
