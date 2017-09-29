/**
 * @file      tcp_manager.cpp
 * @brief     Implementation file for communication with TCP socket.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <cstring>
#include <stdexcept>

#include "tcp_channel.h"

#include "converter.h"

namespace comm {

  namespace network {

    tcp_channel::tcp_channel(const socket_address & p_host_address) { // Constructor for a TCP client
      _socket.reset(new socket(p_host_address, channel_type::tcp));
      if (_socket.get() == NULL) { 
      	std::cerr << "tcp_channel::connect(1): " << std::strerror(errno) << std::endl;
	throw new std::runtime_error("tcp_channel::tcp_channel");
      }
      _socket->bind();
    }

    tcp_channel::tcp_channel(const socket_address & p_host_address, const socket_address & p_remote_address) { // Constructor for a TCP server
      _socket.reset(new socket(p_host_address, p_remote_address, channel_type::tcp));
      if (_socket.get() == NULL) { 
      	std::cerr << "tcp_channel::connect(2): " << std::strerror(errno) << std::endl;
	throw new std::runtime_error("tcp_channel::tcp_channel");
      }
      _socket->bind(); // TODO Check if it's correct to add bind/list here
      _socket->listen();
    }

    tcp_channel::tcp_channel(const int32_t p_socket, const socket_address & p_host_address, const socket_address & p_remote_address) { // Constructor for a TCP server
      throw new std::runtime_error("tcp_channel::tcp_channel: Not implemented yet");
      // if ((_socket = new socket(p_socket, p_host_address, p_remote_address)) == NULL) { 
      // 	std::cerr << "tcp_channel::connect(3): " << std::strerror(errno) << std::endl;
      // 	throw new std::runtime_error("tcp_channel::tcp_channel");
      // }
      // _socket->set_no_delay(true);
    }

    tcp_channel::~tcp_channel() {
      std::clog << "tcp_channel::~tcp_channel" << std::endl;
      // Socket deleted by abstractChannel dtor
    }

    const int32_t tcp_channel::connect() const {
      return _socket->connect();
    }

    const int32_t tcp_channel::accept_connection() const {
      return _socket->accept();
    }

    const int32_t tcp_channel::disconnect() const {
      return _socket->close();
    }

    const int32_t tcp_channel::write(const std::string & p_string) const {
      if (p_string.length() == 0) {
	return 0;
      }
      
      return _socket->send(converter::get_instance().string_to_bytes(p_string));
    }

    const int32_t tcp_channel::write(const std::vector<uint8_t> & p_buffer) const {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->send(p_buffer);
    }

    const int32_t tcp_channel::read(std::vector<uint8_t> & p_buffer) const {
      std::clog << ">>> tcp_channel::read" << std::endl;

      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->receive(p_buffer);
    }

    const uint8_t tcp_channel::read() const {
      std::clog << ">>> tcp_channel::read (1)" << std::endl;

      uint32_t length = 1;
      uint8_t buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], &length) < 0) {
	return '\00';
      }
      return (uint8_t)buffer[0];
    }

  } // End of namespace network

} // End of namespace comm
