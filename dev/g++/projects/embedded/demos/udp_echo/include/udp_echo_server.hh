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

class udp_echo_server : public runnable{
  logger::logger& _logger;
  socket_address _host_addr;
  socket_address _peer_addr;
  uint32_t _channel;
public:
  udp_echo_server(const std::string& p_host_address, const std::string& p_peer_address, const uint16_t p_port, logger::logger& p_logger);
  virtual ~udp_echo_server();

  void run();
}; // End of class udp_echo_server
