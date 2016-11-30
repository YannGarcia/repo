/* Beaglebone THINGSPEAK wrapper description
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
#if !defined(__THINGSPEAKIMPL_H__)
#define __THINGSPEAKIMPL_H__

#include <iomanip> // std::setprecision
#include <string>
#include <map>

#include "httpClient.h"

namespace beagleboneThingspeak {

  /** This class provides a wrapper to the licurl. V0.0.0.1
   *
   * @author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
   */
  class thingspeakImpl {
    std::string _apiKey;
    std::map<unsigned char, std::string> _map;
    network::httpClient _http;
    std::string _url;

  public:
  thingspeakImpl(const std::string & p_apiKey) : 
    _apiKey(p_apiKey), 
    _map(), 
    _http(),
    _url("http://api.thingspeak.com/update") { };
    ~thingspeakImpl() { };

    int setField(const unsigned char p_fieldId, const float p_value);
    int pushData();
    
  }; // End of class thingspeakImpl

} // End of namespace beagleboneThingspeak

using namespace beagleboneThingspeak;

#endif // __THINGSPEAKIMPL_H__

