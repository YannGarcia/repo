#include <iostream>
#include <stdexcept>
#include <cstring> // Used for memset, memcpy, strerror

#include <unistd.h>

#include "socket.h"

namespace raspberryComm {

  namespace network {

    socket::socket(const ipv4Address & p_host, const unsigned short p_port, const ChannelFamily p_family) {
      // Setup Type and prototype values
      _family = p_family;
      int proto;
      int type;
      switch (_family) {
      case socket::udp:
	proto = IPPROTO_UDP;
	type = SOCK_DGRAM;
	break;
      case socket::tcp:
	proto = IPPROTO_TCP;
	type = SOCK_STREAM;
	break;
      case socket::sctp:
	proto = IPPROTO_SCTP;
	type = SOCK_SEQPACKET;
	break;
      case  socket::raw:
	proto = 0;
	type = SOCK_RAW;
	break;
      default:
	proto = 0;
	type = SOCK_STREAM;
      } // End of 'switch' statement
      // Create the socket
      if ((_socket = ::socket(AF_INET, type, proto)) < 0) {
	std::cerr << "socket::socket: " << std::strerror(errno) << std::endl;
	_socket = -1;
	// TODO Raise an exception
      }
      // Construct the host sockaddr_in structure
      memset((void *)&_host, 0x00, sizeof(struct sockaddr_in));
      _host.sin_family = AF_INET;
      memcpy((void *)&_host.sin_addr, p_host.addr(), p_host.length());
      _host.sin_port = htons(p_port);
      // Host and remote are identical
      memcpy((void *)&_remote, (const void *)&_host, sizeof(_host));
    } // End of ctor

    socket::socket(const socketAddress & p_host, const ChannelFamily p_family) {
      // Setup Type and prototype values
      _family = p_family;
      int proto;
      int type;
      switch (_family) {
      case socket::udp:
	proto = IPPROTO_UDP;
	type = SOCK_DGRAM;
	break;
      case socket::tcp:
	proto = IPPROTO_TCP;
	type = SOCK_STREAM;
	break;
      case socket::sctp:
	proto = IPPROTO_SCTP;
	type = SOCK_SEQPACKET;
	break;
      case  socket::raw:
	proto = 0;
	type = SOCK_RAW;
	break;
      default:
	proto = 0;
	type = SOCK_STREAM;
      } // End of 'switch' statement
      // Create the socket
      if ((_socket = ::socket(AF_INET, type, proto)) < 0) {
	std::cerr << "socket::socket: " << std::strerror(errno) << std::endl;
	_socket = -1;
	// TODO Raise an exception
      }
      // Construct the sockaddr_in structure
      memcpy((void *)&_host, p_host.addr(), p_host.length());
      // Host and remote are identical
      memcpy((void *)&_remote, (const void *)&_host, sizeof(_host));
    } // End of ctor

    socket::socket(const socketAddress & p_host, const socketAddress & p_remote, const ChannelFamily p_family) {
      // Setup Type and prototype values
      _family = p_family;
      int proto;
      int type;
      switch (_family) {
      case socket::udp:
	proto = IPPROTO_UDP;
	type = SOCK_DGRAM;
	break;
      case socket::tcp:
	proto = IPPROTO_TCP;
	type = SOCK_STREAM;
	break;
      case socket::sctp:
	proto = IPPROTO_SCTP;
	type = SOCK_SEQPACKET;
	break;
      case  socket::raw:
	proto = 0;
	type = SOCK_RAW;
	break;
      default:
	proto = 0;
	type = SOCK_STREAM;
      } // End of 'switch' statement
      // Create the socket
      if ((_socket = ::socket(AF_INET, type, proto)) < 0) {
	std::cerr << "socket::socket: " << std::strerror(errno) << std::endl;
	_socket = -1;
	// TODO Raise an exception
      }
      // Construct the sockaddr_in structure
      memcpy((void *)&_host, p_host.addr(), p_host.length());
      // Host and remote are different
      memcpy((void *)&_remote, (const void *)p_remote.addr(), p_remote.length());
    } // End of ctor

    socket::~socket() {
      if (_socket != -1) {
	if (::shutdown(_socket, SHUT_RDWR) < 0) {
	  std::cerr << "socket::~socket: " << std::strerror(errno) << std::endl;
	}
	if (::close(_socket) < 0) {
	  std::cerr << "socket::~socket: " << std::strerror(errno) << std::endl;
	}
      }
    } // End of dtor

