/**
 * @file      udp_manager.cpp
 * @brief     Implementation file for communication with UDP socket.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <cstring>
#include <stdexcept>

#include "udp_channel.hh"

#include "converter.hh"

namespace comm {

  namespace network {

    udp_channel::udp_channel(const socket_address & p_host_address) { // Constructorfor an UDP client
      _socket.reset(new socket(p_host_address));
      if (_socket.get() == NULL) {
      	std::cerr << "udp_channel::udp_channel: " << std::strerror(errno) << std::endl;
	throw new std::runtime_error("udp_channel::udp_channel");
      }
      // _socket->bind();
    }

    udp_channel::udp_channel(const socket_address & p_host_address, const socket_address & p_remote_address) { // Constructorfor an UDP server
      _socket.reset(new socket(p_host_address, p_remote_address));
      if (_socket.get() == NULL) {
      	std::cerr << "udp_channel::udp_channel: " << std::strerror(errno) << std::endl;
	throw new std::runtime_error("udp_channel::udp_channel");
      }
      _socket->bind();
    }

    udp_channel::~udp_channel() {
      // Socket deleted by abstractChannel dtor
    }

    const int32_t udp_channel::connect() const {
      return _socket->connect();
    }

    const int32_t udp_channel::disconnect() const {
      return _socket->close();
    }

    const int32_t udp_channel::accept_connection() const {
      return 0; // For UDP, no listen/accept
    }
    
    const int32_t udp_channel::write(const std::string & p_string) const {
      if (p_string.length() == 0) {
	return 0;
      }
      
      return _socket->send(converter::get_instance().string_to_bytes(p_string));
    }

    const int32_t udp_channel::write(const std::vector<uint8_t> & p_buffer) const {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->send(p_buffer);
    }

    const int32_t udp_channel::read(std::vector<uint8_t> & p_buffer) const {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->receive(p_buffer);
    }

    const uint8_t udp_channel::read() const {
      uint32_t length = 1;
      uint8_t buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], &length) < 0) {
	return '\00';
      }
      return (uint8_t)buffer[0];
    }

  } // End of namespace network

} // End of namespace comm
