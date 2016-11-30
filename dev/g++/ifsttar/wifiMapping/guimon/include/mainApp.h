/* Beaglebone guimon application description
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
#if !defined(__MAIN_APP_H__)
#define __MAIN_APP_H__

#include <map>
#include <string>
#include <memory>

#include "runnable.h" // Thread implementation

#include "mqMgr.h" // Message queue manager
#include "ipcCommon.h" // IPC constants 

#include "mcp320x_spi.h"
#include "lcdManager.h"

namespace guimon {

  class mainApp : public beagleboneUtils::thread::runnable {
    mqMgr _guiMgr;
    mqMgr _adcMgr;
    std::unique_ptr<abstractLcd> _lcd;
    std::unique_ptr<mcp320x_spi> _adc;
    std::map<mcp320x_spi::Mcp320xChannels, float> _channels;
    float _samplePeriod;

  public:
    mainApp();
    virtual ~mainApp();
    
    void stop();
    void run();

  private:
    void processCommand(const std::vector<unsigned char> & p_buffer);
    void processLcdCommand(const std::string & p_cmd);
    void processAdcChannels();
  }; // End of class mainApp
  
} // End of namespace guimon

using namespace guimon;

#endif // __MAIN_APP_H__
