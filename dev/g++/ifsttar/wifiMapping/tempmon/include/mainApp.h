/* Beaglebone tempmon implementation
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
#if !defined(__MAIN_APP_H__)
#define __MAIN_APP_H__

//#include <iostream>

#include <memory>

#include "runnable.h" // Thread implementation

#include "smMgr.h" // Shared memory manager

#include "mcp980x_i2c.h"

namespace tempmon {

  class mainApp : public runnable {
    /** Segmented shared memory access */
    smMgr _smMgr;
    std::unique_ptr<mcp980x_i2c> _temp;
    /** PWM pin number */
    unsigned char _pwm;
    float _currentTemp;
    /** Duty cycle applied to the FAN PWM command */
    unsigned char _dutyCycle;
    /**
            Sample period in millisecond.
	         This value is dependant of the vehicle speed. It's set by the main process (wifimapping) according to the GPS information
    */
    float _samplePeriod;
    std::vector<float> _values;

  public:
    mainApp();
    virtual ~mainApp();
    
    void stop();
    void run();

  private:
    void processFan();
    void storeData();
  }; // End of class mainApp
  
} // End of namespace tempmon

using namespace tempmon;

#endif // __MAIN_APP_H__
