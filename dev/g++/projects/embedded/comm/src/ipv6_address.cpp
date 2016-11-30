/**
 * @file    ipv6_address.h
 * @brief   Implementation file for IPv6 address abstraction.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <cstdlib> // Used for memset, strerror
#include <cstring> // Used for memcpy
#include <stdexcept>

#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>

#include "ipv6_address.h"

namespace comm {

  namespace network {

    ipv6_address::ipv6_address() {
      ::memset((void *)&_in_addr, 0x00, sizeof(_in_addr));
    }
    
    ipv6_address::ipv6_address(const std::string & p_address) {
      std::clog << "ipv6_address: " << p_address << std::endl;

      if (p_address.compare("::1") == 0) {
	_in_addr = IN6ADDR_ANY_INIT;
      } else {
	std::unique_ptr<ipv6_address> ipv6;
	if (ipv6_address::try_parse(p_address, ipv6)) { // This is an IPv6 address
	  ::memcpy(static_cast<void *>(&_in_addr), static_cast<const void  *>(ipv6->addr()), sizeof(struct in6_addr));
	  ipv6.reset(); // Free pointer
	} else {
	  throw new std::invalid_argument("ipv6_address::ipv6_address: Wrong parameters");	    
	}
      }
    } // End of ctor

    ipv6_address::ipv6_address(const struct in6_addr * p_in_addr) {
      // Sanity check
      if (p_in_addr == NULL) {
	  std::cerr << "ipv6_address::ipv6_address: Wrong parameters" << std::endl; 
	  throw new std::invalid_argument("ipv6_address::ipv6_address: Wrong parameters");
      }
      memcpy((void *)&_in_addr, (void *)p_in_addr, sizeof(struct in6_addr));
    }

    ipv6_address::ipv6_address(const ipv6_address & p_address) {
      memcpy((void *)&_in_addr, (void *)p_address.addr(), p_address.length());
    }

    std::unique_ptr<ipv6_address> ipv6_address::parse(const std::string & p_address) {
      // Sanity checks
      std::unique_ptr<ipv6_address> ptr;
      if (p_address.empty()) {
	return ptr;
      }

      struct in6_addr ia;
      int32_t result = inet_pton(PF_INET6, p_address.c_str(), &ia);
      if (result == 1) { // Store IPv6 address
	ptr.reset(new ipv6_address(&ia));
      } else if (result == 0) {
	// Try getaddrinfo
	struct addrinfo hints, *result;
	::memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	int32_t status = ::getaddrinfo(p_address.c_str(), NULL, &hints, &result);
	if (status == 0) {
	  struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6 *>(result->ai_addr);
	  ::memcpy(static_cast<void *>(&ia), static_cast<const void  *>(&ipv6->sin6_addr), sizeof(struct in6_addr));
	  ::freeaddrinfo(result); // Free the linked list
	  ptr.reset(new ipv6_address(&ia));
	} else {
	  std::cerr << "ipv6_address::ipv6_address: getaddrinfo: " << ::gai_strerror(status) << std::endl;
	}	  
      } // else, error
      
      return ptr;
    }

    bool ipv6_address::try_parse(const std::string & p_address, std::unique_ptr<ipv6_address> & p_result) {
      p_result.reset(ipv6_address::parse(p_address).release());
      return (bool)(p_result.get() != NULL);
    }

  } // End of namespace network

} // End of namespace comm
