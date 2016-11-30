/* Beaglebone simplified access to Microchip MCP980x Temperature Sensor device (I2C) implementation
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

#include "mcp980x_i2c.h"
#include "wiringBbbI2C.h" // Using I2C functions
#include "beagleboneHwEx.h"

#include "converter.h"

namespace beagleboneHw {

  namespace i2c {

    unsigned char mcp980x_i2c::DeviceId = 0x48; // See DS21909D-page 9 TABLE 3-2: SLAVE ADDRESS

    // See DS21909D-page 14 REGISTER 5-1: REGISTER POINTER
    unsigned char mcp980x_i2c::TA     = 0x00; // Px<1:0>: 00 = Temperature register
    unsigned char mcp980x_i2c::CONFIG = 0x01; // Px<1:0>: 01 = Configuration register
    unsigned char mcp980x_i2c::THYST  = 0x02; // Px<1:0>: 10 = Temperature Hysteresis register
    unsigned char mcp980x_i2c::TSET   = 0x03; // Px<1:0>: 11 = Temperature Limit-set register
    
    mcp980x_i2c::mcp980x_i2c(const unsigned char p_deviceId, const unsigned char p_alert) { // TODO Add freqency support
      // Setup I2C support
      DeviceId |= (p_deviceId & 0x07);
      if ((_fd = wiringBbbI2CSetup(mcp980x_i2c::DeviceId)) == -1) {
	std::cerr << "Failed to initialize adress 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)mcp980x_i2c::DeviceId << " (MCP980x)" << std::endl;
	throw beagleboneHwEx("mcp980x_i2c: wiringBbbI2CSetup failure");
      }
      std::clog << "mcp980x_i2c::mcp980x_i2c: Device address=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)mcp980x_i2c::DeviceId << " - fd=" << std::dec << _fd << std::endl;
      
      // TODO Declare wiringBbb node
      
      _alert = p_alert;
      if(_alert != 0xff) { // INTRA pin is not used
	::pinMode(_alert, INPUT);
      }
      
    } // End of Constructor
    
    mcp980x_i2c::~mcp980x_i2c() {
      if (_fd != -1) {
	close(_fd);
      }
    } // End of Destructor

    const float mcp980x_i2c::readTemp() const {
      // Sanity check
      if (_fd == -1) {
	return FLT_MIN;
      }
      
      // Set Pointer register and read 16 bits data
      unsigned short code = ::wiringBbbI2CReadReg16(_fd, mcp980x_i2c::TA);
      code = converter::getInstance().swap(code); // FIXME Specific to BBB, to be move into wiringBbbI2CReadReg16
      //      std::clog << "mcp980x_i2c::readTemp: temp=0x" << std::hex << std::setw(4) << std::setfill('0') << code << std::endl;
      return convertBytes2Temp(code);
    } // End of method readTemp

    int mcp980x_i2c::setHysteresisTemp(const int p_hysteresisTemp) {
      return ::wiringBbbI2CWriteReg16(_fd, mcp980x_i2c::THYST, convertTemp2Bytes(p_hysteresisTemp));
    } // End of method setHysteresisTemp
    
    int mcp980x_i2c::setAlertTemp(const int p_alertTemp) {
      return ::wiringBbbI2CWriteReg16(_fd, mcp980x_i2c::TSET, convertTemp2Bytes(p_alertTemp));
    } // End of method setAlertTemp
    
    int mcp980x_i2c::setAlertPolarity(const bool p_activeHigh) {
      short config = ::wiringBbbI2CReadReg8(_fd, mcp980x_i2c::CONFIG);
      if (p_activeHigh) { // Set bit 4
	config |= 0x04;
      } else { // Unset bit 4
	config &= ~0x04;
      }
      
      return ::wiringBbbI2CWriteReg8(_fd, mcp980x_i2c::CONFIG, config);
    } // End of method setAlertPolarity
    
    const float mcp980x_i2c::convertBytes2Temp(const unsigned short p_temp) const {
      unsigned short flag = 0x4000; // Bit16 = sign bit
      float result = .0;
      for (int i = 6; i >= 0; i--) {
        if ((p_temp & flag) != 0) {
          result += (1 << i) ;
        }
        flag >>= 1;
      } // End of 'for' statement
      // TODO Check the resolution
      for (int i = 1; i < 5; i++) {
        if ((p_temp & flag) != 0) {
          result += 1 / (1 << i) ;
        }
        flag >>= 1;
      } // End of 'for' statement
      
      if ((p_temp & 0x8000) != 0) {
        result *= -1.0;
      }
      
      return result;
    }
    
    const unsigned short mcp980x_i2c::convertTemp2Bytes(const unsigned short p_temp) const {
      // Read config
      short config = ::wiringBbbI2CReadReg8(_fd, mcp980x_i2c::CONFIG);
      //      std::clog << "mcp980x_i2c::convertTemp2Bytes: config=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)config << std::endl;
      // Check resolution
      AdcResolution adcResolution = static_cast<AdcResolution>(((unsigned char)(config >> 5)));
      //      std::clog << "mcp980x_i2c::convertTemp2Bytes: adcResolution=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)adcResolution << std::endl;
      // Make conversion
      int val = p_temp;
      unsigned short alertTemp = static_cast<unsigned short>((val < 0) ? 0x8000 /*Set sign bit to*/: 0x0000);
      if ((val / 64) != 0)  alertTemp |= 0x4000;
      val %= 64;
      if ((val / 32) != 0)  alertTemp |= 0x2000;
      val %= 32;
      if ((val / 16) != 0)  alertTemp |= 0x1000;
      val %= 16;
      if ((val / 8) != 0)  alertTemp |= 0x0080;
      val %= 8;
      if ((val / 4) != 0)  alertTemp |= 0x0040;
      val %= 4;
      if ((val / 2) != 0)  alertTemp |= 0x0020;
      val %= 2;
      if (val != 0) alertTemp |= 0x0010;

      //      std::clog << "<<< mcp980x_i2c::convertTemp2Bytes: " << std::hex << std::setw(4) << std::setfill('0') << (unsigned short)alertTemp << std::endl;
      return alertTemp;
    } // End of method convertTemp2Bytes

  } // End of namespace i2c

} // End of namespace beagleboneHw

