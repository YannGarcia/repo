
/* Raspberry simplified access to Microchip MCP23x17 GPIO expender device (I2C)
 * Copyright (c) 2013 ygarcia, MIT License
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
#include <iomanip>
#include <cstring>

#include <unistd.h> // Used for usleep and close

#include "mcp23x17_i2c.h"
#include "wiringBbbI2C.h" // Using I2C functions
#include "beagleboneHwEx.h"

// Bits in the IOCON register - See DS21952B-page 17 Clause 1.6.6 CONFIGURATION REGISTER
#define IOCON_UNUSED    0x01
#define IOCON_INTPOL    0x02
#define IOCON_ODR       0x04
#define IOCON_HAEN      0x08
#define IOCON_DISSLW    0x10
#define IOCON_SEQOP     0x20
#define IOCON_MIRROR    0x40
#define IOCON_BANK_MODE 0x80

namespace beagleboneHw {

  namespace i2c {

    unsigned char mcp23x17_i2c::DeviceId = 0x20;

    unsigned char mcp23x17_i2c::IOCON   = 0x0A; // See DS21952B-page 5 TABLE 1-2: REGISTER ADDRESSES 
    unsigned char mcp23x17_i2c::IODIR   = 0x00;
    unsigned char mcp23x17_i2c::IPOL    = 0x02;
    unsigned char mcp23x17_i2c::GPINTEN = 0x04;
    unsigned char mcp23x17_i2c::DEFVAL  = 0x06;
    unsigned char mcp23x17_i2c::INTCON  = 0x08;
    unsigned char mcp23x17_i2c::GPPU    = 0x0C;
    unsigned char mcp23x17_i2c::INTF    = 0x0E;
    unsigned char mcp23x17_i2c::INTCAP  = 0x10;
    unsigned char mcp23x17_i2c::GPIO    = 0x12;
    unsigned char mcp23x17_i2c::OLAT    = 0x14;

    mcp23x17_i2c::mcp23x17_i2c(const unsigned char p_deviceId, const unsigned char p_intra, const unsigned char p_intrb, const unsigned char p_reset) { // TODO Add freqency support
      // Setup I2C support
      DeviceId |= (p_deviceId & 0x07);
      if ((_fd = wiringBbbI2CSetup(mcp23x17_i2c::DeviceId)) == -1) {
	std::cerr << "mcp23x17_i2c::Failed to initialize adress 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)mcp23x17_i2c::DeviceId << " (MCP23x17)" << std::endl;
	throw beagleboneHwEx("mcp23x17_i2c::mcp23x17_i2c: wiringBbbI2CSetup failure");
      }
      std::clog << "mcp23x17_i2c::mcp23x17_i2c: Device address=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)mcp23x17_i2c::DeviceId << " - fd=" << std::dec << _fd << std::endl;

      // TODO Declare wiringBbb node

      // Setup IOCON register
      setDefaultConfig();
    
      // Set all port in INPUT mode
      setPortDirection(mcp23x17_i2c::PortA, mcp23x17_i2c::Input);
      setPortDirection(mcp23x17_i2c::PortB, mcp23x17_i2c::Input);

      // Read INTCAP to clear interrupt on port A and B
      unsigned char gpio;
      digitalRead(mcp23x17_i2c::PortA, gpio);
      digitalRead(mcp23x17_i2c::PortB, gpio); 

      // Manage ports
      _intraCallbackSet = false;
      _intrbCallbackSet = false;
      _intra = p_intra;
      if(_intra != 0xff) { // INTRA pin is not used
	::pinMode(_intra, INPUT);
      }
      _intrb = p_intrb;
      if(_intrb != 0xff) { // INTRB pin is not used
	::pinMode(_intrb, INPUT);
      }
      _reset = p_reset;
      if(_reset != 0xff) { // RESET pin is used
	::pinMode(_reset, OUTPUT);
	::digitalWrite(_reset, HIGH); // RESET is active at low level
      }
 
    } // End of Constructor

    mcp23x17_i2c::~mcp23x17_i2c() {
      if(_reset != 0xff) { // RESET pin was used
	::pinMode(_reset, INPUT);
      }

      if (_fd != -1) {
	close(_fd);
      }
    } // End of Destructor

    void mcp23x17_i2c::setDefaultConfig() {
      // Setup IOCON register
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::IOCON, 
			   IOCON_SEQOP | IOCON_HAEN 
			   );
      // Setup GPIO registers
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::IPOL, // PortA 
			   0x00 // GPIO register bits reflect the same logic state of the input pins - See  DS21952B-page 13 1.6.2 INPUT POLARITY REGISTER
			   );
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::IPOL + 1, // PortB 
			   0x00 // GPIO register bits reflect the same logic state of the input pins - See  DS21952B-page 13 1.6.2 INPUT POLARITY REGISTER
			   );
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::GPINTEN, // PortA 
			   0xff // I/O pin are enabled for interrupt-on-change event - See  DS21952B-page 14 1.6.3 INTERRUPT-ON-CHANGE CONTROL REGISTER
			   );
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::GPINTEN + 1, // PortB 
			   0xff // I/O pin are enabled for interrupt-on-change event - See  DS21952B-page 14 1.6.3 INTERRUPT-ON-CHANGE CONTROL REGISTER
			   );

      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::DEFVAL, // PortA 
			   0x00 // I/O pin are compared against the previous value - See  DS21952B-page 15 1.6.4 DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE 
			   );
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::DEFVAL + 1, // PortB
			   0x00 // I/O pin are compared against the previous value - See  DS21952B-page 15 1.6.4 DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE 
			   );

      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::INTCON, // PortA 
			   0x00 // I/O pin are compared against the previous value - See  DS21952B-page 16 1.6.5 INTERRUPT CONTROL REGISTER 
			   );
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::INTCON + 1, // PortB
			   0x00 // I/O pin are compared against the previous value - See  DS21952B-page 16 1.6.5 INTERRUPT CONTROL REGISTER 
			   );

      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::GPPU, // PortA 
			   0x00 // Pull-up I/O pins are disabled - See  DS21952B-page 19 1.6.7 PULL-UP RESISTOR CONFIGURATION REGISTER 
			   );
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::GPPU + 1, // PortB 
			   0x00 // Pull-up I/O pins are disabled - See  DS21952B-page 19 1.6.7 PULL-UP RESISTOR CONFIGURATION REGISTER 
			   );

      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::INTF, // PortA 
			   0x00 // I/O pins interrupt are disabled - See  DS21952B-page 20 1.6.8 INTERRUPT FLAG REGISTER
			   );
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   mcp23x17_i2c::INTF + 1, // PortB 
			   0x00 // I/O pins interrupt are disabled - See  DS21952B-page 20 1.6.8 INTERRUPT FLAG REGISTER
			   );
    }

    void mcp23x17_i2c::setPortDirection(const PortId p_portId, const PortDirection p_portDir, const bool p_pullupMode) {
      // Set pullup mode
      if (p_pullupMode == true) {
	::wiringBbbI2CWriteReg8(
			     _fd, 
			     (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPPU : mcp23x17_i2c::GPPU + 1, 
			     0xff
			     );
      } else {
	::wiringBbbI2CWriteReg8(
			     _fd, 
			     (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPPU : mcp23x17_i2c::GPPU + 1, 
			     0x00
			     );
      }
      // Set port direction
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::IODIR : mcp23x17_i2c::IODIR + 1, 
			   (p_portDir == Output) ? 0x00 : 0xff 
			   );
    }

    void mcp23x17_i2c::setIoDirection(const PortId p_portId, const IoId p_ioId, const PortDirection p_portDir, const bool p_pullupMode) {
      // Set pullup mode
      unsigned char gppu = ::wiringBbbI2CReadReg8(
					       _fd, 
					       (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPPU : mcp23x17_i2c::GPPU + 1
					       );
      std::clog << "mcp23x17_i2c::setIoDirection: gppu=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)gppu << std::endl;
      if (p_pullupMode == true) { // Set pullup for the specified pin - See DS21952B-page 19 Clause 1.6.7 PULL-UP RESISTOR CONFIGURATION REGISTER
	if ((gppu & (1 << p_ioId)) == 0x00) { // PUn bit not set
	  std::clog << "mcp23x17_i2c::setIoDirection: new gppu=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)(gppu | (1 << p_ioId)) << std::endl;
	  ::wiringBbbI2CWriteReg8(
			       _fd, 
			       (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPPU : mcp23x17_i2c::GPPU + 1, 
			       gppu | (1 << p_ioId)
			       );
	} // else PUn bit is already set
      } else { // Check if PUn pin is set
	if ((gppu & (1 << p_ioId)) == 0x01) { // Clear PUn bit
	  std::clog << "mcp23x17_i2c::setIoDirection: new gppu=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)(gppu ^ (1 << p_ioId)) << std::endl;
	  ::wiringBbbI2CWriteReg8(
			       _fd, 
			       (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPPU : mcp23x17_i2c::GPPU + 1, 
			       gppu ^ (1 << p_ioId)
			       );
	}
      }
      // Read current settings
      unsigned char iodir = ::wiringBbbI2CReadReg8(
						_fd, 
						(p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::IODIR : mcp23x17_i2c::IODIR + 1
						);
      std::clog << "mcp23x17_i2c::setIoDirection: iodir=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)iodir << std::endl;
      // Change IO pin direction
      if (p_portDir == Input) { // Set bit to 1 for input
	iodir |= (1 << (unsigned int)p_ioId);
      } else { // Set bit to 0 for input
	iodir ^= (1 << (unsigned int)p_ioId);
      }
      // Save it
      std::clog << "mcp23x17_i2c::setIoDirection: new iodir=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)iodir << std::endl;
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::IODIR : mcp23x17_i2c::IODIR + 1, 
			   iodir 
			   );
    }

    void mcp23x17_i2c::digitalWrite(const PortId p_portId, const IoId p_ioId, const IoLevel p_level) {
      // Read current GPIO register
      unsigned char gpio = ::wiringBbbI2CReadReg8(
					       _fd, 
					       (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPIO : mcp23x17_i2c::GPIO + 1
					       );
      std::clog << "mcp23x17_i2c::digitalWrite: gpio=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)gpio << std::endl;
      // Change IO pin level
      if (p_level == High) { // Set bit to 1
	gpio |= (1 << (unsigned int)p_ioId);
      } else { // Set bit to 0
	gpio ^= (1 << (unsigned int)p_ioId);
      }
      // Save it
      std::clog << "mcp23x17_i2c::digitalWrite: new gpio=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)gpio << std::endl;
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPIO : mcp23x17_i2c::GPIO + 1, 
			   gpio 
			   );
    }

    void mcp23x17_i2c::digitalWrite(const PortId p_portId, const unsigned char p_gpio) {
      // Write GPIO register
      ::wiringBbbI2CWriteReg8(
			   _fd, 
			   (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPIO : mcp23x17_i2c::GPIO + 1, 
			   p_gpio
			   );
    }

    void mcp23x17_i2c::digitalRead(const PortId p_portId, const IoId p_ioId, IoLevel & p_data) {
      unsigned char gpio;
      digitalRead(p_portId, gpio);
      p_data = (gpio & (1 << p_ioId)) ? mcp23x17_i2c::High : mcp23x17_i2c::Low;
    }

    void mcp23x17_i2c::digitalRead(const PortId p_portId, unsigned char & p_gpio) {
      p_gpio = ::wiringBbbI2CReadReg8(
				   _fd, 
				   (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::GPIO : mcp23x17_i2c::GPIO + 1
				   );
      std::clog << "mcp23x17_i2c::digitalRead: port" << (char)((p_portId == mcp23x17_i2c::PortA) ? 'A' : 'B') << " - gpio:" << std::hex << std::setw(2) << std::setfill('0') << (int)p_gpio << std::endl;
    }

    void mcp23x17_i2c::setIntrCallback(const PortId p_portId, const IntrCallback p_callback) {
      std::clog << "mcp23x17_i2c::setIntrCallback: port" << (char)((p_portId == mcp23x17_i2c::PortA) ? 'A' : 'B') << std::endl;
      if ((p_portId == mcp23x17_i2c::PortA) && (_intra != 0xff)) {
	::wiringBbbISR(_intra, INT_EDGE_RISING, p_callback);
	_intraCallbackSet = true;
      } else if ((p_portId == mcp23x17_i2c::PortB) && (_intrb != 0xff)) {
	::wiringBbbISR(_intrb, INT_EDGE_RISING, p_callback);
	_intrbCallbackSet = true;
      } else {
	std::cerr << "mcp23x17_i2c::setIntrCallback: Wrong parameters" << std::endl;
      } 
    }

    void mcp23x17_i2c::readFromIsr(const PortId p_portId, unsigned char & p_isrFlags, unsigned char & p_gpio) {
      // Read INTF register to clear INTR - See DS21952B-page 20 Clause 1.6.8 INTERRUPT FLAG REGISTER
      p_isrFlags = ::wiringBbbI2CReadReg8(
				       _fd, 
				       (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::INTF : mcp23x17_i2c::INTF + 1
				       );
      std::clog << "mcp23x17_i2c::readFromIsr: isrFlags=0x" << std::hex << std::setw(2) << (int)p_isrFlags << std::endl;
      // Read INTCAP register to clear INTR - See DS21952B-page 21 Clause 1.6.9 INTERRUPT CAPTURE REGISTER
      p_gpio = ::wiringBbbI2CReadReg8(
				   _fd, 
				   (p_portId == mcp23x17_i2c::PortA) ? mcp23x17_i2c::INTCAP : mcp23x17_i2c::INTCAP + 1
				   );
      std::clog << "mcp23x17_i2c::readFromIsr: gpio=0x" << std::hex << std::setw(2) << (int)p_gpio << std::endl;
    }

  } // End of namespace i2c

} // End of namespace beagleboneHw
