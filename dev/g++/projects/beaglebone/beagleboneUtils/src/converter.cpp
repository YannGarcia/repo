/* Beaglebone converstion tools
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
#include <algorithm>
#include <stdexcept>

#include "converter.h"

namespace beagleboneUtils {

  converter * converter::Instance = NULL;

  unsigned short converter::swap(const unsigned short p_value) {
    unsigned char *ptr = (unsigned char *)&p_value;
    return (ptr[0] << 8) | ptr[1];
  }

  unsigned int converter::swap(const unsigned int p_value) {
    unsigned char *ptr = (unsigned char *)&p_value;
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
  }

  std::string converter::string2hexa(const std::string & p_string) {
    static const char * const lut = "0123456789ABCDEF";
    size_t length = p_string.length();

    std::string output;
    output.reserve(2 * length);
    for (size_t i = 0; i < length; ++i) {
      const unsigned char c = p_string[i];
      output.push_back(lut[c >> 4]);
      output.push_back(lut[c & 15]);
    } // End of 'for' statement

    return output;
  }

  std::string converter::hexa2string(const std::string & p_hexa) {
    static const char * const lut = "0123456789ABCDEF";
    size_t length = p_hexa.length();
    if (length & 1) {
      throw std::invalid_argument("odd length");
    }

    std::string output;
    output.reserve(length / 2);
    for (size_t i = 0; i < length; i += 2) {
      char msbDigit = p_hexa[i];
      const char *msbIndex = std::lower_bound(lut, lut + 16, msbDigit);
      if (*msbIndex != msbDigit) {
	throw std::invalid_argument("not a hex digit");
      }
      char lsbDigit = p_hexa[i + 1];
      const char* lsbIndex = std::lower_bound(lut, lut + 16, lsbDigit);
      if (*lsbIndex != lsbDigit) {
	throw std::invalid_argument("not a hex digit");
      }
      output.push_back(((msbIndex - lut) << 4) | (lsbIndex - lut));
    } // End of 'for' statement

    return output;
  }

} // End of namespace beagleboneUtils
