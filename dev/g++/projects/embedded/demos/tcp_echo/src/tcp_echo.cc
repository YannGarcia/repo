/*!
 * \file      tcp_echo.cpp
 * \brief     tcp_echo class implementation file
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include "tcp_echo.hh"

tcp_echo::tcp_echo(const std::string& p_address, const uint16_t p_port, logger::logger& p_logger) :
  _logger(p_logger),
  _addr(p_address, p_port) {
  _channel = channel_manager::get_instance().create_channel(channel_type::tcp, _addr);
  if (_channel < 0) {
    throw std::string("tcp_echo::tcp_echo: Cannot create the channel");
  }
  // Connect to host
  _logger.info("tcp_echo::tcp_echo: _channel= %d", _channel);
  int32_t result = channel_manager::get_instance().get_channel(_channel).connect();
  if (result < 0) {
    throw std::string("tcp_echo::tcp_echo: Failed connect to the server");
  }
  uint32_t tries = 0;
  do {
    std::vector<uint32_t> fds;
    std::vector<uint32_t> s;
    s.push_back(_channel);
    channel_manager::get_instance().poll_channels(1000, s, fds); // 1000 ms
    _logger.info("tcp_echo::tcp_echo: 0000: %d", fds.size());
    std::vector<uint32_t>::iterator it = std::find(fds.begin(), fds.end(), _channel);
    _logger.info("tcp_echo::tcp_echo: 1111");
    if (it != fds.end()) { // Some data are available
      break;
    }
    _logger.info("tcp_echo::tcp_echo: 2222");
  } while (tries++ < 5);
  if (tries == 5) { // Some data are available
    throw std::string("tcp_echo::tcp_echo: Cannot connect to the server");
  }
  _logger.info("tcp_echo::tcp_echo: 3333");
} // End of ctor

tcp_echo::~tcp_echo() {
  channel_manager::get_instance().get_channel(_channel).disconnect();
  channel_manager::get_instance().remove_channel(_channel);
} // End of dtor

int32_t tcp_echo::send(const std::string& p_message) {
  _logger.info("Client send %s", p_message.c_str());
  std::vector<uint8_t> buffer(p_message.cbegin(), p_message.cend());
  return channel_manager::get_instance().get_channel(_channel).write(buffer);
}

int32_t tcp_echo::receive(std::string& p_message) {
  std::vector<uint8_t> buffer(128, 0x00);
  std::vector<uint32_t> fds;
  channel_manager::get_instance().poll_channels(1000, fds); // 1000 ms
  std::vector<uint32_t>::iterator it = std::find(fds.begin(), fds.end(), _channel);
  int32_t result = -1;
  if (it != fds.end()) {
    _logger.info("tcp_echo: New incoming data...");
    result = channel_manager::get_instance().get_channel(_channel).read(buffer);
    _logger.info("Client receive result = %d", result);
    if (result != -1) {
      p_message.assign(buffer.cbegin(), buffer.cend());
    } else {
      p_message = "";
    }
  }
  return result;
}
