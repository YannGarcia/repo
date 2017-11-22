/*!
 * \file      udp_echo.cpp
 * \brief     udp_echo class implementation file
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include "logger_factory.hh"

#include "udp_echo.hpph"

udp_echo::udp_echo(const std::string& p_address, const uint16_t p_port) :
  _addr(p_address, p_port) {
  _channel = channel_manager::get_instance().create_channel(channel_type::udp, _addr);
  if (_channel < 0) {
    // TODO Throw an execption
  }
} // End of ctor

udp_echo::~udp_echo() {
  channel_manager::get_instance().remove_channel(_channel);
} // End of dtor
