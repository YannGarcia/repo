/* Beaglebone ThingSpeak wrapper description
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

#include <iostream>
#include <sstream>
#include <sstream> // std::ostringstream

#include "thingspeakImpl.h"

namespace beagleboneThingspeak {

  int thingspeakImpl::setField(const unsigned char p_fieldId, const float p_value) {
    // Sanity checks
    if (p_fieldId == 0x00) {
      return -1;
    }
    std::ostringstream os;
    os << std::setprecision(4) << p_value;
    _map.insert(std::pair<unsigned char, std::string>(p_fieldId, os.str()));
    return 0;
  }
  
  int thingspeakImpl::pushData() {
    // TODO Sanity checks

    // Prepare URL + data
    std::ostringstream url;
    url << _url << "?key=" << _apiKey;
    for (std::map<unsigned char, std::string>::iterator it = _map.begin(); it != _map.end(); ++it) {
      url << "&field" << static_cast<unsigned int>(it->first) << "=" << it->second;
    } // End of 'for' statement
    // Send URL
    std::string response;
    _http.get(url.str(), response);
    // Clear map content
    _map.clear();

    return 0;
  }

} // End of namespace beagleboneThingspeak

