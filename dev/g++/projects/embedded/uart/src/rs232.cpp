/**
 * @file    helper.cpp
 * @brief   Implementation file for the Helper library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <iomanip>
#include <sstream> // std::ostringstream
#include <stdexcept> // std::out_of_range
#include <cstring> // Used std::strerror
#include <vector>

#include <unistd.h> // Used for read

#include "rs232.h"

namespace uart {

  rs232::rs232(const std::string & p_device, const int p_speed) {
    std::clog << ">>> rs232::rs232: " << p_device << ", " << p_speed << std::endl;

    if ((_fd = ::serial_open(p_device.c_str(), p_speed)) < 0) {
      _fd = -1;
      std::ostringstream os;
      os << "rs232::rs232: Wrong parameters: " << std::strerror(errno) << std::endl;
      throw std::out_of_range(os.str()); 
    }
      
    std::clog << "<<< rs232::rs232: " << (int)_fd << " - " << (int)(this->get_fd()) << std::endl;
  } // End of ctor

  rs232::~rs232() {
    std::clog << ">>> rs232::~rs232: " << (int)_fd << std::endl;

    if (_fd != -1) {
      ::serial_close(_fd);
      _fd = -1;
    }
  }

  int rs232::write(const std::string & p_string) {
    //      std::clog << "<<< rs232::write: " << p_string << " - " << (int)(this->get_fd()) << std::endl;
      
    if (p_string.length() == 0) {
      return 0;
    }
      
    ::serial_put_string(_fd, p_string.c_str());
    return p_string.length();
  }

  int rs232::write(const std::vector<unsigned char> & p_buffer) {
    if (p_buffer.size() == 0) {
      return 0;
    }
    ::serial_put_string(_fd, reinterpret_cast<const char *>(p_buffer.data()));

    return p_buffer.size();
  }

  int rs232::read(std::string & p_line) {
    p_line.clear();
      
    char c = static_cast<char>(::serial_get_char(_fd) & 0xff);
    while (c != '\r') {
      p_line += c;
      c = static_cast<char>(::serial_get_char(_fd) & 0xff);
    } // End of 'while' statement
    if (c == '\r') p_line += c;

    return p_line.length();
  }

  int rs232::read(std::vector<unsigned char> & p_buffer) {
    if (::serial_data_available(_fd) <= 0) {
      return -1;
    }

    int count = ::read(_fd, p_buffer.data(), p_buffer.size());
    return count;
  }

  char rs232::read() {
    /*if (::serial_data_available(_fd) <= 0) {
      return '\00';
      }*/

    char c = static_cast<char>(::serial_get_char(_fd) & 0xff);
    std::clog << "rs232::read: read '" << c << "'" << std::endl;
    return c;
    //      return static_cast<char>(::serial_get_char(_fd) & 0xff);
  }

} // End of namespace uart
