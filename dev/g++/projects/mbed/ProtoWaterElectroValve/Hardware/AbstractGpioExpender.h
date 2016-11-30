#pragma once

#include <list>

#include <mbed.h>

class AbstractGpioExpender {
public:
    virtual int read(const unsigned char p_portId) = 0;
    virtual int write(const unsigned char p_portId, const unsigned char p_value) = 0;
    
    virtual unsigned char createBus(const std::list<unsigned char> p_lines, const PinMode p_mode = PullNone) = 0;
    virtual void deleteBus(const unsigned char p_busId) = 0;
    virtual int busRead(const unsigned char p_busId) = 0;
    virtual int busWrite(const unsigned char p_busId, const unsigned char p_value) = 0;
}; // End of class AbstractGpioExpender 