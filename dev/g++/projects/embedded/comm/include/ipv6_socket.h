/**
 * @file    ipv6_socket.h
 * @brief   Header file for IPv6 socket communication.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "socket_address.h"
#include "ipvx_socket.h"

namespace comm {

  namespace network {

    /**
     * @class ipv6_socket
     * @brief This class implements IPv6 socket behavior
     */
    class ipv6_socket : public ipvx_socket {
    protected:
      bool _is_ipv6_only;
      int32_t _socket;
      struct sockaddr_in6 _host;
      struct sockaddr_in6 _remote;
 
    public:
      /**
       * @brief Constructor for client usage (peer connection)
       * @param p_remote_address IPv6 address or the peer
       * @param p_type Transport protocol
       *
       * @see socket_address
       */
      ipv6_socket(const socket_address & p_remote_address, const channel_type p_type = channel_type::udp);
      /**
       * @brief Constructor for server usage (listener)
       * @param p_host_address IPv6 address of the host (local)
       * @param p_remote_address IPv6 address of the peer
       *
       * @see socket_address
       */
      ipv6_socket(const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type = channel_type::udp);
      /**
       * @brief Close socket and reset class members
       */
      virtual ~ipv6_socket();

      /**
       * @brief Establish a connection with the peer
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t connect() const;
      /**
       * @brief Close the peer connection
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t close();
      /**
       * @brief Associate the socket with the host port (listener only)
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t bind() const;
      /**
       * @brief Wait for incoming connections (listener only)
       * @param p_backlog The number of connections allowed on the incoming queue
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t listen(const uint32_t p_backlog = 5) const;
      /**
       * @brief Accept the next pending connection (listener only)
       * @return The new socket file descriptor on success, -1 otherwise
       */
      virtual const int32_t accept() const;
      /**
       * @brief Send data to peer
       * @param p_buffer The data to send
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t send(const std::vector<uint8_t> & p_buffer) const;
      /* virtual const int32_t receive(std::vector<uint8_t> & p_buffer, struct sockaddr_in6 * p_from) const; */
      /**
       * @brief Receive data from peer
       * @param p_buffer The data to send. The size of the buffer is the number of bytes received
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t receive(std::vector<uint8_t> & p_buffer) const;
      /**
       * @brief Receive data from peer
       * @param p_buffer The data to send. 
       * @param p_length The number of bytes received
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t receive(uint8_t *p_buffer, uint32_t *p_length) const;

      /**
       * @brief Retrieve the socket file descriptor
       * @return The socket file descriptor on success, -1 otherwise
       */
      inline virtual const int32_t get_fd() const { return _socket; };

      inline void set_no_delay(const bool p_flag) { if (_socket != -1) { set_option(IPPROTO_TCP, TCP_NODELAY, (p_flag == true) ? 1 : 0); } };
      inline void set_blocking(const bool p_flag) { };
      inline void set_option(const uint32_t p_protocol, const uint32_t p_option, const uint32_t p_value) { ::setsockopt(_socket, p_protocol, p_option, (void *)&p_value, sizeof(p_value)); };

      inline const bool is_ipv6_only() const { return _is_ipv6_only; };
      
    private:
      const int32_t send_to(const std::vector<uint8_t> & p_buffer) const;
      const int32_t receive_from(std::vector<uint8_t> & p_buffer, struct sockaddr_in6 * p_from) const;
      const int32_t receive_from(uint8_t *p_buffer, uint32_t *p_length, struct sockaddr_in6 * p_from) const;  
      const int32_t send_tcp(const std::vector<uint8_t> & p_buffer) const;
      const int32_t recv(std::vector<uint8_t> & p_buffer) const;
      const int32_t recv(uint8_t * p_buffer, uint32_t *p_length) const;

      const int32_t process_result() const;
    }; // End of class ipv6_socket

  } // End of namespace network

} // End of namespace comm

using namespace comm::network;
