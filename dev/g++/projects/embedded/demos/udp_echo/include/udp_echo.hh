/*!
 * \file      udp_echo.hpp
 * \brief     ud_echo class definition file
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <cstring>
#include <iostream>
#include <algorithm>

#include "helper.hh"
#include "get_opt.hh"
#include "runnable.hh"

#include "socket_address.hh"
#include "channel_manager.hh"

class udp_echo {
  socket_address _addr;
  uint32_t _channel;
public:
  udp_echo(const std::string& p_address, const uint16_t p_port);
  virtual ~udp_echo();
}; // End of class udp_echo