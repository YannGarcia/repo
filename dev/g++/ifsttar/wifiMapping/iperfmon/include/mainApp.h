/* Beaglebone iperf-3.0 monitor implementation
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

#include "runnable.h" // Thread implementation

#include "iperfImpl.h"
#include "smMgr.h" // Shared memory manager
#include "ipcCommon.h" // IPC constants 

namespace iperfmon {

  class mainApp : public runnable {
    smMgr _smMgr;
    smMgr _parmMgr;
    iperfImpl _iperfImpl;
    /**
     Sample period in millisecond.
     This value is dependant of the vehicle speed. It's set by the main process (wifimapping) according to the GPS information
    */
    float _samplePeriod;

  public:
    mainApp(const std::string & p_client, const std::string & p_host, const int p_port, const int p_duration);
    virtual ~mainApp();
    
    void stop();
    void run();
  }; // End of class mainApp
  
} // End of namespace iperfmon

using namespace iperfmon;

#endif // __MAIN_APP_H__
