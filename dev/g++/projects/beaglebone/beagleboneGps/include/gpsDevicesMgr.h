/* Beaglebone Abstract GPS devices manager
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
#if !defined(__GPS_DEVICES_MGR_H__)
#define __GPS_DEVICES_MGR_H__

#include <memory> // Used for unique_ptr

#include "gpsDevices.h"

namespace beagleboneGps {

  class gpsDevicesMgr {
    static std::unique_ptr<gpsDevicesMgr> _instance;

    gpsDevicesMgr();

  public:
    enum Devices : unsigned char {
      ublox = 0x00
    };
    
    ~gpsDevicesMgr();

    inline static gpsDevicesMgr & getInstance() { return *gpsDevicesMgr::_instance; };

    void createGpsDevice(const gpsDevicesMgr::Devices p_device, std::unique_ptr<gpsDevices> & p_newInstance);

  }; // End of class gpsDevicesMgr

} // End of namespace beagleboneGps

using namespace beagleboneGps;

#endif // __GPS_DEVICES_MGR_H__
