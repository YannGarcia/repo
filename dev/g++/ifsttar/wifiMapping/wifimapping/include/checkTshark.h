/* Beaglebone check Tshark description
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
#if !defined(__CHECKTSHARK_H__)
#define __CHECKTSHARK_H__

#include <map>

#include "checkIface.h"

namespace wifimapping {

  class checkTshark : public checkIface {
    std::map<std::string, std::vector<std::string> > _values;

  public:
    checkTshark();
    virtual ~checkTshark();
    
    virtual int initialize();
    virtual int uninitialize();
    virtual void update();
    virtual void serialize(archive & p_archive);

    inline const std::map<std::string, std::vector<std::string>> & getValues() const { return _values; };
    inline std::string & toString() const {
      static std::string str;
      
      std::ostringstream os;
      unsigned int counter = 0;
      for (std::map<std::string, std::vector<std::string> >::const_iterator it = _values.begin(); it != _values.end(); ++it) {
	os << "bssid#" << counter++ << "=" << it->first << ",";
	//      os << "  rssid=" << it->second[0/*IDX_RSSID*/] << ",";
	//      os << "  essid=" << it->second[3/*IDX_ESSID*/];
      } // End of 'for' statement
      os << std::endl;
      str = os.str();
      
      return str;
    };
    
  }; // End of class checkTshark
  
} // End of namespace wifimapping

using namespace wifimapping;

#endif // __CHECKTSHARK_H__
