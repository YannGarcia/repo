/**
 * \file      socket.h
 * \brief     Header file for IPv4/IPv6 socket communication.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <cstdint>
#include <vector>

#include "channel_type.hh"
#include "socket_address.hh"
#include "ipvx_socket.hh"

namespace comm {

  namespace network {

    /**
     * \class socket (implement ipvx_socket)
     * \brief This class unifies the socket behavior (facade for IPv4/IPv6 socket behavior) 
     *
     * \see ipv4_socket
     * \see ipv6_socket
     */
    class socket : public ipvx_socket {
    protected:
      std::unique_ptr<ipvx_socket> _socket; /** Socket instance */
      
    public:
      /**
       * \brief Constructor for client usage (peer connection)
       * \param p_remote_address IPv4/IPv6 address or the peer
       * \param p_type Transport protocol
       *
       * \see socket_address
       */
      socket(const socket_address & p_host_address, const channel_type p_type = channel_type::udp);
      /**
       * \brief Constructor for server usage (listener)
       * \param p_host_address IPv4 address of the host (local)
       * \param p_remote_address IPv4 address of the peer
       *
       * \see socket_address
       */
      socket(const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type = channel_type::udp);

      socket(const int32_t p_socket, const socket_address & p_host_address, const socket_address & p_remote_address, const channel_type p_type = channel_type::tcp);

      /**
       * \brief Close socket and reset class members
       */
      virtual ~socket() { _socket.reset(); };

      /**
       * \brief Establish a connection with the peer
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t connect() const { if (_socket.get() != NULL) { return _socket->connect(); } return -1; };
      /**
       * \brief Close the peer connection
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t close() { if (_socket.get() != NULL) { return _socket->close(); } return -1; };
      /**
       * \brief Associate the socket with the host port (listener only)
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t bind() const { if (_socket.get() != NULL) { return _socket->bind(); } return -1; };
      /**
       * \brief Wait for incoming connections (listener only)
       * \param p_backlog The number of connections allowed on the incoming queue
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t listen(const uint32_t p_backlog = 5) const { if (_socket.get() != NULL) { return _socket->listen(p_backlog); } return -1; };
      /**
       * \brief Accept the next pending connection (listener only)
       * \return The new socket file descriptor on success, -1 otherwise
       */
      virtual inline const int32_t accept() const { if (_socket.get() != NULL) { return _socket->accept(); } return -1; };
      /**
       * \brief Send data to peer
       * \param p_buffer The data to send
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t send(const std::vector<uint8_t> & p_buffer) const { if (_socket.get() != NULL) { return _socket->send(p_buffer); } return -1; };
      /* virtual inline const int32_t receive(std::vector<uint8_t> & p_buffer, socket_address & p_from) const { if (_socket.get() != NULL) { return _socket->receive(p_buffer, p_from); } return -1; }; */
      /**
       * \brief Receive data from peer
       * \param p_buffer The data to send. The size of the buffer is the number of bytes received
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t receive(std::vector<uint8_t> & p_buffer) const { if (_socket.get() != NULL) { return _socket->receive(p_buffer); } return -1; };
      /**
       * \brief Receive data from peer
       * \param p_buffer The data to send. 
       * \param p_length The number of bytes received
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t receive(uint8_t *p_buffer, uint32_t *p_length) const { if (_socket.get() != NULL) { return _socket->receive(p_buffer, p_length); } return -1; };

      /**
       * \brief Retrieve the socket file descriptor
       * \return The socket file descriptor on success, -1 otherwise
       */
      virtual inline const int32_t get_fd() const { if (_socket.get() != NULL) { return _socket->get_fd(); } return -1; };

      /**
       * \brief Set the NIC name to be used, in case of RAW socket only
       * \param p_nic_name[in] The NIC name. 
       * \return 0 on success, -1 otherwise
       */
      virtual inline const int32_t set_nic_name(const std::string & p_nic_name) const {  if (_socket.get() != NULL) { return _socket->set_nic_name(p_nic_name); } return -1; };

      virtual inline void set_no_delay(const bool p_flag) { if (_socket.get() != NULL) _socket->set_no_delay(p_flag); };
      virtual inline void set_blocking(const bool p_flag) { if (_socket.get() != NULL) _socket->set_blocking(p_flag); };
      virtual inline void set_option(const uint32_t p_protocol, const uint32_t p_option, const uint32_t p_value) { if (_socket.get() != NULL) set_option(p_protocol, p_option, p_value); };
    }; // End of class socket

  } // End of namespace network

} // End of namespace comm

using namespace comm::network;
