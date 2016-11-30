/* Beaglebone check Iperf-3.x result description
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
#if !defined(__CHECKIPERF_H__)
#define __CHECKIPERF_H__

#include "checkIface.h"

namespace wifimapping {

  class checkIperf : public checkIface {
    std::vector<float> _values;

  public:
    checkIperf();
    virtual ~checkIperf();
    
    virtual int initialize();
    virtual int uninitialize();
    virtual void update();
    virtual void serialize(archive & p_archive);

    inline const std::vector<float> & getValues() const { return _values; };
    inline std::string toString() const {
      std::ostringstream os;
      
      if (_values.size() != 0) {  
	os << "xfert: " << _values[0] << " - Bandwidth: " << _values[1] << std::endl;
    } else {
	os << "No values available" << std::endl;
      }
      
      return os.str();
    };

  }; // End of class checkIperf
  
} // End of namespace wifimapping

using namespace wifimapping;

#endif // __CHECKIPERF_H__
