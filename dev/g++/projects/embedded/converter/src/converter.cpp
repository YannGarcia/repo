/**
 * @File      converter.cpp
 * @brief     Implementation file for the types converter library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <cctype> // Used for toupper

#include "converter.h"

namespace helpers {

  converter * converter::instance = NULL;
    
  uint16_t converter::swap(const uint16_t p_value) {
    uint8_t *ptr = (uint8_t *)&p_value;
    return (ptr[0] << 8) | ptr[1];
  }
    
  uint32_t converter::swap(const uint32_t p_value) {
    uint8_t *ptr = (uint8_t *)&p_value;
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
  }
    
  std::string converter::string_to_hexa(const std::string & p_value) {
    static const char * const lut = "0123456789ABCDEF";

    std::string input(p_value);
    std::for_each(
		  input.begin(), 
		  input.end(), 
		  [](char & c) { 
		    c = std::toupper(c);
		  }
		  );
        
    std::string output;
    uint32_t length = p_value.length();
    output.reserve(2 * length);
    for (uint32_t i = 0; i < length; ++i) {
      const uint8_t c = input[i];
      output.push_back(lut[c >> 4]);
      output.push_back(lut[c & 15]);
    } // End of 'for' statement
    
    return output;
  }
    
  std::string converter::bytes_to_hexa(const std::vector<uint8_t> & p_value) {
    std::ostringstream oss;
    oss << std::setfill('0');
    std::for_each(
		  p_value.begin(), 
		  p_value.end(),
		  [&oss](uint8_t ch) {
		    oss << std::hex
			<< std::setw(2)
			<< static_cast<int>(ch);
		  }
		  );
    return oss.str();
  }

  std::vector<uint8_t> converter::hexa_to_bytes(const std::string & p_value) {
    static const char * const lut = "0123456789ABCDEF";

    // Sanity check
    std::vector<uint8_t> output;
    if (p_value.length() & 1) {
      return output;
    }
    
    std::string input(p_value);
    std::for_each(
		  input.begin(), 
		  input.end(), 
		  [](char & c) { 
		    c = std::toupper(c);
		  }
		  );
        
    output.reserve(p_value.length() / 2);
    for (uint32_t i = 0; i < p_value.length(); i += 2) {
      char msbDigit = input[i];
      const char *msbIndex = std::lower_bound(lut, lut + 16, msbDigit);
      if (*msbIndex != msbDigit) {
	output.clear();
	return output;
      }
      char lsbDigit = input[i + 1];
      const char* lsbIndex = std::lower_bound(lut, lut + 16, lsbDigit);
      if (*lsbIndex != lsbDigit) {
	output.clear();
	return output;
      }
      output.push_back(((msbIndex - lut) << 4) | (lsbIndex - lut));
    } // End of 'for' statement
    
    return output;    
  }

  std::string converter::time_to_string(const time_t p_time) {
    struct tm * t = std::localtime(&p_time);
    return time_to_string(*t);
  }
    
  std::string converter::time_to_string(const struct tm & p_time) {
    char buffer[64] = { 0 };
    // Format: RFC 822, 1036, 1123, 2822
    std::strftime(buffer, 64, "%a, %d %b %Y %H:%M:%S %z", &p_time);
    return std::string(buffer);
  }
    
  std::string converter::trim(const std::string& str, const std::string& whitespace) {
    size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
      return ""; // no content
     
    size_t strEnd = str.find_last_not_of(whitespace);
    size_t strRange = strEnd - strBegin + 1;
     
    return str.substr(strBegin, strRange);
  }
    
  std::vector<std::string> converter::split_arguments_line(const std::string & p_value) {
    std::vector<std::string> output;
    std::string line = trim(p_value);
    if (!line.empty() && (line[0] == '-')) { // Valid command line
      size_t current = 0;
      size_t next = (size_t)-1;
      size_t pos = 0;
      do {
	if (line[pos + 1] == '-') { // --
	  current = pos + 2;
	} else {
	  current = pos + 1;
	}
	next = line.find("-", current);
	std::string str(line.substr(pos, next - pos));
	output.push_back(str);
	pos = next;
      } while (next != std::string::npos);
    } // else, invalid command line
    return output;
  }

} // End of namespace helpers
