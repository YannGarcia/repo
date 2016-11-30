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

#include "dateTime.h"

namespace beagleboneUtils {

  const time_t dateTime::gps2utc(const std::string & p_gpsDateTime) const {
    // Sanity check
    if (p_gpsDateTime.length() != 15) {
      std::cerr << "dateTime::gps2utc: Wrong parameter length" << std::endl;
      return 0;
    }
    size_t sep = p_gpsDateTime.find(" ");
    if (sep == std::string::npos) {
      std::cerr << "dateTime::gps2utc: Wrong parameter format" << std::endl;
      return 0;
    }

    // Extract date and time ddmmyy hhmmssdd
    unsigned long date = static_cast<unsigned long>(std::stol(p_gpsDateTime.substr(0, sep).c_str()));
    unsigned long time = static_cast<unsigned long>(std::stol(p_gpsDateTime.substr(sep + 1).c_str()));
    std::clog << "dateTime::gps2utc: date=" << std::dec << date << " - time=" << time << std::endl;
    struct tm tm_ = { 0 };
    tm_.tm_year = date % 100;
    tm_.tm_year += tm_.tm_year > 80 ? 1900 : 2000;
    tm_.tm_mon = (date / 100) % 100;
    tm_.tm_wday = 0;
    tm_.tm_yday = date / 10000;
    tm_.tm_hour = time / 1000000;
    tm_.tm_min = (time / 10000) % 100;
    tm_.tm_sec = (time / 100) % 100;
    std::clog << "dateTime::gps2utc: " << std::dec << tm_.tm_year << " " << tm_.tm_mon << " " << tm_.tm_yday << " " << tm_.tm_hour << " " << tm_.tm_min << " " << tm_.tm_sec << std::endl;
    //    hundredths = time % 100;

    return mktime(&tm_);
  }
  
  const std::string & dateTime::toString() const {
    static std::string str;
    str = std::ctime(&_time);
    return str;
  }
  
} // End of namespace beagleboneUtils

