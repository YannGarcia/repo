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
#if !defined(__CONVERTER_H__)
#define __CONVERTER_H__

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

namespace beagleboneUtils {

  /** This class provides a set of convertion methods. V0.0.0.1
   *
   * \author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
   */
  class converter {
    static converter * Instance;

    converter() {};
    ~converter() { if (Instance != NULL) { delete Instance; Instance = NULL; } };

  public:
    inline static converter & getInstance() { if (Instance == NULL) Instance = new converter(); return *Instance; };

  public:
    inline unsigned char bcd2bin(const unsigned char p_bcdValue) { return ((p_bcdValue / 16 * 10) + (p_bcdValue % 16)); };
    inline unsigned char bin2bcd(const unsigned char p_binValue) { return ((p_binValue / 10 * 16) + (p_binValue % 10)); };

  public:
    unsigned short swap(const unsigned short p_value);
    inline short swap(const short p_value) { return static_cast<short>(swap(static_cast<unsigned short>(p_value))); };
    unsigned int swap(const unsigned int p_value);
    inline int swap(const int p_value) { return static_cast<int>(swap(static_cast<unsigned int>(p_value))); };

  public:
    std::string string2hexa(const std::string & p_string);
    std::string hexa2string(const std::string & p_hexa);

  public:
    inline std::vector<unsigned char> int2bytes(const int p_int) const {
      unsigned char bytes[sizeof(p_int)];
      std::copy(
		static_cast<const unsigned char *>(static_cast<const void *>(&p_int)),
		static_cast<const unsigned char *>(static_cast<const void *>(&p_int)) + sizeof(p_int),
		bytes
		);
      std::vector<unsigned char> result(bytes, bytes + sizeof(bytes) / sizeof(unsigned char));
      return result;
    }; // End of int2bytes
    inline int bytes2int(const std::vector<unsigned char> & p_bytes) const {
      return *((int *)p_bytes.data());
    }; // End of bytes2int 

    inline std::vector<unsigned char> long2bytes(const long p_long) const {
      unsigned char bytes[sizeof(p_long)];
      std::copy(
		static_cast<const unsigned char *>(static_cast<const void *>(&p_long)),
		static_cast<const unsigned char *>(static_cast<const void *>(&p_long)) + sizeof(p_long),
		bytes
		);
      std::vector<unsigned char> result(bytes, bytes + sizeof(bytes) / sizeof(unsigned char));
      return result;
    }; // End of long2bytes
    inline int bytes2long(const std::vector<unsigned char> & p_bytes) const {
      return *((long *)p_bytes.data());
    }; // End of bytes2long 

    inline std::vector<unsigned char> float2bytes(const float p_float) const {
      unsigned char bytes[sizeof(p_float)];
      std::copy(
		static_cast<const unsigned char *>(static_cast<const void *>(&p_float)),
		static_cast<const unsigned char *>(static_cast<const void *>(&p_float)) + sizeof(p_float),
		bytes
		);
      std::vector<unsigned char> result(bytes, bytes + sizeof(bytes) / sizeof(unsigned char));
      return result;
    }; // End of float2bytes
    inline float bytes2float(const std::vector<unsigned char> & p_bytes) const {
      return *((float *)p_bytes.data());
    }; // End of bytes2float

    inline std::vector<unsigned char> string2bytes(const std::string & p_string) const {
      return std::vector<unsigned char>(p_string.begin(), p_string.end());
    }; // End of string2bytes
    inline std::string bytes2string(const std::vector<unsigned char> & p_bytes) const {
      return std::string(p_bytes.begin(), p_bytes.end());
    }; // End of bytes2string

    inline std::string trim(const std::string & p_string) {
      std::string::const_iterator wsfront = std::find_if_not(p_string.begin(), p_string.end(), [](int c) { return std::isspace(c); } );
      std::string::const_iterator wsback = std::find_if_not(p_string.rbegin(), p_string.rend(), [](int c) { return std::isspace(c); } ).base();
      return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
    }

    inline std::vector<std::string> split_arguments_line(const std::string & p_string) {
      std::vector<std::string> output;
      std::string line = trim(p_string);
      if (!line.empty() &&  (line[0] == '-')) { // Valid command line
	size_t current = 0;
	size_t next = -1;
	size_t pos = 0;
	do {
	  if (line[pos + 1] == '-') { // --
	    current = pos + 2;
	  } else {
	    current = pos + 1;
	  }
	  next = line.find("-", current);
	  output.push_back(line.substr(pos, next - pos));
	  pos = next;
	} while (next != std::string::npos);
      } // else, invalid command line
      return output;
    }
    
  }; // End of class converter

} // End of namespace beagleboneUtils

using namespace beagleboneUtils;

#endif // __CONVERTER_H__
