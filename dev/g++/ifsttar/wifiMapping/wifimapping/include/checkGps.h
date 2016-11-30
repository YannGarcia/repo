/* Raspberry check GPS result description
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
#if !defined(__CHECKGPS_H__)
#define __CHECKGPS_H__

#include "checkIface.h"

#include "ipcCommon.h"

namespace wifimapping {

  class checkGps : public checkIface {
    std::vector<std::string> _values;

  public:
    checkGps();
    virtual ~checkGps();
    
    virtual int initialize();
    virtual int uninitialize();
    virtual void update();
    virtual void serialize(archive & p_archive);

    inline const std::vector<std::string> & getValues() const { return _values; };
    inline std::string toString() const {
      std::ostringstream os;
      
      if (_values.size() != 0) {  
	os << "latitude: " << _values[IDX_LATITUDE] << " - longitude: " << _values[IDX_LONGITUDE] << std::endl;
      } else {
	os << "No values available" << std::endl;
      }
      
      return os.str();
    } // End of method toString
  
  }; // End of class checkGps
  
} // End of namespace wifimapping

using namespace wifimapping;

#endif // __CHECKGPS_H__

