/* mbed simplified access to Microchip MCP28x17 GPIO expender devices (I2C)
 * Copyright (c) 2010-2012 ygarcia, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include <sstream>

#include "MCP23017_I2C.h"

#define IODIRA      0x00
#define IODIRB      0x01
#define IPOLA       0x02
#define IPOLB       0x03
#define GPINTENA    0x04
#define GPINTENB    0x05
#define DEFVALA     0x06
#define DEFVALB     0x07
#define INTCONA     0x08
#define INTCONB     0x09
#define IOCON       0x0a
#define GPPUA       0x0c
#define GPPUB       0x0d
#define INTFA       0x0e
#define INTFB       0x0f
#define INTCAPA     0x10
#define INTCAPB     0x11
#define GPIOA       0x12
#define GPIOB       0x13
#define OLATA       0x14
#define OLATB       0x15

namespace MCP23017_I2C {

  unsigned char CMCP23017_I2C::I2CModuleRefCounter = 0;

  CMCP23017_I2C::CMCP23017_I2C(const PinName p_sda, const PinName p_scl, const unsigned char p_address, const PinName p_intA, const PinName p_intB, const PinName p_reset, const bool p_internalPullUp, const unsigned int p_frequency) : _internalId("") {
    DEBUG_ENTER("CMCP23017_I2C")

      if (CMCP23017_I2C::I2CModuleRefCounter != 0) {
	error("CMCP23017_I2C: Wrong params");
      }
#ifdef __DEBUG
    std::ostringstream out(std::ostringstream::out);
    out << "CMCP23017_I2C #" << CMCP23017_I2C::I2CModuleRefCounter;
    _internalId.assign(out.str());
    DEBUG("CMCP23017_I2C: _internalId='%s'", _internalId.c_str())
#endif // __DEBUG
      _i2cInstance = new I2C(p_sda, p_scl);
    CMCP23017_I2C::I2CModuleRefCounter += 1;
    DEBUG_ENTER("CMCP23017_I2C: refCounter=%d", CMCP23017_I2C::I2CModuleRefCounter)

      _slaveAddress = (p_address << 1) | 0x40; // Slave address format is: 0 0 1 0 A3 A2 A1 R/W
    DEBUG("CMCP23017_I2C: I2C slave adress:  0x%02x", _slaveAddress)
      _i2cInstance->frequency(p_frequency); // Set the frequency of the I2C interface
        
    if (p_intA != NC) {
      DEBUG("CMCP23017_I2C: INTA managed");
      _intA = new InterruptIn(p_intA);
      if (p_internalPullUp) _intA->mode(PullDown);
      _intA->enable_irq(); // Enable interrupt
    } else {
      DEBUG("CMCP23017_I2C: INTA not managed");
      _intA = NULL; // Not used
    }
    if (p_intB != NC) {
      DEBUG("CMCP23017_I2C: INTB managed");
      _intB = new InterruptIn(p_intB);
      if (p_internalPullUp) _intB->mode(PullDown);
      _intB->enable_irq(); // Enable interrupt
    } else {
      DEBUG("CMCP23017_I2C: INTB not managed");
      _intB = NULL; // Not used
    }
    if (p_reset != NC) {
      DEBUG("CMCP23017_I2C: RESET managed");
      _reset = new DigitalOut(p_reset);
      _reset->write(1); // Disable reset
    } else {
      DEBUG("CMCP23017_I2C: RESET not managed");
      _reset = NULL; // Not used
    }
    
    // Configure default behavior
    configure();
        
    DEBUG_LEAVE("CMCP23017_I2C")
      }

  CMCP23017_I2C::~CMCP23017_I2C() {
    DEBUG_ENTER("~CMCP23017_I2C")
    
      // Release I2C instance
      DEBUG_ENTER("~CMCP23017_I2C: refCounter=%d", CMCP23017_I2C::I2CModuleRefCounter)
      CMCP23017_I2C::I2CModuleRefCounter -= 1;
    if (CMCP23017_I2C::I2CModuleRefCounter == 0) {
      delete _i2cInstance;
      _i2cInstance = NULL;
    }
    // Release _wp if required
    if (_intA != NULL) {
      delete _intA;
    }
    if (_intB != NULL) {
      delete _intB;
    }
    if (_reset != NULL) {
      delete _reset;
    }
    
    DEBUG_LEAVE("~CMCP23017_I2C")
      }
    
  void CMCP23017_I2C::reset() {
    if (_reset != NULL) {
      _reset->write(0);
      wait_us(1);
      _reset->write(1);
      wait_us(1);
      configure();
    }
  }
    
  int CMCP23017_I2C::read(const unsigned char p_portId) {
    return -1;
  }
    
  int CMCP23017_I2C::write(const unsigned char p_portId, const unsigned char p_value) {
    return -1;
  }
    
  unsigned char CMCP23017_I2C::createBus(const std::list<unsigned char> p_lines, const PinMode p_mode) {
    return 0xff;
  }

  void CMCP23017_I2C::deleteBus(const unsigned char p_busId) {
  }

  int CMCP23017_I2C::busRead(const unsigned char p_busId) {
    return -1;
  }

  int CMCP23017_I2C::busWrite(const unsigned char p_busId, const unsigned char p_value) {
    return -1;
  }

  void CMCP23017_I2C::configure() { // TODO Optimization with sequential access
    DEBUG_ENTER("CMCP23017_I2C::configure")
        
      // Setup IOCON - See REGISTER 1-6: IOCON – I/O EXPANDER CONFIGURATION REGISTER
      unsigned char vregister = 0x08;
    _write(IOCON, vregister);
    // Setup IODIR - See REGISTER 1-1: IODIR – I/O DIRECTION REGISTER (ADDR 0x00)
    vregister = 0x00; // All ports as output
    _write(IODIRA, vregister);
    _write(IODIRB, vregister);
    // Setup IPOL - See REGISTER 1-2: IPOL – INPUT POLARITY PORT REGISTER
    vregister = 0x00; // GPIO register bit will reflect the same logic state of the input pin
    _write(IPOLA, vregister);
    _write(IPOLB, vregister);
    // Setup GPPU - See REGISTER 1-7: GPPU – GPIO PULL-UP RESISTOR REGISTER
    vregister = 0x00; // Pull-up disabled
    _write(GPPUA, vregister);
    _write(GPPUB, vregister);
        
    // Setup interrupt       
    if (_intA != NULL) {
      DEBUG("CMCP23017_I2C::configure: INTA is supported")
      // Setup GPINTEN - See GPINTEN – INTERRUPT-ON-CHANGE PINS
      vregister = 0xff; // Enable GPIO input pin for interrupt-on-change event
      _write(GPINTENA, vregister);
      // Setup DEFVAL - See REGISTER 1-4: DEFVAL – DEFAULT VALUE REGISTER
      vregister = 0x00; // Pin level change from 0 to 1 raises an interrupt
      _write(DEFVALA, vregister);
      // Setup INTCON - REGISTER 1-5: INTCON – INTERRUPT-ON-CHANGE CONTROL REGISTER
      vregister = 0xff; // Pin level change from 0 to 1 raises an interrupt
      _write(INTCONA, vregister);
    }
    if (_intB != NULL) {
      DEBUG("CMCP23017_I2C::configure: INTB is supported")
      // Setup GPINTEN - See GPINTEN – INTERRUPT-ON-CHANGE PINS
      vregister = 0xff; // Enable GPIO input pin for interrupt-on-change event
      _write(GPINTENB, vregister);
      // Setup DEFVAL - See REGISTER 1-4: DEFVAL – DEFAULT VALUE REGISTER
      vregister = 0x00; // Pin level change from 0 to 1 raises an interrupt
      _write(DEFVALB, vregister);
      // Setup INTCON - REGISTER 1-5: INTCON – INTERRUPT-ON-CHANGE CONTROL REGISTER
      vregister = 0xff; // Pin level change from 0 to 1 raises an interrupt
      _write(INTCONB, vregister);
    }
        
    DEBUG_LEAVE("CMCP23017_I2C::configure")
      }
    
  bool CMCP23017_I2C::_write(const unsigned char p_address, const unsigned char p_byte) {
    DEBUG_ENTER("CMCP23017_I2C::_write (byte): Memory address: 0x%02x - 0x%02x", p_address, p_byte)
    
      // 1.Prepare buffer
      char i2cBuffer[2]; // Memory address + one byte of data
    // 1.1. Memory address
    i2cBuffer[0] = p_address;
    // 1.2. Datas
    i2cBuffer[1] = p_byte;
    
    // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
    int result = _i2cInstance->write(_slaveAddress, i2cBuffer, 2);
    wait(0.02);
    
    DEBUG_LEAVE("CMCP23017_I2C::_write (byte) %x", (bool)(result == 0))
      return (bool)(result == 0);
  }
    
} // End of namespace MCP23017_I2C
