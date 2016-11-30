/**
 * @file    socket_address.cpp
 * @brief   Implementation file for IPv4/IPv6 address abstraction.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <sstream>
#include <cstring> // Used for memset and memcpy
#include <stdexcept>

#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#include "socket_address.h"
#include "ipv4_address.h"
#include "ipv6_address.h"

namespace comm {

  namespace network {

    socket_address::socket_address(const std::string & p_host, const uint16_t p_port) : _port(p_port) {
      // Try parsing p_host as IPv4
      std::unique_ptr<ipv4_address> ipv4;
      if (ipv4_address::try_parse(p_host, ipv4)) { // This is an IPv4 address
    	_address.reset(ipv4.release());
      } else { // Try IPv6
    	std::unique_ptr<ipv6_address> ipv6;
    	if (ipv6_address::try_parse(p_host, ipv6)) { // This is an IPv4 address
    	  _address.reset(ipv6.release());
    	} else { // Error
    	  throw new std::invalid_argument("socket_address::socket_address: Wrong parameters");
    	}
      }
    } // End of ctor

    socket_address::socket_address(const socket_address & p_host) : _port(p_host.port()) {
      if (p_host.is_ipv4()) {
    	_address.reset(new ipv4_address(static_cast<const in_addr *>(p_host.addr())));
      } else if (p_host.is_ipv6()) {
    	_address.reset(new ipv6_address(static_cast<const in6_addr *>(p_host.addr())));
      } else { // Error
    	throw new std::invalid_argument("socket_address::socket_address: Wrong parameters");
      }
    } // End of ctor

    std::unique_ptr<socket_address> socket_address::parse(const std::string & p_address, const uint16_t p_port) {
      // Sanity checks
      std::unique_ptr<socket_address> ptr;
      if (p_address.empty()) {
	return ptr;
      }

      // TODO
      // struct in6_addr ia = { 0 };
      // if (inet_pton(p_address.c_str(), &ia)) { // Store IPv6 address
      // 	ptr.reset(new ipv6_address(&ia, p_port));
      // }
      
      return ptr;
    }

    bool socket_address::try_parse(const std::string & p_address, const uint16_t p_port, std::unique_ptr<socket_address> & p_result) {
      p_result.reset(socket_address::parse(p_address, p_port).release());
      return (bool)(p_result.get() != NULL);
    }
    
  } // End of namespace network

} // End of namespace comm
