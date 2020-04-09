/**
 * @file      ipv6_socket.cpp
 * @brief     Implememtation file for IPv6 socket communication.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <iostream>
#include <cstring> // Used for memcpy, strerror
#include <memory> // Used for memset
#include <stdexcept>

#include <unistd.h> // Used for ::close

#include "ipv6_socket.hh"
#include "channel_manager.hh"

namespace comm {

  namespace network {

    ipv6_socket::ipv6_socket(const socket_address & p_remote_address, const channel_type p_type) : _is_ipv6_only(true) {
      uint32_t proto;
      uint32_t type;
      _type = p_type;
      switch (_type) {
      case channel_type::udp:
	proto = IPPROTO_UDP;
	type = SOCK_DGRAM;
	break;
      case channel_type::tcp:
	proto = 0;
	type = SOCK_STREAM;
	break;
      case channel_type::sctp:
	proto = IPPROTO_SCTP;
	type = SOCK_SEQPACKET;
	break;
      case channel_type::raw:
	proto = 0;
	type = SOCK_RAW;
	break;
      default:
	proto = 0;
	type = SOCK_STREAM;
      } // End of 'switch' statement
      // Create the socket
      if ((_socket = ::socket(PF_INET6, type, proto)) < 0) {
	std::cerr << "ipv6_socket::socket: " << std::strerror(errno) << std::endl;
	_socket = -1;
	throw std::runtime_error("ipv6_socket::socket");
      }
      // Construct the remote sockaddr_in6 structure
      ::memset((void *)&_remote, 0x00, sizeof(struct sockaddr_in6));
      _remote.sin6_family = PF_INET6;
      ::memcpy((void *)&_remote.sin6_addr, p_remote_address.addr(), p_remote_address.length());
      _remote.sin6_port = htons(p_remote_address.port());
      // Reset host (not used)
      ::memset((void *)&_host, 0x00, sizeof(_host));
      // Set options
      if (_is_ipv6_only) {
	int32_t v = 1;
	::setsockopt(_socket, IPPROTO_IPV6, IPV6_V6ONLY, &v, sizeof(v));
      } else {
	throw std::runtime_error("ipv6_socket::socket: Not implemented yet");
      }
    } // End of ctor

    ipv6_socket::ipv6_socket(const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type) : ipv6_socket(p_remote_address, p_type) {
    // Construct the host sockaddr_in structure
      ::memset((void *)&_host, 0x00, sizeof(struct sockaddr_in6));
      _host.sin6_family = PF_INET6;
      ::memcpy((void *)&_host.sin6_addr, p_host_address.addr(), p_host_address.length());
      _host.sin6_port = htons(p_host_address.port());
    } // End of ctor

    ipv6_socket::ipv6_socket(const int32_t p_socket, const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type) {
      throw std::runtime_error("ipv6_socket::socket: Not implemented yet");
    }

    ipv6_socket::~ipv6_socket() {
      close();
      ::memset((void *)&_host, 0x00, sizeof(_host));
      ::memset((void *)&_remote, 0x00, sizeof(_remote));
    } // End of dtor

    const int32_t ipv6_socket::connect() const {
      if (::connect(_socket, (const sockaddr *)&_host, sizeof(struct sockaddr)) == -1) {
	return process_result();
      }

      return 0;
    } // End of connect
    
    const int32_t ipv6_socket::close() {
      // Sanity check
      if (_socket == -1) {
	return -1;
      }
      if (::shutdown(_socket, SHUT_RDWR) == -1) {
	process_result();
      }
      if (::close(_socket) == -1) {
	process_result();
      }
      _socket = -1;
      
      return 0;
    }
    
    const int32_t ipv6_socket::bind() const {
      // std::clog << ">>> ipv6_socket::bind: " << _socket << std::endl;

      if (::bind(_socket, reinterpret_cast<const struct sockaddr *>(&_host), sizeof(struct sockaddr)) == -1) {
	return process_result();
      }
      
      return 0;
    } // End of bind
    
    const int32_t ipv6_socket::listen(const uint32_t p_backlog) const {
      // std::clog << ">>> ipv6_socket::listen: " << p_backlog << std::endl;

      if (::listen(_socket, p_backlog) == -1) {
      }
      
      return 0;
    } // End of listen

    const int32_t ipv6_socket::accept() const {
      std::clog << ">>> ipv6_socket::accept: " << _socket << std::endl;

      struct sockaddr_in6 addr = { 0 };
      socklen_t len  = sizeof(addr);
      int32_t fd = ::accept(_socket, (struct sockaddr *)&addr, &len);
      if (fd < 0) {
	if (process_result() == -1) {
	  return -1;
	}
      }

      char ipstr[INET6_ADDRSTRLEN];
      inet_ntop(addr.sin6_family, &addr.sin6_addr, ipstr, sizeof(ipstr));
      socket_address remote(std::string(ipstr), htons(addr.sin6_port));
      inet_ntop(_host.sin6_family, &_host.sin6_addr, ipstr, sizeof(ipstr));
      socket_address host(std::string(ipstr), htons(_host.sin6_port));
      std::clog << "ipv6_socket::accept: new connection from " << remote.to_string() << std::endl;

      return channel_manager::get_instance().create_channel(fd, host, remote);
    }
    
    const int32_t ipv6_socket::send(const std::vector<uint8_t> & p_buffer) const {
      int32_t result = -1;
      switch (_type) {
      case channel_type::udp:
	result = this->send_to(p_buffer);
	break;
      case channel_type::tcp:
	result = this->send_tcp(p_buffer);
	break;
      case channel_type::sctp:
	// TODO 
	break;
      case channel_type::raw:
	// TODO 
	break;
      } // End of 'switch' statement

      return result;
    } // End of send

    const int32_t ipv6_socket::receive(std::vector<uint8_t> & p_buffer) const {
      int32_t result = -1;
      switch (_type) {
      case channel_type::udp:
	struct sockaddr_in6 from;
	result = this->receive_from(p_buffer, &from);
	break;
      case channel_type::tcp:
	result = this->recv(p_buffer);
	break;
      case channel_type::sctp:
	// TODO 
	break;
      case channel_type::raw:
	// TODO 
	break;
      } // End of 'switch' statement

      return result;
    } // End of receive

    const int32_t ipv6_socket::receive(uint8_t *p_buffer, uint32_t *p_length) const {
      int32_t result = -1;
      switch (_type) {
      case channel_type::udp:
	struct sockaddr_in6 from;
	result = this->receive_from(p_buffer, p_length, &from);
	break;
      case channel_type::tcp:
	result = this->recv(p_buffer, p_length);
	break;
      case channel_type::sctp:
	// TODO 
	break;
      case channel_type::raw:
	// TODO 
	break;
      } // End of 'switch' statement

      return result;
    } // End of receive

    const int32_t ipv6_socket::send_to(const std::vector<uint8_t> & p_buffer) const {
      std::clog << "ipv6_socket::send_to: " << std::dec << p_buffer.size() << std::endl;

      int32_t result;
      do {
	result = sendto(_socket, (const void *)p_buffer.data(), p_buffer.size(), 0, (const struct sockaddr *)&_remote, sizeof(_remote));
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
	std::cerr <<  "ipv6_socket::send: " << std::strerror(errno) << std::endl;
	return -1;
      }

      return 0;
    }

    const int32_t ipv6_socket::receive_from(std::vector<uint8_t> & p_buffer, struct sockaddr_in6 * p_from) const {
      //      std::clog << ">>> ipv6_socket::receive_from (1): " << std::dec << p_buffer.size() << std::endl;

      int32_t result;
      uint8_t *buffer = p_buffer.data();
      socklen_t fromlen; // Length of sender's address
      do {
	fromlen = sizeof(struct sockaddr_in);
	memset((void *)p_from, 0x00, fromlen);
	result = recvfrom(_socket, static_cast<void *>(buffer), p_buffer.size(), 0, (struct sockaddr *)p_from, &fromlen);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
	std::cerr <<  "ipv6_socket::recv_from (1): " << std::strerror(errno) << std::endl;
	return -1;
      }
      p_buffer.resize(result);
      
      char ipstr[INET6_ADDRSTRLEN];
      inet_ntop(p_from->sin6_family, &p_from->sin6_addr, ipstr, sizeof(ipstr));
      std::clog << "ipv6_socket::receive_from (1): " << ipstr << ":" << (int)ntohs(p_from->sin6_port) << ", " << result << " bytes received - Fd=" << _socket << std::endl;

      return 0;
    }

    const int32_t ipv6_socket::receive_from(uint8_t *p_buffer, uint32_t *p_length, struct sockaddr_in6 * p_from) const {
      //      std::clog << ">>> ipv6_socket::receive_from (2): " << std::dec << *p_length << std::endl;

      int32_t result;
      socklen_t fromlen; // Length of sender's address
      do {
	fromlen = sizeof(struct sockaddr_in);
	memset((void *)p_from, 0x00, fromlen);
	result = recvfrom(_socket, static_cast<void *>(p_buffer), *p_length, 0, (struct sockaddr *)p_from, &fromlen);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
	std::cerr <<  "ipv6_socket::recv_from (2): " << std::strerror(errno) << std::endl;
	return -1;
      }
      *p_length = result;
      //      std::clog << "ipv6_socket::receive_from (2): " << inet_ntoa(p_from->sin_addr) << ":" << (int)ntohs(p_from->sin_port) << std::endl;

      return 0;
    }

    const int32_t ipv6_socket::send_tcp(const std::vector<uint8_t> & p_buffer) const {
      std::clog << "ipv6_socket::send_tcp: fd=" << _socket << " - " << p_buffer.size() << std::endl;

      int32_t result;
      do {
	result = ::send(_socket, static_cast<const void *>(p_buffer.data()), p_buffer.size(), 0);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
	std::cerr <<  "ipv6_socket::send_tcp: " << std::strerror(errno) << std::endl;
	return -1;
      }

      return 0;
    }

    const int32_t ipv6_socket::recv(std::vector<uint8_t> & p_buffer) const {
      std::clog << "ipv6_socket::recv (1): fd=" << _socket << " - " << p_buffer.size() << std::endl;

      int32_t result = ::recv(_socket, static_cast<void *>(p_buffer.data()), p_buffer.size(), 0);
      if (result < 0) {
	std::cerr <<  "ipv6_socket::recv(1): " << std::strerror(errno) << std::endl;
	return -1;
      }
      // Set the correct size
      p_buffer.resize(result);

      return 0;
    }
    
    const int32_t ipv6_socket::recv(uint8_t * p_buffer, uint32_t *p_length) const {
      std::clog << "ipv6_socket::recv (2): fd=" << _socket << " - " << *p_length << std::endl;

      int32_t result = ::recv(_socket, static_cast<void *>(p_buffer), *p_length, 0);
      if (result < 0) {
	std::cerr <<  "ipv6_socket::recv(2): " << std::strerror(errno) << std::endl;
	return -1;
      }

      return 0;
    }

    const int32_t ipv6_socket::process_result() const {
      int32_t result = 0;
      if (errno == EINPROGRESS) {
	struct timeval tv;
	fd_set s;
	do {
	  tv.tv_sec = 2;
	  tv.tv_usec = 0;
	  FD_ZERO(&s); 
	  FD_SET(_socket, &s);
	  if (::select(_socket + 1, NULL, &s, NULL, &tv) > 0) {
	    socklen_t length = sizeof(result);
	    if (::getsockopt(_socket, SOL_SOCKET, SO_ERROR, static_cast<void *>(&result), &length) == -1) {
	      std::cerr <<  "ipv6_socket::process_result (SO_ERROR): " << std::strerror(errno) << std::endl;
	      return -1; // Terminate here
	    } else if (result != 0) {
	      std::cerr <<  "ipv6_socket::process_result (Delayed): " << std::strerror(errno) << std::endl;
	      return -1; // Terminate here
	    } else { // Connected
	      std::clog << "ipv6_socket::process_result: done" << std::endl;
	      break; // exit loop
	    }
	  } else if (result != EINTR) {
	    std::cerr <<  "ipv6_socket::process_result (select): " << std::strerror(errno) << std::endl;
	    return -1; // Terminate here
	  } else {
	    std::clog <<  "ipv6_socket::process_result: " << std::strerror(errno) << std::endl;
	  }
	} while (true);
      } else {
	std::cerr <<  "ipv6_socket::process_result: " << std::strerror(errno) << std::endl;
	return -1; // Terminate here
      }

      return result;
    }
    
  } // End of namespace network

} // End of namespace comm
