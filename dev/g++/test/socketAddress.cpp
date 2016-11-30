#include <iostream>
#include <sstream>

#include <cstring> // Used for memset and memcpy
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#include "socketAddress.h"

namespace raspberryComm {

  namespace network {

    socketAddress::socketAddress(const std::string p_host, const int p_port) {
      _host = new ipv4Address(p_host);
      memset((void *)&_sockaddr, 0x00, sizeof(_sockaddr));
      _sockaddr.sin_port = htons(p_port);
      _sockaddr.sin_family = AF_INET;
      memcpy((void *)&_sockaddr.sin_addr, _host->addr(), _host->length());
    }

    socketAddress::socketAddress(const socketAddress & p_host) {
      _host = new ipv4Address(p_host.host());
      memcpy((void *)&_sockaddr, p_host.addr(), p_host.length());
    }

    socketAddress::~socketAddress() {
      if (_host != NULL) {
	delete _host;
	_host = NULL;
      }
    }

    std::string socketAddress::toString() const {
      std::ostringstream os;
      os << 
	"Address: " << _host->toString() << 
	" - Port   : " << std::dec << ntohs(_sockaddr.sin_port);
      return std::string(os.str());
    }

  } // End of namespace network

} // End of namespace raspberryComm
