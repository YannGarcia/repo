/**
 * @File      converter.h
 * @brief     Header file for the types converter library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>

#include <string>
#include <vector>
#include <algorithm>

#include <cstdint>
#include <cctype>
#include <climits> // LONG_MAX, LLONG_MAX
#include <ctime>   // time_t, struct tm, difftime, time, mktime

namespace helpers {

  /**
   * @class converter
   * @brief This class provide a set of metthods for types convertions
   * @remark Singleton pattern
   */
  class converter {
    
    /**
     * @brief Unique static object reference of this class
     */
    static converter * instance;
    
    /**
     * @brief Default private ctor
     */
    converter() {};
    /**
     * @brief Default private dtor
     */
    ~converter() {
      if (instance != NULL) {
	delete instance;
	instance = NULL;
      }
    };
    
  public:
    /**
     * @brief Public accessor to the single object reference
     */
    inline static converter & get_instance() {
      if (instance == NULL) instance = new converter();
      return *instance;
    };
    
  public:
    /**
     * @enum endian_t
     * @brief Endianess style
     */
    typedef enum {
      big_endian,
      little_endian
    } endian_t;

  public:
    /**
     * @desc Convert a Binary Coded Decimal value into a binary value
     * @param[in] p_value The BDC value
     * @return The binary value
     */
    inline uint8_t bcd_to_bin(const uint8_t p_value) {
      return ((p_value / 16 * 10) + (p_value % 16));
    };

    /**
     * @desc Convert a binary value into a Binary Coded Decimal value
     * @param[in] p_value The binary value
     * @return The BCD value
     */
    inline uint8_t bin_to_bcd(const uint8_t p_value) {
      return ((p_value / 10 * 16) + (p_value % 10));
    };
    
    /**
     * @desc Swap two bytes length value (e.g. 0xCAFE becomes 0xFECA)
     * @param[in] p_value The value to swap
     * @return The swapped value
     */
    uint16_t swap(const uint16_t p_value);
    inline int16_t swap(const int16_t p_value) {
      return static_cast<short>(swap(static_cast<uint16_t>(p_value)));
    };
    /**
     * @desc Swap four bytes length value (used for littel endian / big endian)
     * @param[in] p_value The value to swap
     * @return The swapped value
     */
    uint32_t swap(const uint32_t p_value);
    inline int32_t swap(const int32_t p_value) {
      return static_cast<int>(swap(static_cast<uint32_t>(p_value)));
    };
    
    /**
     * @desc Convert a string into an hexadecimal string
     * @param[in] p_value The string value
     * @return The hexadecimal value
     */
    std::string string_to_hexa(const std::string & p_value);
    /**
     * @desc Convert a bytes array int32_t an hexadecimal string
     * @param[in] p_value The bytes array value
     * @return The hexadecimal value
     */
    std::string bytes_to_hexa(const std::vector<uint8_t> & p_value);
    /**
     * @desc Convert an hexadecimal string into a bytes array
     * @param[in] p_value The hexadecimal value
     * @return The bytes array value
     */
    std::vector<uint8_t> hexa_to_bytes(const std::string & p_value);
    
    /**
     * @desc Convert a time in time_t format into a string formated according to RFC 822, 1036, 1123, 2822
     * @param[in] p_time The time to convert in time_t format
     * @return The time string formated
     * @see http://www.unixtimestamp.com/
     * @code
     * std::string result = time_to_string(1489755780);
     * result.compare("Fri, 17 Mar 2017 14:03:00 +0100") == 0 //when date/time location is Paris (UTC + 1)
     * @endcode
     */
    std::string time_to_string(const time_t p_time);
    /**
     * @desc Convert a time in struct tm format into a string formated according to RFC 822, 1036, 1123, 2822
     * @param[in] p_time The time to convert in struct tm format
     * @return The time string formated
     * @see http://www.unixtimestamp.com/
     */
    std::string time_to_string(const struct tm & p_time);
    
    /**
     * @desc Convert a 16-bits integer (int16_t) into a bytes array
     * @param[in] p_value The 16-bits integer value
     * @param[in] p_endianess Endianess style. Default: big_endian
     * @return The bytes array value
     */
    inline std::vector<uint8_t> short_to_bytes(const int16_t p_value, const endian_t p_endianess = big_endian) const {
      std::vector<uint8_t> result(sizeof(short), 0x00);
      for (int i = sizeof(short) - 1; i >= 0; i--) {
	int offset = (sizeof(short) - 1 - i) * 8;
	result[i] = static_cast<uint8_t>((p_value >> offset) & 0xFF);
      } // End of 'for' statement
      return result;
    }; // End of short_to_bytes

    /**
     * @desc Convert a bytes array into a 16-bits integer (int16_t)
     * @param[in] p_value The bytes array
     * @param[in] p_endianess Endianess style. Default: big_endian
     * @return The 16-bits integer on success, SHRT_MAX on error (wrong bytes array size)
     */
    inline int16_t bytes_to_short(const std::vector<uint8_t> & p_value, const endian_t p_endianess = big_endian) const {
      // Sanity check
      if (p_value.size() > sizeof(short)) {
	return SHRT_MAX;
      }
      int16_t value = 0;
      for (size_t i = 0; i < p_value.size(); i++) {
	value = (value << 8) + (p_value[i] & 0xff);
      } // End of 'for' statement
      return value;
    }; // End of bytes_to_short
    
    /**
     * @desc Convert a 32-bits integer (int32_t) into a bytes array
     * @param[in] p_value The 32-bits integer value
     * @param[in] p_endianess Endianess style. Default: big_endian
     * @return The bytes array value
     */
    inline std::vector<uint8_t> int_to_bytes(const int32_t p_value, const endian_t p_endianess = big_endian) const {
      /*uint8_t bytes[sizeof(p_value)];
      std::copy(
                static_cast<const uint8_t *>(static_cast<const void *>(&p_value)),
                static_cast<const uint8_t *>(static_cast<const void *>(&p_value)) + sizeof(p_value),
                bytes
		);
		std::vector<uint8_t> result(bytes, bytes + sizeof(bytes) / sizeof(uint8_t));*/
      std::vector<uint8_t> result(sizeof(int), 0x00);
      for (int i = sizeof(int) - 1; i >= 0; i--) {
	int offset = (sizeof(int) - 1 - i) * 8;
	result[i] = static_cast<uint8_t>((p_value >> offset) & 0xFF);
      } // End of 'for' statement
      return result;
    }; // End of int_to_bytes

    /**
     * @desc Convert a bytes array into a 32-bits integer (int32_t)
     * @param[in] p_value The bytes array
     * @param[in] p_endianess Endianess style. Default: big_endian
     * @return The 32-bits integer on success, LONG_MAX on error (wrong bytes array size)
     */
    inline int32_t bytes_to_int(const std::vector<uint8_t> & p_value, const endian_t p_endianess = big_endian) const {
      // Sanity check
      if (p_value.size() > sizeof(int)) {
	return INT_MAX;
      }
      int32_t value = 0;
      for (size_t i = 0; i < p_value.size(); i++) {
	value = (value << 8) + (p_value[i] & 0xff);
      } // End of 'for' statement
      return value;
      //      return *((int *)(&p_value[0]));
    }; // End of bytes_to_int
    
    /**
     * @desc Convert a 64-bits integer (int64_t) into a bytes array
     * @param[in] p_value The 64-bits integer value
     * @param[in] p_endianess Endianess style. Default: big_endian
     * @return The bytes array value
     */
    inline std::vector<uint8_t> long_to_bytes(const int64_t p_value, const endian_t p_endianess = big_endian) const {
      /*uint8_t bytes[sizeof(p_value)];
      std::copy(
                static_cast<const uint8_t *>(static_cast<const void *>(&p_value)),
                static_cast<const uint8_t *>(static_cast<const void *>(&p_value)) + sizeof(p_value),
                bytes
		);
		std::vector<uint8_t> result(bytes, bytes + sizeof(bytes) / sizeof(uint8_t));*/
      std::vector<uint8_t> result(sizeof(int64_t), 0x00);
      for (int i = sizeof(int64_t) - 1; i >= 0; i--) {
	int offset = (sizeof(int64_t) - 1 - i) * 8;
	result[i] = static_cast<uint8_t>((p_value >> offset) & 0xFF);
      } // End of 'for' statement
      return result;
    }; // End of long_to_bytes

    /**
     * @desc Convert a bytes array into a 64-bits integer (int64_t)
     * @param[in] p_value The bytes array
     * @param[in] p_endianess Endianess style. Default: big_endian
     * @return The 64-bits integer on success, LLONG_MAX on error (wrong bytes array size)
     */
    inline int64_t bytes_to_long(const std::vector<uint8_t> & p_value, const endian_t p_endianess = big_endian) const {
      // Sanity check
      if (p_value.size() > sizeof(int64_t)) {
	return LLONG_MAX;
      }
      int64_t value = 0;
      for (size_t i = 0; i < p_value.size(); i++) {
	value = (value << 8) + (p_value[i] & 0xff);
      } // End of 'for' statement
      return value;
      //      return *((long *)(&p_value[0]));
    }; // End of bytes_to_long
    
    /**
     * @desc Convert a float value into a bytes array
     * @param[in] p_value The float value
     * @return The bytes array value
     */
    inline std::vector<uint8_t> float_to_bytes(const float p_value) const {
      uint8_t bytes[sizeof(p_value)];
      std::copy(
                static_cast<const uint8_t *>(static_cast<const void *>(&p_value)),
                static_cast<const uint8_t *>(static_cast<const void *>(&p_value)) + sizeof(p_value),
                bytes
		);
      std::vector<uint8_t> result(bytes, bytes + sizeof(bytes) / sizeof(uint8_t));
      return result;
    }; // End of float_to_long

    /**
     * @desc Convert a bytes array into a float
     * @param[in] p_value The bytes array
     * @return The float value
     */
    inline float bytes_to_float(const std::vector<uint8_t> & p_value) const {
      return *((float *)(&p_value[0]));
    }; // End of bytes_to_float
    
    /**
     * @desc Convert a string into a bytes array
     * @param[in] p_value The string value
     * @return The bytes array value
     */
    inline std::vector<uint8_t> string_to_bytes(const std::string & p_value) const {
      return std::vector<uint8_t>(p_value.begin(), p_value.end());
    }; // End of string_to_bytes
    
    /**
     * @desc Convert a bytes array into a string
     * @param[in] p_value The bytes array value
     * @return The string value
     */
    inline std::string bytes_to_string(const std::vector<uint8_t> & p_value) const {
      return std::string(p_value.begin(), p_value.end());
    }; // End of bytes_to_string
    
  public:
    /**
     * @desc Convert a string into an integer
     * @param[in] p_value The string value
     * @return The integer value
     */
    inline int32_t string_to_int(const std::string & p_value) const {
      return std::stoi(p_value);
      //return atoi(p_value.c_str());
    }; // End of string_to_int

    /**
     * @desc Convert an integer into a string
     * @param[in] p_value The integer value
     * @return The string value
     */
    inline std::string int_to_string(const int32_t & p_value) const {
      std::ostringstream ss;
      ss << p_value;
      return ss.str();
    }; // End of string_to_bytes

  public:
        
    /**
     * @desc Returns a copy of the string, with leading and trailing special characters omitted
     * @param[in] p_value The string value
     * @param[in] p_trim_chars The special characters to be omitted. Default: ' ' and TAB
     * @return The new string value
     */
    std::string trim(const std::string& p_value, const std::string& p_trim_chars = " \t");
    
    /**
     * @desc Convert the provided string into a list of arguments
     * @param[in] p_value The string value
     * @return The arguments list
     * @code{.cc}
     *     std::string str = "--host localhost --port 12345 --duration -1";
     *     std::vector<std::string> tokens = converter::get_instance().split_arguments_line(str);
     *     std::clog << "Tokens: " << std::endl;
     *     for (auto it = tokens.begin(); it != tokens.end(); ++it) {
     *       std::clog << "   " << *it << std::endl;
     *     }
     * @endcode
     */
    std::vector<std::string> split_arguments_line(const std::string & p_value);
                  
  }; // End of class converter

} // End of namespace helpers

using namespace helpers;
