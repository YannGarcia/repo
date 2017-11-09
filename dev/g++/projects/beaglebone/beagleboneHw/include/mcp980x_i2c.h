/* Beaglebone simplified access to Microchip MCP980x Temperature Sensor device (I2C) description
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
#if !defined(__MCP980x_I2C_H__)
#define __MCP980x_I2C_H__

#include <float.h>

#include "wiringBbb.h"

namespace beagleboneHw {

  namespace i2c {

    class mcp980x_i2c {
 
      // TODO Use #define is memory space required
      static unsigned char DeviceId;
      /** 
       * @desc Ambient Temperature register
       * \see DS21952B-page 14 Clause 5.3 Registers
       */
      static unsigned char TA;
      /** 
       * @desc Temperature Limit-set register
       * \see DS21952B-page 14 Clause 5.3 Registers
       */
      static unsigned char TSET;
      /** 
       * @desc Temperature Hysteresis register
       * \see DS21952B-page 14 Clause 5.3 Registers
       */
      static unsigned char THYST;
      /** 
       * @desc Device Configuration register
       * \see DS21952B-page 14 Clause 5.3 Registers
       */
      static unsigned char CONFIG;
  
      int _fd;
      unsigned char _alert;
      
    public:
      /**
       * \see DS21909D-page 18 REGISTER 5-3: CONFIGURATION REGISTER (CONFIG) – ADDRESS
       */
      enum AdcResolution : unsigned char {
	_0_5    = 0x00,
	_0_25 = 0x01,
	_0_125 = 0x02,
	_0_0625 = 0x03
      };

    
    public:
      mcp980x_i2c(const unsigned char p_deviceId, const unsigned char p_alert = 0xff);
      virtual ~mcp980x_i2c();

      /** 
       * @desc Get the ambiamt temperature
       * \return The ambiant temperature on success, FLT_MIN otherwise
       * \see DS21952B-page 16 Clause 5.3.1 AMBIENT TEMPERATURE REGISTER (TA)
       */
      const float readTemp() const;
      
      int setHysteresisTemp(const int p_hysteresisTemp);
      
      int setAlertTemp(const int p_alertTemp);
      
      int setAlertPolarity(const bool p_activeHigh = true);
      
    private:
      const unsigned short convertTemp2Bytes(const unsigned short p_temp) const;
      const float convertBytes2Temp(const unsigned short p_temp) const;
    }; // End of class mcp980x_i2c

  } // End of namespace i2c

} // End of namespace beagleboneHw 

using namespace beagleboneHw::i2c;

#endif //  __MCP23x17_I2C_H__
