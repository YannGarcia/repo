/**
 * @file      udp_echo.hpp
 * @brief     ud_echo class definition file
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

#include <cstring>
#include <iostream>
#include <algorithm>

#include "helper.h"
#include "get_opt.h"
#include "runnable.h"

#include "socket_address.h"
#include "channel_manager.h"

class udp_echo {
  socket_address _addr;
  uint32_t _channel;
public:
  udp_echo(const std::string& p_address, const uint16_t p_port);
  virtual ~udp_echo();
}; // End of class udp_echo
