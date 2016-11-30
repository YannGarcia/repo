/**
 * @file    ipv4_address.cpp
 * @brief   Implementation file for IPv4 address abstraction.
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

#include "ipv4_address.h"

namespace comm {

  namespace network {

    ipv4_address::ipv4_address() {
      ::memset((void *)&_in_addr, 0x00, sizeof(_in_addr));
    }
    
    ipv4_address::ipv4_address(const std::string & p_address) {
      if (p_address.compare("0.0.0.0") == 0) {
	_in_addr.s_addr = INADDR_ANY;
      } else {
	std::unique_ptr<ipv4_address> ipv4;
	if (ipv4_address::try_parse(p_address, ipv4)) { // This is an IPv4 address
	  ::memcpy(static_cast<void *>(&_in_addr), static_cast<const void  *>(ipv4->addr()), sizeof(struct in_addr));
	  ipv4.reset(); // Free pointer
	} else {
	  throw new std::invalid_argument("ipv4_address::ipv4_address: Wrong parameters");	    
	}
      }
    } // End of ctor

    ipv4_address::ipv4_address(const struct in_addr * p_in_addr) {
      // Sanity check
      if (p_in_addr == NULL) {
	  std::cerr << "ipv4_address::ipv4_address: Wrong parameters" << std::endl; 
	  throw new std::invalid_argument("ipv4_address::ipv4_address: Wrong parameters");
      }
      memcpy((void *)&_in_addr, (void *)p_in_addr, sizeof(struct in_addr));
    }

    ipv4_address::ipv4_address(const ipv4_address & p_address) {
      memcpy((void *)&_in_addr, (void *)p_address.addr(), p_address.length());
    }

    std::unique_ptr<ipv4_address> ipv4_address::parse(const std::string & p_address) {
      // Sanity checks
      std::unique_ptr<ipv4_address> ptr;
      if (p_address.empty()) {
	return ptr;
      }

      struct in_addr ia;
      int32_t result = inet_pton(PF_INET, p_address.c_str(), &ia);
      if (result == 1) { // Store IPv4 address
	ptr.reset(new ipv4_address(&ia));
      } else if (result == 0) {
	// Try getaddrinfo
	struct addrinfo hints, *result;
	::memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int32_t status = ::getaddrinfo(p_address.c_str(), NULL, &hints, &result);
	if (status == 0) {
	  struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(result->ai_addr);
	  ::memcpy(static_cast<void *>(&ia), static_cast<const void  *>(&ipv4->sin_addr), sizeof(struct in_addr));
	  ::freeaddrinfo(result); // Free the linked list
	  ptr.reset(new ipv4_address(&ia));
	} else {
	  std::cerr << "ipv4_address::ipv4_address: getaddrinfo: " << ::gai_strerror(status) << std::endl;
	}	  
      } // else, error
      
      return ptr;
    }

    bool ipv4_address::try_parse(const std::string & p_address, std::unique_ptr<ipv4_address> & p_result) {
      p_result.reset(ipv4_address::parse(p_address).release());
      return (bool)(p_result.get() != NULL);
    }

  } // End of namespace network

} // End of namespace comm
