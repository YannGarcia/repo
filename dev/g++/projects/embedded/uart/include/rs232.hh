/**
 * \file    rs232.h
 * \brief   Header file for the rs232 library.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include "libhal_serial.h"

namespace uart {

  class rs232 {
    int _fd;

  public:
    rs232(const std::string & p_device, const int p_speed);
    virtual ~rs232();

    int write(const std::string & p_string);
    int write(const std::vector<unsigned char> & p_buffer);
    int read(std::string & p_line);
    int read(std::vector<unsigned char> & p_buffer);
    char read();

    inline int data_available() { return ::serial_data_available(_fd); };

    inline const int get_fd() const { return _fd; };
      
  }; // End of class rs232

} // End of namespace uart

using namespace uart;
