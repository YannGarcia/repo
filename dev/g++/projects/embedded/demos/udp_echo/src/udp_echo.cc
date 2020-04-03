/*!
 * \file      udp_echo.cpp
 * \brief     udp_echo class implementation file
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include "udp_echo.hh"

udp_echo::udp_echo(const std::string& p_address, const uint16_t p_port) :
  _addr(p_address, p_port) {
  _channel = channel_manager::get_instance().create_channel(channel_type::udp, _addr);
  if (_channel < 0) {
    // TODO Throw an execption
  }
  // Connect to host
  channel_manager::get_instance().get_channel(_channel).connect();
} // End of ctor

udp_echo::~udp_echo() {
  channel_manager::get_instance().get_channel(_channel).disconnect();
  channel_manager::get_instance().remove_channel(_channel);
} // End of dtor

int32_t udp_echo::send(const std::string& p_message) {
  std::vector<uint8_t> buffer(p_message.cbegin(), p_message.cend());
  return channel_manager::get_instance().get_channel(_channel).write(buffer);
}

int32_t udp_echo::receive(std::string& p_message) {
  std::vector<uint8_t> buffer;
  int32_t result = channel_manager::get_instance().get_channel(_channel).read(buffer);
  p_message.assign(buffer.cbegin(), buffer.cend());
  return result;
}
