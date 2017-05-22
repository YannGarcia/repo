/**
 * @file    ipvx_socket.h
 * @brief   Header file for abstraction of IPv4/IPv6 socket communication.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <vector>
#include <memory>

#include "channel_type.h"

namespace comm {

  namespace network {

    /**
     * @class ipvx_socket (abstract)
     * @brief This class implements abstraction for IPv4/IPv6 socket behavior
     *
     * @see ipv4_socket
     * @see ipv6_socket
     */
    class ipvx_socket {
    protected:
      channel_type _type;
      
    public:
      virtual ~ipvx_socket() { };
      /**
       * @brief Establish a connection with the peer
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t connect() const = 0;
      /**
       * @brief Close the peer connection
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t close() = 0;
      /**
       * @brief Associate the socket with the host port (listener only)
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t bind() const = 0;
      /**
       * @brief Wait for incoming connections (listener only)
       * @param p_backlog The number of connections allowed on the incoming queue
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t listen(const uint32_t p_backlog = 5) const = 0;
      /**
       * @brief Accept the next pending connection (listener only)
       * @return The new socket file descriptor on success, -1 otherwise
       */
      virtual const int32_t accept() const = 0;
      /**
       * @brief Send data to peer
       * @param p_buffer The data to send
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t send(const std::vector<uint8_t> & p_buffer) const = 0;
      /* virtual const int32_t receive(std::vector<uint8_t> & p_buffer, struct sockaddr_in * p_from) const = 0; */
      /**
       * @brief Receive data from peer
       * @param p_buffer The data to send. The size of the buffer is the number of bytes received
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t receive(std::vector<uint8_t> & p_buffer) const = 0;
      /**
       * @brief Receive data from peer
       * @param p_buffer The data to send. 
       * @param p_length The number of bytes received
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t receive(uint8_t *p_buffer, uint32_t *p_length) const = 0;

      /**
       * @brief Retrieve the socket file descriptor
       * @return The socket file descriptor on success, -1 otherwise
       */
      virtual const int32_t get_fd() const = 0;

      /**
       * @brief Set the NIC name to be used, in case of RAW socket only
       * @param p_nic_name[in] The NIC name. 
       * @return 0 on success, -1 otherwise
       */
      virtual const int32_t set_nic_name(const std::string & p_nic_name) const { return -1; };

      virtual void set_no_delay(const bool p_flag) = 0;      
      virtual void set_blocking(const bool p_flag) = 0;
      virtual void set_option(const uint32_t p_protocol, const uint32_t p_option, const uint32_t p_value) = 0;
    }; // End of class ipvx_socket

  } // End of namespace network

} // End of namespace comm

