#include <iostream>

//#include <cstdlib> // Used for memcpy
#include <cstring> // Used for memset and memcpy
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#include "ipv4Address.h"

namespace raspberryComm {

  namespace network {

    ipv4Address::ipv4Address(const std::string & p_address) {
      std::clog << "ipv4Address: " << p_address << std::endl;

      if (p_address.compare("0.0.0.0") == 0) {
	_inaddr.s_addr = INADDR_ANY;
      } else {
	if (inet_aton(p_address.c_str(), &_inaddr) == 0) {
	  std::cerr << "ipv4Address::ipv4Address: Wrong parameters" << std::endl; 
	  // TODO Raise exception
	}
      }
    }

    ipv4Address::ipv4Address(const struct in_addr * p_inaddr) {
      // Sanity check
      if (p_inaddr == NULL) {
	  std::cerr << "ipv4Address::ipv4Address: Wrong parameters" << std::endl; 
	  // TODO Raise exception
      }
      memcpy((void *)&_inaddr, (void *)p_inaddr, sizeof(struct in_addr));
    }

    ipv4Address::ipv4Address(const ipv4Address & p_address) {
      memcpy((void *)&_inaddr, (void *)p_address.addr(), p_address.length());
    }

    std::string ipv4Address::toString() const {
      std::string ip(inet_ntoa(_inaddr));
      return ip;
    }

    ipv4Address * ipv4Address::parse(const std::string& p_address) {
      // Sanity checks
      if (p_address.empty()) {
	return NULL;
      }

      struct in_addr ia = { 0 };
      if (inet_aton(p_address.c_str(), &ia)) { // Store IPv4 address
	return new ipv4Address(&ia);
      }
      
      return NULL;
    }

    bool ipv4Address::tryParse(const std::string & p_address, ipv4Address & p_result) {
      ipv4Address *addr = ipv4Address::parse(p_address);
      return (bool)(addr != NULL);
    }

  } // End of namespace network

} // End of namespace raspberryComm
