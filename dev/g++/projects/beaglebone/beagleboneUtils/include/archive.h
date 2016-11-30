/* Beaglebone data ONLY serialization description
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
#if !defined(__ARCHIVE_H__)
#define __ARCHIVE_H__
 
#include <vector>
 
#include "converter.h"
 
namespace beagleboneUtils {
 
  namespace serialization {
 
    class archive {
      std::vector<unsigned char> _data;
 
    public:
      enum arType : unsigned char{
	arBoolean   = 0x00,
	arShort     = 0x01,
	arInteger   = 0x02,
	arFloat     = 0x03,
	arTime_t    = 0x04,
	arLong      = 0x05,
	arCharacter = 0x06
      };
 
      inline void deserialize_(std::vector<unsigned char> & p_data, std::vector<unsigned char> & p_value) {
	// p_data[0]: Length of the value
	if (p_data[0] == 0x01) {
	  p_value.assign(1, 0);
	  p_value[0] = p_data[1];
	  p_data.erase(p_data.begin(), p_data.begin() + 2);
	} else {
	  // Extract the length of the value
	  std::vector<unsigned char> vlength(p_data[0]);
	  p_value.assign(p_data.begin() + 1, p_data.begin() + p_data[0] + 1); // Extract the bytes
	  p_data.erase(p_data.begin(), p_data.begin() + p_data[0] + 1); // Update p_data data
	}
      };
 
    public:
    archive() : _data() {};
      virtual ~archive() {};
 
      // TODO Check of to use template instead of polymorphism: template <typename T> void serialize(const arType p_type, const T p_value);
      inline void serialize(const bool p_value) {
	_data.push_back(getTypeSize(archive::arBoolean));
	_data.push_back((unsigned char)p_value);
      };
      inline void deserialize(std::vector<unsigned char> & p_data, bool *p_value) {
	if (p_data[0] == getTypeSize(archive::arBoolean)) {
	  *p_value = static_cast<bool>(p_data[1]);
	  p_data.erase(p_data.begin(), p_data.begin() + p_data[0] + 1);
	}
      };
 
      inline void serialize(const int p_value) {
	_data.push_back(getTypeSize(archive::arInteger));
	std::vector<unsigned char> value = converter::getInstance().int2bytes(p_value);
	_data.insert(_data.begin() + _data.size(), value.cbegin(), value.cend());
      };
      inline void deserialize(std::vector<unsigned char> & p_data, int *p_value) {
	if (p_data[0] == getTypeSize(archive::arInteger)) {
	  std::vector<unsigned char> value;
	  deserialize_(p_data, value);
	  *p_value = converter::getInstance().bytes2int(value);
	}
      };
 
      inline void serialize(const float p_value) {
	_data.push_back(getTypeSize(archive::arFloat));
	std::vector<unsigned char> value = converter::getInstance().float2bytes(p_value);
	_data.insert(_data.begin() + _data.size(), value.cbegin(), value.cend());
      };
      inline void deserialize(std::vector<unsigned char> & p_data, float *p_value) {
	if (p_data[0] == getTypeSize(archive::arFloat)) {
	  std::vector<unsigned char> value;
	  deserialize_(p_data, value);
	  *p_value = converter::getInstance().bytes2float(value);
	}
      };
 
      /*      inline void serialize(const time_t p_value) {
	_data.push_back(getTypeSize(archive::arTime_t));
	std::vector<unsigned char> value = converter::getInstance().long2bytes(p_value);
	_data.insert(_data.begin() + _data.size(), value.cbegin(), value.cend());
      };
      inline void deserialize(std::vector<unsigned char> & p_data, time_t *p_value) {
	if (p_data[0] == getTypeSize(archive::arTime_t)) {
	  std::vector<unsigned char> value;
	  deserialize_(p_data, value);
	  *p_value = converter::getInstance().bytes2long(value);
	}
	};*/
 
      inline void serialize(const long p_value) {
	_data.push_back(getTypeSize(archive::arTime_t));
	std::vector<unsigned char> value = converter::getInstance().long2bytes(p_value);
	_data.insert(_data.begin() + _data.size(), value.cbegin(), value.cend());
      };
      inline void deserialize(std::vector<unsigned char> & p_data, long *p_value) {
	if (p_data[0] == getTypeSize(archive::arTime_t)) {
	  std::vector<unsigned char> value;
	  deserialize_(p_data, value);
	  *p_value = converter::getInstance().bytes2long(value);
	}
      };
 
      inline void serialize(const char p_value) {
	serialize((const unsigned char)p_value);
      };
      inline void deserialize(std::vector<unsigned char> & p_data, char *p_value) {
	if (p_data[0] == getTypeSize(archive::arCharacter)) {
	  *p_value = static_cast<char>(p_data[1]);
	  p_data.erase(p_data.begin(), p_data.begin() + p_data[0] + 1);
      }    
      };
       
      inline void serialize(const unsigned char p_value) {
	_data.push_back(getTypeSize(archive::arCharacter));
	_data.push_back((unsigned char)p_value);
      };
       
    inline void serialize(const std::string & p_value) {
      serialize((int)p_value.size());
      _data.insert(_data.begin() + _data.size(), p_value.cbegin(), p_value.cend());      
    };
    inline void deserialize(std::vector<unsigned char> & p_data, std::string & p_value) {
      int size;
      deserialize(p_data, (int *)&size); // Length of the frame
      p_value.assign(p_data.begin(), p_data.begin() + size);
      p_data.erase(p_data.begin(), p_data.begin() + size);      
    };
       
    inline void serialize(const std::vector<unsigned char> & p_value) {
      serialize((int)p_value.size());
      _data.insert(_data.begin() + _data.size(), p_value.cbegin(), p_value.cend());
    };
    inline void deserialize(std::vector<unsigned char> & p_data, std::vector<unsigned char> & p_value) {
      int size;
      deserialize(p_data, (int *)&size); // Length of the frame
      p_value.assign(p_data.begin(), p_data.begin() + size);
      p_data.erase(p_data.begin(), p_data.begin() + size);      
    };
 
    inline void clear() { _data.clear(); };
 
    inline const std::vector<unsigned char> & toBytes() const { return _data; };
 
  private:
    int getTypeSize(const archive::arType p_type);
 
  }; // End of class archive
 
} // End of namespace serialization
 
} // End of namespace beagleboneUtils
 
using namespace beagleboneUtils::serialization;
 
#endif // __ARCHIVE_H__
