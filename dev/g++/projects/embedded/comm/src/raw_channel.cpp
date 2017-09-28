/**
 * @file      raw_manager.cpp
 * @brief     Implementation file for communication with raw socket.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <cstring>
#include <stdexcept> // std::out_of_range

#include "raw_channel.h"

#include "converter.h"

namespace comm {

  namespace network {

    raw_channel::raw_channel(const socket_address & p_host_address) {
      //throw std::runtime_error("raw_channel::raw_channel");
      _socket.reset(new socket(p_host_address, channel_type::raw));
      if (_socket.get() == NULL) {
       	std::cerr << "raw_channel::connect: " << std::strerror(errno) << std::endl;
       	throw std::runtime_error("raw_channel::raw_channel");
       }
      // _socket->bind();
    }

    raw_channel::raw_channel(const socket_address & p_host_address, const socket_address & p_remote_address) {
      throw std::runtime_error("raw_channel::raw_channel");
      // if ((_socket = new socket(p_host, p_remote, socket::raw)) == NULL) {
      // 	std::cerr << "raw_channel::connect: " << std::strerror(errno) << std::endl;
      // 	throw std::runtime_error("raw_channel::raw_channel");
      // }
      // _socket->bind();
    }

    raw_channel::~raw_channel() {
      std::clog << "raw_channel::~raw_channel" << std::endl;
      // Socket deleted by abstractChannel dtor
    }

    const int32_t raw_channel::connect() const {
      return _socket->connect();
    }

    const int32_t raw_channel::disconnect() const {
      return _socket->close();
    }
    
    const int32_t raw_channel::accept_connection() const {
      throw std::runtime_error("raw_channel::accept_connection");      
    }
    
    const int32_t raw_channel::write(const std::string & p_string) const {
      if (p_string.length() == 0) {
	return 0;
      }
      
      return _socket->send(converter::get_instance().string_to_bytes(p_string));
    }

    const int32_t raw_channel::write(const std::vector<uint8_t> & p_buffer) const {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->send(p_buffer);
    }

    const int32_t raw_channel::read(std::vector<uint8_t> & p_buffer) const {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->receive(p_buffer);
    }

    const uint8_t raw_channel::read() const {
      uint32_t length = 1;
      uint8_t buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], &length) < 0) {
	return '\00';
      }
      return (uint8_t)buffer[0];
    }

  } // End of namespace network

} // End of namespace comm
