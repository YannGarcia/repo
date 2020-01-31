/*!
 * \file      helper.cpp
 * \brief     Implementation file for the Helper library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <fstream>

#include "helper.hh"

#define __LINE_LENGTH__ 93

namespace helpers {

  helper * helper::instance = NULL;

  void helper::hexa_dump(const std::vector<unsigned char> p_buffer, const unsigned int p_offset, const unsigned int p_length) {
    hexa_dump(
              static_cast<const unsigned char *>(p_buffer.data()),
              p_offset,
              (p_length == static_cast<unsigned int>(-1)) ? p_buffer.size() - p_offset : p_length
              );
  }

  void helper::hexa_dump(const unsigned char * p_buffer, const unsigned int p_offset, const unsigned int p_length) {
    std::ostringstream os;
    hexa_dump(os, p_buffer, p_offset, p_length);
    std::cout << os.str();
  } // End of method helper::hexa_dump

  void helper::hexa_dump(std::ostringstream & p_output, const unsigned char * p_buffer, const unsigned int p_offset, const unsigned int p_length) {

    int current_index = p_offset;
    unsigned short start_address = ((unsigned short)(p_offset / 16)) * 16;

    // Display header
    p_output << " HEX | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  : 0 1 2 3 4 5 6 7 8 9 A B C D E F " << std::endl;
    p_output << "-----|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-:--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << std::endl;
    //      01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901 2
    //      0         1         2         3         4         5         6         7         8         9
    // Address offset padding
    char line[__LINE_LENGTH__ + 1];
    ::memset(line, 0x20, __LINE_LENGTH__);
    line[__LINE_LENGTH__] = 0x00; // NULL character
    ::sprintf(line, "%04x |", (unsigned short)start_address);
    line[6] = 0x20; // Remove NULL character added by ::sprintf
    unsigned int idx = 0;
    unsigned int hex_offset = 7;
    unsigned int char_offset = 58;
    for ( ; idx < (unsigned int)(current_index % 16); idx++) {
      line[hex_offset] = 0x30;
      line[hex_offset + 1] = 0x30;
      hex_offset += 3;
      char_offset += 2;
    }
    // Fill line by line
    int end_of_dump = p_length + p_offset;
    while(current_index < end_of_dump) {
      for ( ; (idx < 16) && (current_index < end_of_dump); idx++) {
        line[hex_offset] = helper::to_hex_digit(*(p_buffer + current_index) >> 4);
        line[hex_offset + 1] = helper::to_hex_digit(*(p_buffer + current_index) & 0x0f);
        line[char_offset] = helper::to_char_digit(*(p_buffer + current_index));
        // Prepare next byte
        hex_offset += 3;
        char_offset += 2;
        current_index += 1;
      }
      // Display the line
      line[56] = ':';
      line[__LINE_LENGTH__ - 1] = 0x0d;
      line[__LINE_LENGTH__] = 0x0a;
      p_output << line;
      if (current_index < end_of_dump) { // Prepare next line, one line = 16 digits
        start_address += 16;
        ::memset(line, 0x20, __LINE_LENGTH__);
        ::sprintf(line, "%04x |", (unsigned short)start_address);
        line[6] = 0x20; // Remove NULL character added by ::sprintf
        idx = 0;
        hex_offset = 7;
        char_offset = 58;
      } else { // End of line padding
        break;
      }
    } // End of 'while' statement
  } // End of method helper::hexa_dump

  int helper::file_size(const std::string& p_file_name) {
    std::ifstream is(p_file_name, std::ios::in | std::ios::binary | std::ios::ate);
    if (is.is_open()) {
      is.seekg(0, std::ios_base::end);
      int size = is.tellg();
      is.close();
      return size;
    }

    return -1;
  }

  int helper::file_load(const std::string& p_file_name, std::vector<unsigned char>& p_buffer) {
    std::ifstream is(p_file_name, std::ios::in | std::ios::binary | std::ios::ate);
    if (is.is_open()) {
      is.seekg(0, is.end);
      p_buffer.reserve(is.tellg());
      is.seekg(0, is.beg);
      p_buffer.assign((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
      return p_buffer.size();
    }

    return -1;
  }

} // End of namespace helpers
