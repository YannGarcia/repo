/**
 * @file      tcp_manager.h
 * @brief     Header file for communication with TCP socket.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

#include <stdexcept>

#include "abstract_channel.h"
#include "socket_address.h"

namespace comm {

  namespace network {
    
    /**
     * @class tcp_channel
     * @brief This class implements TCP socket networking
     *
     * @see abstract_channel
     */
    class tcp_channel : public abstract_channel {
    
    public:
      /**
       * @brief Constructor for client usage (peer connection)
       * @param p_remote_address IP address of the peer
       *
       * @see socket_address
       */
      tcp_channel(const socket_address & p_host_address);
      /**
       * @brief Constructor for server usage (listener)
       * @param p_host_address IP address of the host (local)
       * @param p_remote_address IP address of the peer
       *
       * @see socket_address
       */
      tcp_channel(const socket_address & p_host_address, const socket_address & p_remote_address);
      tcp_channel(const int32_t p_socket, const socket_address & p_host, const socket_address & p_remote);
      /**
       * @brief Default destructor
       */
      virtual ~tcp_channel();
      
      /**
       * @brief Establish a connection with the peer
       * @return 0 on success, -1 otherwise
       */
      const int32_t connect() const;
      /**
       * @brief Accept the next pending connection (listener only)
       * @return The new socket file descriptor on success, -1 otherwise
       */
      const int32_t accept_connection() const;
      /**
       * @brief Close the peer connection
       * @return 0 on success, -1 otherwise
       */
      const int32_t disconnect() const;
      /**
       * @brief Send data to peer
       * @param p_string The string data to send
       * @return 0 on success, -1 otherwise
       */
      const int32_t write(const std::string & p_string) const;
      /**
       * @brief Send data to peer
       * @param p_buffer The bytes data to send
       * @return 0 on success, -1 otherwise
       */
      const int32_t write(const std::vector<uint8_t> & p_buffer) const;
      /**
       * @brief Retrieve data sent by peer
       * @param p_buffer The data to send. 
       * @return 0 on success, -1 otherwise
       */
      const int32_t read(std::vector<uint8_t> & p_buffer) const;
      /**
       * @brief Retrieve the first byte available
       * @return 0 on success, -1 otherwise
       */
      const uint8_t read() const;
      /**
       * @brief Retrieve the number of bytes available
       * @return 0 on success, -1 otherwise
       */
      inline const int32_t data_available() const { throw std::runtime_error("Not implemented yet"); };
      
      
    }; // End of class tcp_channel

  } // End of namespace network

} // End of namespace comm

using namespace comm::network;