    int socket::connect() {
      if (::connect(_socket, (const sockaddr *)&_host, sizeof(struct sockaddr)) < 0) {
	std::cerr <<  "socket::connect: " << std::strerror(errno) << std::endl;
	return -1;
      }

      return 0;
    } // End of connect
    
    int socket::bind() {
      struct sockaddr_in localHost;
      memcpy((void *)&localHost, (const void *)&_host, sizeof(struct sockaddr));
      localHost.sin_addr.s_addr = htonl(INADDR_ANY);
      if (::bind(_socket, reinterpret_cast<const struct sockaddr *>(&localHost), sizeof(struct sockaddr)) < 0) {
	std::cerr <<  "socket::bind: " << std::strerror(errno) << std::endl;
	return -1;
      }
      
      return 0;
    } // End of bind
    
    int socket::listen(const int backlog) {
      if (::listen(_socket, backlog) < 0) {
	std::cerr <<  "socket::listen: " << std::strerror(errno) << std::endl;
	return -1;
      }
      
      return 0;
    } // End of listen
    
    int socket::send(const std::vector<unsigned char> & p_buffer) {
      int result = -1;
      switch (_family) {
      case socket::udp:
	result = this->sendTo(p_buffer);
	break;
      case socket::tcp:
	// TODO 
	break;
      case socket::sctp:
	// TODO 
	break;
      case socket::raw:
	// TODO 
	break;
      } // End of 'switch' statement

      return result;
    } // End of send

    int socket::receive(std::vector<unsigned char> & p_buffer) {
      int result = -1;
      switch (_family) {
      case socket::udp:
	struct sockaddr_in from;
	result = this->receiveFrom(p_buffer, &from);
	break;
      case socket::tcp:
	// TODO 
	break;
      case socket::sctp:
	// TODO 
	break;
      case socket::raw:
	// TODO 
	break;
      } // End of 'switch' statement

      return result;
    } // End of receive

    int socket::receive(unsigned char *p_buffer, const int p_length) {
      int result = -1;
      switch (_family) {
      case socket::udp:
	struct sockaddr_in from;
	result = this->receiveFrom(p_buffer, p_length, &from);
	break;
      case socket::tcp:
	// TODO 
	break;
      case socket::sctp:
	// TODO 
	break;
      case socket::raw:
	// TODO 
	break;
      } // End of 'switch' statement

      return result;
    } // End of receive

    int socket::sendTo(const std::vector<unsigned char> & p_buffer) {
      std::clog << "socket::sendTo: " << std::dec << p_buffer.size() << std::endl;

      int result;
      do {
	result = sendto(_socket, (const void *)p_buffer.data(), p_buffer.size(), 0, (const struct sockaddr *)&_remote, sizeof(_remote));
      } while ((result < 0) && (errno == EINTR));
      if (result < 0){
	std::cerr <<  "socket::send: " << std::strerror(errno) << std::endl;
	return -1;
      }

      return 0;
    }

    int socket::receiveFrom(std::vector<unsigned char> & p_buffer, struct sockaddr_in * p_from) {
      std::clog << "socket::receiveFrom: " << std::dec << p_buffer.size() << std::endl;

      int result;
      unsigned char *buffer = p_buffer.data();
      socklen_t fromlen; // Length of sender's address
      do {
	fromlen = sizeof(struct sockaddr);
	memset((void *)p_from, 0x00, fromlen);
	result = recvfrom(_socket, (void *)buffer, p_buffer.size(), 0, (struct sockaddr *)p_from, &fromlen);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0){
	std::cerr <<  "socket::recvFrom: " << std::strerror(errno) << std::endl;
	return -1;
      }
      std::clog << "socket::receiveFrom: " << inet_ntoa(p_from->sin_addr) << ":" << (int)ntohs(p_from->sin_port) << std::endl;

      return 0;
    }

    int socket::receiveFrom(unsigned char *p_buffer, const int p_length, struct sockaddr_in * p_from) {
      std::clog << "socket::receiveFrom: " << std::dec << p_length << std::endl;

      int result;
      socklen_t fromlen; // Length of sender's address
      do {
	fromlen = sizeof(struct sockaddr);
	memset((void *)p_from, 0x00, fromlen);
	result = recvfrom(_socket, (void *)p_buffer, p_length, 0, (struct sockaddr *)p_from, &fromlen);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0){
	std::cerr <<  "socket::recvFrom: " << std::strerror(errno) << std::endl;
	return -1;
      }
      std::clog << "socket::receiveFrom: " << inet_ntoa(p_from->sin_addr) << ":" << (int)ntohs(p_from->sin_port) << std::endl;

      return 0;
    }

  } // End of namespace network

} // End of namespace raspberryComm

