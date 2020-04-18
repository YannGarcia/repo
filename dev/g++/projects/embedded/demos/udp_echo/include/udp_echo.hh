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

#include "logger.hh"

class udp_echo {
  logger::logger& _logger;
  socket_address _addr;
  uint32_t _channel;
public:
  udp_echo(const std::string& p_address, const uint16_t p_port, logger::logger& p_logger);
  virtual ~udp_echo();

  int32_t send(const std::string& p_message);
  int32_t receive(std::string& p_message);
}; // End of class udp_echo
