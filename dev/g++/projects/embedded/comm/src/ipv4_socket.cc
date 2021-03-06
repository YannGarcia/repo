/**
 * @file      ipv4_socket.cpp
 * @brief     Implememtation file for IPv4 socket communication.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <iostream>
#include <cstring> // Used for memcpy, strerror
#include <memory> // Used for memset
#include <stdexcept>

#include <unistd.h> // Used for ::close

#include <sys/ioctl.h>
 
#include "ipv4_socket.hh"
#include "channel_manager.hh"

namespace comm {

  namespace network {

    ipv4_socket::ipv4_socket(const socket_address & p_remote_address, const channel_type p_type) {
      std::clog << ">>> ipv4_socket::ipv4_socket(1): " << p_remote_address.to_string() << " - " <<  static_cast<unsigned int>(p_type) << std::endl;

      uint32_t family = PF_INET;
      uint32_t proto;
      uint32_t type;
      _type = p_type;
      switch (_type) {
      case channel_type::udp:
        proto = IPPROTO_UDP;
        type = SOCK_DGRAM;
        break;
      case channel_type::tcp:
        proto = IPPROTO_TCP;
        type = SOCK_STREAM;
        break;
      case channel_type::sctp:
        proto = IPPROTO_SCTP;
        type = SOCK_SEQPACKET;
        break;
      case channel_type::raw:
        proto = IPPROTO_RAW;
        type = SOCK_RAW;
        family = AF_PACKET;
        memset(&_if_interface, 0x00, sizeof(struct ifreq));
        memset(&_if_mac_addr, 0x00, sizeof(struct ifreq));
        // Need to setup up the NIC index using set_nic_name()
        break;
      default:
        proto = 0;
        type = SOCK_STREAM;
      } // End of 'switch' statement
      // Create the socket
      if ((_socket = ::socket(family, type, proto)) < 0) {
        std::cerr << "ipv4_socket::ipv4_socket(1): " << std::strerror(errno) << std::endl;
        _socket = -1;
        throw std::runtime_error("ipv4_socket");
      }
      // Construct the remote sockaddr_in structure
      ::memset((void *)&_remote, 0x00, sizeof(struct sockaddr_in));
      _remote.sin_family = family;
      _remote.sin_port = htons(p_remote_address.port());
      ::memcpy((void*)&_remote.sin_addr, p_remote_address.addr(), p_remote_address.length());
      // Reset _host, note used
      ::memset((void *)&_host, 0x00, sizeof(_host));
    } // End of ctor

    ipv4_socket::ipv4_socket(const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type) {
      std::clog << ">>> ipv4_socket::ipv4_socket(2): " << p_host_address.to_string() << " - " << p_remote_address.to_string() << " - " << static_cast<unsigned int>(p_type) << std::endl;

      uint32_t family = PF_INET;
      uint32_t proto;
      uint32_t type;
      _type = p_type;
      switch (_type) {
      case channel_type::udp:
        proto = IPPROTO_UDP;
        type = SOCK_DGRAM;
        break;
      case channel_type::tcp:
        proto = IPPROTO_TCP;
        type = SOCK_STREAM;
        break;
      case channel_type::sctp:
        proto = IPPROTO_SCTP;
        type = SOCK_SEQPACKET;
        break;
      case channel_type::raw:
        proto = IPPROTO_RAW;
        type = SOCK_RAW;
        family = AF_PACKET;
        memset(&_if_interface, 0x00, sizeof(struct ifreq));
        memset(&_if_mac_addr, 0x00, sizeof(struct ifreq));
        // Need to setup up the NIC index using set_nic_name()
        break;
      default:
        proto = 0;
        type = SOCK_STREAM;
      } // End of 'switch' statement
      // Create the socket
      if ((_socket = ::socket(family, type, proto)) < 0) {
        std::cerr << "ipv4_socket::ipv4_socket(2): " << std::strerror(errno) << std::endl;
        _socket = -1;
        throw std::runtime_error("ipv4_socket");
      }
      // Construct the remote sockaddr_in structure
      ::memset((void *)&_host, 0x00, sizeof(struct sockaddr_in));
      _host.sin_family = family;
      _host.sin_port = htons(p_host_address.port());
      _host.sin_addr.s_addr = htonl(INADDR_ANY);
      std::clog << "ipv4_socket::ipv4_socket: _host.sin_family=" << _host.sin_family << ", _host.sin_port=" << _host.sin_port << ", _host.sin_addr.s_addr=" << _host.sin_addr.s_addr << std::endl;
      //      ::memcpy((void *)&_host.sin_addr, p_host_address.addr(), p_host_address.length());
      // Construct the remote sockaddr_in structure
      ::memset((void *)&_remote, 0x00, sizeof(struct sockaddr_in));
      _remote.sin_family = family;
      ::memcpy((void *)&_remote.sin_addr, p_remote_address.addr(), p_remote_address.length());
      _remote.sin_port = htons(p_remote_address.port());
    } // End of ctor

    ipv4_socket::ipv4_socket(const int32_t p_socket, const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type) {
      std::clog << ">>> ipv4_socket::ipv4_socket(3): " << p_socket << " - " << p_host_address.to_string() << " - " << p_remote_address.to_string() << " - " << static_cast<unsigned int>(p_type) << std::endl;

      uint32_t family = PF_INET;
      uint32_t type;
      _type = p_type;
      switch (_type) {
      case channel_type::udp:
        type = SOCK_DGRAM;
        break;
      case channel_type::tcp:
        type = SOCK_STREAM;
        break;
      case channel_type::sctp:
        type = SOCK_SEQPACKET;
        break;
      case channel_type::raw:
        type = SOCK_RAW;
        family = AF_PACKET;
        memset(&_if_interface, 0x00, sizeof(struct ifreq));
        memset(&_if_mac_addr, 0x00, sizeof(struct ifreq));
        // Need to setup up the NIC index using set_nic_name()
        break;
      default:
        type = SOCK_STREAM;
      } // End of 'switch' statement
      // Create the socket
      _socket = p_socket;
      // Construct the remote sockaddr_in structure
      ::memset((void *)&_host, 0x00, sizeof(struct sockaddr_in));
      _host.sin_family = family;
      _host.sin_port = htons(p_host_address.port());
      ::memcpy((void *)&_host.sin_addr, p_host_address.addr(), p_host_address.length());
      // Construct the remote sockaddr_in structure
      ::memset((void *)&_remote, 0x00, sizeof(struct sockaddr_in));
      _remote.sin_family = family;
      ::memcpy((void *)&_remote.sin_addr, p_remote_address.addr(), p_remote_address.length());
      _remote.sin_port = htons(p_remote_address.port());
      std::clog << "<<< ipv4_socket::ipv4_socket(3)" << std::endl;
    } // End of ctor

    ipv4_socket::~ipv4_socket() {
      close();
      ::memset((void *)&_host, 0x00, sizeof(_host));
      ::memset((void *)&_remote, 0x00, sizeof(_remote));
    } // End of dtor

    const int32_t ipv4_socket::connect() const {
      std::clog << ">>> ipv4_socket::connect: " << _socket << std::endl;

      if (::connect(_socket, (const sockaddr *)&_remote, sizeof(struct sockaddr)) == -1) {
        return process_result();
      }

      std::clog << "<<< ipv4_socket::connect succeed" << std::endl;
      return 0; // Succeed
    } // End of connect

    const int32_t ipv4_socket::close() {
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
    
    const int32_t ipv4_socket::bind() const {
      std::clog << ">>> ipv4_socket::bind: " << _socket << std::endl;

      int32_t reuse = 1;
      if (::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) {
        return process_result();
      }

      std::clog << "ipv4_socket::bind: _host.sin_family=" << _host.sin_family << ", _host.sin_port=" << _host.sin_port << ", _host.sin_addr.s_addr=" << _host.sin_addr.s_addr << std::endl;
      if (::bind(_socket, reinterpret_cast<const struct sockaddr *>(&_host), sizeof(struct sockaddr)) == -1) {
        return process_result();
      }
      
      std::clog << "<<< ipv4_socket::bind: succeed" << std::endl;
      return 0;
    } // End of bind
    
    const int32_t ipv4_socket::listen(const uint32_t p_backlog) const {
      std::clog << ">>> ipv4_socket::listen: " << _socket << ", " << p_backlog << std::endl;

      if (::listen(_socket, p_backlog) == -1) {
        return process_result();
      }
      
      std::clog << "<<< ipv4_socket::listen: succeed" << std::endl;
      return 0;
    } // End of listen

    const int32_t ipv4_socket::accept() const {
      std::clog << ">>> ipv4_socket::accept: " << _socket << std::endl;

      struct sockaddr_in addr = { 0 };
      socklen_t len  = sizeof(addr);
      int32_t fd = ::accept(_socket, (struct sockaddr *)&addr, &len);
      std::clog << "ipv4_socket::accept: fd=" << fd << std::endl;
      if (fd < 0) {
        if (process_result() == -1) {
          return -1;
        }
      }

      char ipstr[INET_ADDRSTRLEN];
      inet_ntop(addr.sin_family, &addr.sin_addr, ipstr, sizeof(ipstr));
      socket_address remote(std::string(ipstr), htons(addr.sin_port));
      inet_ntop(_host.sin_family, &_host.sin_addr, ipstr, sizeof(ipstr));
      socket_address host(std::string(ipstr), htons(_host.sin_port));
      //      std::clog << "ipv4_socket::accept: new connection from " << addr.sin_addr << ", port: " << htons(addr.sin_port) << std::endl;

      int32_t result = channel_manager::get_instance().create_channel(fd, host, remote);
      std::clog << "<<< ipv4_socket::accept: channel idx=" << result << std::endl;
      return result;
      //      return channel_manager::get_instance().create_channel(fd, host, remote);
    }
    
    const int32_t ipv4_socket::send(const std::vector<uint8_t> & p_buffer) const {
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
        result = this->send_raw(p_buffer);
        break;
      } // End of 'switch' statement

      return result;
    } // End of send

    const int32_t ipv4_socket::receive(std::vector<uint8_t> & p_buffer) const {
      int32_t result = -1;
      switch (_type) {
      case channel_type::udp: {
        struct sockaddr_in from;
        result = this->receive_from(p_buffer, &from);
      }
        break;
      case channel_type::tcp:
        result = this->recv(p_buffer);
        break;
      case channel_type::sctp:
        // TODO 
        break;
      case channel_type::raw: {
        struct sockaddr_ll from;
        result = this->receive_raw(p_buffer, &from);
      }
        break;
      } // End of 'switch' statement

      return result;
    } // End of receive

    const int32_t ipv4_socket::receive(uint8_t *p_buffer, uint32_t *p_length) const {
      int32_t result = -1;
      switch (_type) {
      case channel_type::udp: {
        struct sockaddr_in from;
        result = this->receive_from(p_buffer, p_length, &from);
        break;
      }
      case channel_type::tcp:
        result = this->recv(p_buffer, p_length);
        break;
      case channel_type::sctp:
        // TODO 
        break;
      case channel_type::raw: {
        struct sockaddr_ll from;
        result = this->receive_raw(p_buffer, p_length, &from);
      }
        break;
      } // End of 'switch' statement

      return result;
    } // End of receive

    const int32_t ipv4_socket::send_to(const std::vector<uint8_t> & p_buffer) const {
      std::clog << "ipv4_socket::send_to: " << std::dec << p_buffer.size() << std::endl;

      int32_t result;
      do {
        result = ::sendto(_socket, (const void *)p_buffer.data(), p_buffer.size(), 0, (const struct sockaddr *)&_remote, sizeof(_remote));
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
        std::cerr <<  "ipv4_socket::send_to: " << std::strerror(errno) << std::endl;
        return -1;
      }

      return 0;
    }

    const int32_t ipv4_socket::send_raw(const std::vector<uint8_t> & p_buffer) const {
      std::clog << "ipv4_socket::send_raw: " << std::dec << p_buffer.size() << std::endl;

      int32_t result;
      // Setup 'from' parameter
      struct sockaddr_ll sa = { 0 };
      sa.sll_family = AF_INET;
      sa.sll_ifindex = _if_interface.ifr_ifindex;
      sa.sll_halen = ETH_ALEN;
      struct ether_header *eh = (struct ether_header *)((unsigned char *)p_buffer.data());
      // Setup the destination MAC address
      sa.sll_addr[0] = eh->ether_dhost[0];
      sa.sll_addr[1] = eh->ether_dhost[1];
      sa.sll_addr[2] = eh->ether_dhost[2];
      sa.sll_addr[3] = eh->ether_dhost[3];
      sa.sll_addr[4] = eh->ether_dhost[4];
      sa.sll_addr[5] = eh->ether_dhost[5];
      // Overwrite the source MAC address
      // FIXME: It should not be a good idea, perhaps it should be removed later
      eh->ether_shost[0] = ((uint8_t *)&_if_mac_addr.ifr_hwaddr.sa_data)[0];
      eh->ether_shost[1] = ((uint8_t *)&_if_mac_addr.ifr_hwaddr.sa_data)[1];
      eh->ether_shost[2] = ((uint8_t *)&_if_mac_addr.ifr_hwaddr.sa_data)[2];
      eh->ether_shost[3] = ((uint8_t *)&_if_mac_addr.ifr_hwaddr.sa_data)[3];
      eh->ether_shost[4] = ((uint8_t *)&_if_mac_addr.ifr_hwaddr.sa_data)[4];
      eh->ether_shost[5] = ((uint8_t *)&_if_mac_addr.ifr_hwaddr.sa_data)[5];
      // Send the data
      do {
        result = ::sendto(_socket, (const void *)p_buffer.data(), p_buffer.size(), 0, (const struct sockaddr *)&sa, sizeof(struct sockaddr_ll));
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
        std::cerr <<  "ipv4_socket::send_raw: " << std::strerror(errno) << std::endl;
        return -1;
      }

      return 0;
    }

    const int32_t ipv4_socket::receive_from(std::vector<uint8_t> & p_buffer, struct sockaddr_in * p_from) const {
      std::clog << ">>> ipv4_socket::receive_from (1): " << std::dec << p_buffer.size() << std::endl;

      int32_t result;
      uint8_t *buffer = p_buffer.data();
      socklen_t fromlen; // Length of sender's address
      do {
        fromlen = sizeof(struct sockaddr_in);
        ::memset((void *)p_from, 0x00, fromlen);
        result = ::recvfrom(_socket, static_cast<void *>(buffer), p_buffer.size(), 0, (struct sockaddr *)p_from, &fromlen);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
        std::cerr <<  "ipv4_socket::recv_from (1): " << std::strerror(errno) << std::endl;
        return -1;
      }
      p_buffer.resize(result);

      char ipstr[INET_ADDRSTRLEN];
      inet_ntop(p_from->sin_family, &p_from->sin_addr, ipstr, sizeof(ipstr));
      std::clog << "ipv4_socket::receive_from (1): " << ipstr << ":" << (int)ntohs(p_from->sin_port) << ", " << result << " bytes received - Fd=" << _socket << std::endl;

      return 0;
    }

    const int32_t ipv4_socket::receive_from(uint8_t *p_buffer, uint32_t *p_length, struct sockaddr_in * p_from) const {
      //      std::clog << ">>> ipv4_socket::receive_from (2): " << std::dec << *p_length << std::endl;

      int32_t result;
      socklen_t fromlen; // Length of sender's address
      do {
        fromlen = sizeof(struct sockaddr_in);
        ::memset((void *)p_from, 0x00, fromlen);
        result = ::recvfrom(_socket, static_cast<void *>(p_buffer), *p_length, 0, (struct sockaddr *)p_from, &fromlen);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
        std::cerr <<  "ipv4_socket::recv_from (2): " << std::strerror(errno) << std::endl;
        return -1;
      }
      *p_length = result;
      std::clog << "ipv4_socket::receive_from (2): " << inet_ntoa(p_from->sin_addr) << ":" << (int)ntohs(p_from->sin_port) << ", " << result << " bytes received" << std::endl;

      return 0;
    }

    const int32_t ipv4_socket::receive_raw(std::vector<uint8_t> & p_buffer, struct sockaddr_ll * p_from) const {
      std::clog << ">>> ipv4_socket::receive_raw (1): " << std::dec << p_buffer.size() << std::endl;

      int32_t result;
      uint8_t *buffer = p_buffer.data();
      do {
        result = ::recvfrom(_socket, static_cast<void *>(buffer), p_buffer.size(), 0, NULL, NULL);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
        std::cerr <<  "ipv4_socket::recv_from (1): " << std::strerror(errno) << std::endl;
        return -1;
      }
      p_buffer.resize(result);

      std::clog << "ipv4_socket::receive_from (1): " << result << " bytes received - Fd=" << _socket << std::endl;

      return 0;
    }

    const int32_t ipv4_socket::receive_raw(uint8_t *p_buffer, uint32_t *p_length, struct sockaddr_ll * p_from) const {
      std::clog << ">>> ipv4_socket::receive_raw (2): " << std::dec << *p_length << std::endl;

      int32_t result;
      do {
        result = ::recvfrom(_socket, static_cast<void *>(p_buffer), *p_length, 0, NULL, NULL);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
        std::cerr <<  "ipv4_socket::recv_from (2): " << std::strerror(errno) << std::endl;
        return -1;
      }
      *p_length = result;
      
      std::clog << "ipv4_socket::receive_from (12): " << result << " bytes received - Fd=" << _socket << std::endl;

      return 0;
    }
    
    const int32_t ipv4_socket::send_tcp(const std::vector<uint8_t> & p_buffer) const {
      std::clog << "ipv4_socket::send_tcp: fd=" << _socket << " - " << p_buffer.size() << std::endl;

      int32_t result;
      do {
        result = ::send(_socket, static_cast<const void *>(p_buffer.data()), p_buffer.size(), 0);
      } while ((result < 0) && (errno == EINTR));
      if (result < 0) {
        std::cerr <<  "ipv4_socket::send_tcp: " << std::strerror(errno) << std::endl;
        return -1;
      }

      return 0;
    }

    const int32_t ipv4_socket::recv(std::vector<uint8_t> & p_buffer) const {
      std::clog << "ipv4_socket::recv (1): fd=" << _socket << " - " << p_buffer.size() << std::endl;

      int32_t result = ::recv(_socket, static_cast<void *>(p_buffer.data()), p_buffer.size(), 0);
      if (result < 0) {
        std::cerr <<  "ipv4_socket::recv(1): " << std::strerror(errno) << std::endl;
        return -1;
      }
      // Set the correct size
      p_buffer.resize(result);

      return 0;
    }
    
    const int32_t ipv4_socket::recv(uint8_t * p_buffer, uint32_t *p_length) const {
      std::clog << "ipv4_socket::recv (2): fd=" << _socket << " - " << *p_length << std::endl;

      int32_t result = ::recv(_socket, static_cast<void *>(p_buffer), *p_length, 0);
      if (result < 0) {
        std::cerr <<  "ipv4_socket::recv(2): " << std::strerror(errno) << std::endl;
        return -1;
      }

      return 0;
    }

    const int32_t ipv4_socket::set_nic_name(const std::string & p_nic_name) const {
      // Sanity check
      if (p_nic_name.empty()) {
        std::cerr <<  "ipv4_socket::set_nic_name: Invalid argument" << std::endl;
        return -1;
      }
      // Get NIC idx from its name
      ::strcpy((char *)&_if_interface.ifr_name, p_nic_name.c_str()); // FIXME check size IFNAMESIS
      if (::ioctl(_socket, SIOCGIFINDEX, &_if_interface) < 0) {
        std::cerr <<  "ipv4_socket::set_nic_name: " << std::strerror(errno) << std::endl;
        return -1;
      }
      // Get NIC MAC address from its index
      ::strcpy((char *)&_if_mac_addr.ifr_name, p_nic_name.c_str()); // FIXME check size IFNAMESIS
      if (::ioctl(_socket, SIOCGIFHWADDR, &_if_mac_addr) < 0) {
        std::cerr <<  "ipv4_socket::set_nic_name: " << std::strerror(errno) << std::endl;
        return -1;
      }
      // Allow the socket to be reused
      int32_t sockopt;
      if (::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) < 0) {
        std::cerr <<  "ipv4_socket::set_nic_name: " << std::strerror(errno) << std::endl;
        return -1;
      }
      // Bind to the device to receive packet
      if (::setsockopt(_socket, SOL_SOCKET, SO_BINDTODEVICE, p_nic_name.c_str(), p_nic_name.length()) < 0) {
        std::cerr <<  "ipv4_socket::set_nic_name: " << std::strerror(errno) << std::endl;
        return -1;
      }
      
      return 0;
    }

    const int32_t ipv4_socket::process_result() const {
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
              std::cerr <<  "ipv4_socket::process_result (SO_ERROR): " << std::strerror(errno) << std::endl;
              return -1; // Terminate here
            } else if (result != 0) {
              std::cerr <<  "ipv4_socket::process_result (Delayed): " << std::strerror(errno) << std::endl;
              break; //return -1; // Terminate here
            } else { // Connected
              std::clog << "ipv4_socket::process_result: done" << std::endl;
              break; // exit loop
            }
          } else if (result != EINTR) {
            std::cerr <<  "ipv4_socket::process_result (select): " << std::strerror(errno) << std::endl;
            return -1; // Terminate here
          } else {
            std::clog <<  "ipv4_socket::process_result: " << std::strerror(errno) << std::endl;
          }
        } while (true);
      } else {
        std::cerr <<  "ipv4_socket::process_result: " << std::strerror(errno) << std::endl;
        return -1; // Terminate here
      }

      return result;
    }
    
  } // End of namespace network

} // End of namespace comm
