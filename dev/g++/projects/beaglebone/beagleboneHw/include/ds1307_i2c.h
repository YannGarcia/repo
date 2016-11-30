/* Raspberry simplified access to Maxim DS1307 RTC + Memory device (I2C)
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
#if !defined(__DS1307_I2C_H__)
#define __DS1307_I2C_H__

#include "rtcc_i2c.h"

namespace beagleboneHw {

  namespace i2c {
    
    class ds1307_i2c : public rtcc_i2c {
      
    public:
      ds1307_i2c();
      ds1307_i2c(const time_t p_time);
      virtual ~ds1307_i2c();
    }; // End of class ds1307_i2c
    
  } // End of namespace i2c
  
} // End of namespace beagleboneHw 

using namespace beagleboneHw::i2c;

#endif // __DS1307_SPI_H__
