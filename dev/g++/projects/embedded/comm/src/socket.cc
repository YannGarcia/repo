/**
 * @file      socket.cpp
 * @brief     Implememtation file for socket communication.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include "socket.hh"
#include "ipv4_socket.hh"
#include "ipv6_socket.hh"

namespace comm {

  namespace network {

    socket::socket(const socket_address & p_host_address, const channel_type p_type) {
      if (p_host_address.is_ipv4()) {
        _socket.reset(static_cast<ipvx_socket *>(new ipv4_socket(p_host_address, p_type)));
      } else {
        _socket.reset(static_cast<ipvx_socket *>(new ipv6_socket(p_host_address, p_type)));
      }
    } // End of ctor

    socket::socket(const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type) {
      if (p_host_address.is_ipv4()) {
        _socket.reset(static_cast<ipvx_socket *>(new ipv4_socket(p_host_address, p_remote_address, p_type)));
      } else {
        _socket.reset(static_cast<ipvx_socket *>(new ipv6_socket(p_host_address, p_remote_address, p_type)));
      }
    } // End of ctor

    socket::socket(const int32_t p_socket, const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type) {
      if (p_host_address.is_ipv4()) {
        _socket.reset(static_cast<ipvx_socket *>(new ipv4_socket(p_socket, p_host_address, p_remote_address, p_type)));
      } else {
        _socket.reset(static_cast<ipvx_socket *>(new ipv6_socket(p_socket, p_host_address, p_remote_address, p_type)));
      }
    }

  } // End of namespace network

} // End of namespace comm
