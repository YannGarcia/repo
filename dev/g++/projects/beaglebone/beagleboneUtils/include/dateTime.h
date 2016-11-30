/* Beaglebone date/time description
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
#if !defined(__DATETIME_H__)
#define __DATETIME_H__

#include <iostream>
#include <chrono>
#include <ctime>

namespace beagleboneUtils {

  /** This class provides a set of date/time methods. V0.0.0.1
   *
   * @author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
   */
  class dateTime {

    time_t _time;

  public:
    dateTime() : _time(time(NULL)) { };
    dateTime(const time_t p_time) : _time(p_time) { };
    virtual ~dateTime() { };

    /**
     * @desc Convert provided GPS UTC date/time int UTC time.
     * @param p_gpsDateTime GPS UTC date/time (ddmmyy hhmmssdd)
     * @return The Unix UTC time on success, 0 otherwise
     */
    const time_t gps2utc(const std::string & p_gpsDateTime) const;
    inline const time_t utc2local(const time_t p_time) const {
      struct tm * tm_ = localtime(&p_time);
      return mktime(tm_);
    };
    
    virtual const std::string & toString() const;

  }; // End of class dateTime

} // End of namespace beagleboneUtils

using namespace beagleboneUtils;

#endif // __DATETIME_H__

