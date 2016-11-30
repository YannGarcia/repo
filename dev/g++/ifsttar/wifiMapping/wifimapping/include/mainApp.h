/* BeagleBone wifiMapping implementation
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

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "runnable.h" // Thread implementation

#include "mqMgr.h"
#include "ipcCommon.h" // IPC constants 

#include "processMonitor.h"

#include "checkIperf.h"
#include "checkGps.h"
#include "checkTemp.h"
#include "checkRtc.h"
#include "checkTshark.h"

namespace wifimapping {

  class mainApp : public beagleboneUtils::thread::runnable {
    smMgr _smMgr;
    smMgr _parmMgr;
    mqMgr _dsMq;
    mqMgr _guiMq;
    mqMgr _adcMq;
    /** Configuration map */
    std::map<std::string, std::string> _cfgMap;
    archive _archive;
    checkIperf _checkIperf;
    checkGps _checkGps;
    checkTemp _checkTemp;
    checkRtc _checkRtc;
    checkTshark _checkTshark;
    /** List of the monitoring processes */
    std::vector< std::unique_ptr<processMonitor> > _monitorProcess;
    /** Sample period in millisecond.
        This value is dependant of the vehicle speed. It's set by the main process (wifimapping) according to the GPS information
    */
    float _samplePeriod;
    /** Current LCD refresh timer */
    float _lcdTimeout;
    /** LCD Refresh timeout (6 seconds) */
    const float LcdRefreshTimeout = 6e6;
    bool _autoMode;

    void loadPropertiesFile(const std::string & p_cfgFile);

    inline void processGps() { _checkGps.update(); };
    inline void processIperf() { _checkIperf.update(); };
    inline void processAcc() { /*_checkAcc.update();*/ };
    inline void processTemp() { _checkTemp.update(); };
    inline void processRtc() { _checkRtc.update(); };
    inline void processTshark() { _checkTshark.update(); };
    
    void processGui();

    int storeData();

    void updateSamplePeriod();

  public:
    mainApp();
    virtual ~mainApp();

    virtual int initialize(const bool p_autoMode = true);
    virtual void stop();
    void run();
  }; // End of class mainApp
  
} // End of namespace wifimapping

using namespace wifimapping;

#endif // __MAIN_APP_H__
