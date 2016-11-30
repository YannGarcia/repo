/* BeagleBone simplified access to Microchip MCP7941X RTC+ Memory device (I2C)
 * Copyright (c) 2014 ygarcia, MIT License
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

#include "mcp7941x_i2c.h"

namespace beagleboneHw {

  namespace i2c {

    unsigned char mcp7941x_i2c::Calibration    = 0x08;
    unsigned char mcp7941x_i2c::UnlockId       = 0x09;
    unsigned char mcp7941x_i2c::OffsetAlarm0   = 0x0a;
    unsigned char mcp7941x_i2c::OffsetAlarm1   = 0x11;

    mcp7941x_i2c::mcp7941x_i2c() {
      _deviceId = 0x6f; // See DS22266A-page 8 Clause 3.1.2 DEVICE ADDRESSING AND OPERATION
      _memoryBaseAddress = 0x20; // See DS22266A-page 10 TABLE 4-1: RTCC MEMORY MAP
    } // End of Constructor

    mcp7941x_i2c::mcp7941x_i2c(const time_t p_time) : mcp7941x_i2c::mcp7941x_i2c() {
      //TODO: To be continued
    } // End of Constructor

    mcp7941x_i2c::~mcp7941x_i2c() {
    } // End of Destructor

    void mcp7941x_i2c::createAlarm() {
    }

    void mcp7941x_i2c::cancelAlarm() {
    }

  } // End of namespace i2c

} // End of namespace beagleboneHw
