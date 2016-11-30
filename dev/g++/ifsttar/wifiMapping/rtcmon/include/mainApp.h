/* Beaglebone rtcmon implementation
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

#include <string>
#include <memory> // Used std::unique_ptr

#include "runnable.h" // Thread implementation

#include "smMgr.h" // Shared memory manager
#include "ipcCommon.h" // IPC constants 

#include "mcp7941x_i2c.h"

#include "ntpClient.h"

namespace rtcmon {

  class mainApp : public beagleboneUtils::thread::runnable {
    /** Segmented shared memory access */
    smMgr _smMgr;
    std::unique_ptr<mcp7941x_i2c> _mcp7941x;
    bool enableClock;
    std::unique_ptr<network::ntpClient> _ntp;
    float _samplePeriod;

  public:
    mainApp(const std::string & p_host, const std::string & p_remote);
    virtual ~mainApp();
    
    void stop();
    void run();
  }; // End of class mainApp
  
} // End of namespace rtcmon

using namespace rtcmon;

#endif // __MAIN_APP_H__
