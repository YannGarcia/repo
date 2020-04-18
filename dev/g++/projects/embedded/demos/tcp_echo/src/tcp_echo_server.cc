/*!
 * \file      tcp_echo.cpp
 * \brief     tcp_echo class implementation file
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include "tcp_echo_server.hh"

tcp_echo_server::tcp_echo_server(const std::string& p_host_address, const std::string& p_peer_address, const uint16_t p_port, logger::logger& p_logger) :
  _logger(p_logger),
  _host_addr(p_host_address, p_port),
  _peer_addr(p_peer_address, p_port) {
  _channel = channel_manager::get_instance().create_channel(channel_type::tcp, _host_addr, _peer_addr);
  if (_channel < 0) {
    // TODO Throw an execption
  }
  _logger.info("tcp_echo_server::tcp_echo_server: _channel= %d", _channel);
} // End of ctor

tcp_echo_server::~tcp_echo_server() {
  channel_manager::get_instance().remove_channel(_channel);
} // End of dtor

void tcp_echo_server::run() {
  _running = true;
  std::vector<uint32_t> fds;
  std::vector<uint32_t> s;
  while(_running) {
    _logger.info("tcp_echo_server: In while: %d", _channel);
    s.clear();
    fds.clear();
    channel_manager::get_instance().poll_channels(500, fds); // 500 ms
    _logger.info("tcp_echo_server: 0000: %d", fds.size());
    std::vector<uint32_t>::iterator it = std::find(fds.begin(), fds.end(), _channel);
    if (it != fds.end()) {
      _logger.info("tcp_echo_server: New incoming connection: %d/%d", *it, _channel);
      int32_t result = channel_manager::get_instance().get_channel(_channel).accept_connection();
      _logger.info("tcp_echo_server: Accept return %d", result);
      if (result != -1) { // Incoming connection
        s.push_back(result);
        _logger.info("tcp_echo_server: 1111");
        fds.clear();
        channel_manager::get_instance().poll_channels(500, s, fds); // 500 ms
        _logger.info("tcp_echo_server: 2222: %d", fds.size());
        std::vector<uint32_t>::iterator it = std::find(fds.begin(), fds.end(), result);
        _logger.info("tcp_echo_server: 3333");
        if (it != fds.end()) { // Some data are available
          _logger.info("tcp_echo_server: 4444: %d", *it);
          // Read incoming data
          std::vector<uint8_t> buffer(128, 0xff);
          uint32_t result = channel_manager::get_instance().get_channel(*it).read(buffer);
          _logger.info("tcp_echo_server: receive data: result=%d", result);
          _logger.info("tcp_echo_server: receive data: '%s'", std::string(buffer.cbegin(), buffer.cend()).c_str());
          // Echo
          _logger.info("Send echo...");
          channel_manager::get_instance().get_channel(*it).write(buffer);
          // Wait some few seconds
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        // Wait some few seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        channel_manager::get_instance().get_channel(*it).disconnect();
        _logger.info("tcp_echo_server: 6666");
        channel_manager::get_instance().remove_channel(*it);
        _logger.info("tcp_echo_server: 7777");
      } else {
        _logger.info("tcp_echo_server: 8888");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    } else {
      _logger.info("tcp_echo_server: 9999");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  } // End of 'while' statement
}
