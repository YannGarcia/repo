/*!
 * \file      udp_echo.cpp
 * \brief     udp_echo class implementation file
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include "udp_echo_server.hh"

udp_echo_server::udp_echo_server(const std::string& p_host_address, const std::string& p_peer_address, const uint16_t p_port, logger::logger& p_logger) :
  _logger(p_logger),
  _host_addr(p_host_address, p_port),
  _peer_addr(p_peer_address, p_port) {
  _channel = channel_manager::get_instance().create_channel(channel_type::udp, _host_addr, _peer_addr);
  if (_channel < 0) {
    // TODO Throw an execption
  }
} // End of ctor

udp_echo_server::~udp_echo_server() {
  channel_manager::get_instance().remove_channel(_channel);
} // End of dtor

void udp_echo_server::run() {
  std::vector<uint32_t> fds;
  _running = true;
  while(_running) {
    channel_manager::get_instance().poll_channels(1000, fds); // 1000 ms
    std::vector<uint32_t>::iterator it = std::find(fds.begin(), fds.end(), _channel);
    if (it != fds.end()) { // Some data are available for _udp channel
      // Read incoming data
      std::vector<uint8_t> buffer(128, 0xff);
      channel_manager::get_instance().get_channel(*it).read(buffer);
      _logger.info("udp_echo_server: receive data: %s", std::string(buffer.cbegin(), buffer.cend()).c_str());
      fds.clear();
    }
  } // End of 'while' statement
}
