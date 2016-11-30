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
#if !defined(__MCP23x17_I2C_H__)
#define __MCP23x17_I2C_H__

#include "wiringBbb.h"

namespace beagleboneHw {

  namespace i2c {

    class mcp23x17_i2c {
 
      // TODO Use #define is memory space required
      static unsigned char DeviceId;
      static unsigned char IOCON; // See DS21952B-page 5 TABLE 1-2: REGISTER ADDRESSES 
      static unsigned char IODIR;
      static unsigned char IPOL;
      static unsigned char GPINTEN;
      static unsigned char DEFVAL;
      static unsigned char INTCON;
      static unsigned char GPPU;
      static unsigned char INTF;
      static unsigned char INTCAP;
      static unsigned char GPIO;
      static unsigned char OLAT;

      int _fd;
      unsigned char _intra;
      unsigned char _intrb;
      unsigned char _reset;

    public:
      enum PortId : unsigned char {
	PortA = 0x00,
	  PortB = 0x01
	  };

      enum IoId : unsigned char {
	Io0 = 0x00,
	  Io1 = 0x01,
	  Io2 = 0x02,
	  Io3 = 0x03,
	  Io4 = 0x04,
	  Io5 = 0x05,
	  Io6 = 0x06,
	  Io7 = 0x07
	  };

      enum GpioDirection : unsigned char {
	Input = 0x00,
	  Output = 0x01
	  };

      typedef GpioDirection PortDirection;
    
      enum IoLevel : unsigned char {
	Low = 0x00,
	  High = 0x01
	  };

    public:
      mcp23x17_i2c(const unsigned char p_deviceId, const unsigned char p_intra = 0xff, const unsigned char p_intrb = 0xff, const unsigned char p_reset = 0xff);
      virtual ~mcp23x17_i2c();

      void setPortDirection(const PortId p_portId, const PortDirection p_portDir, const bool p_pullupMode = false);
      void setIoDirection(const PortId p_portId, const IoId p_ioId, const PortDirection p_portDir, const bool p_pullupMode = false);

      void digitalWrite(const PortId p_portId, const IoId p_ioId, const IoLevel p_level);
      void digitalWrite(const PortId p_portId, const unsigned char p_gpio);

      void digitalRead(const PortId p_portId, const IoId p_ioId, IoLevel & p_data);
      void digitalRead(const PortId p_portId, unsigned char & p_gpio);

      typedef int (*IntrCallback)(void *);
      void setIntrCallback(const PortId p_portId, const IntrCallback p_callback);
      void readFromIsr(const PortId p_portId, unsigned char & p_isrFlags, unsigned char & p_gpio);

      inline void reset() { if (_reset != 0xff) { ::digitalWrite(_reset, LOW); usleep(1); ::digitalWrite(_reset, HIGH); } };

    private:
      bool _intraCallbackSet;
      bool _intrbCallbackSet;
      IntrCallback _intrCallback;

      void setDefaultConfig();

    }; // End of class mcp23x17_i2c

  } // End of namespace i2c

} // End of namespace beagleboneHw 

using namespace beagleboneHw::i2c;

#endif //  __MCP23x17_I2C_H__
