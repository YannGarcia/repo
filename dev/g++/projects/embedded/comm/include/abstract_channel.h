/**
 * @file      abstract_channel.h
 * @brief     Header file for communication channel interface.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "socket.h"

/** Define POLLRDHUP for MAC OS X and CYGWYN */
#if !defined(POLLRDHUP)
#define POLLRDHUP 0x2000
#endif

namespace comm {
  
  /**
   * @class abstract_channel
   * @brief This class unifies the socket networking (e.g. UDP, TCP...)
   *
   * @see protocol_t
   */
  class abstract_channel {
  protected:
    std::unique_ptr<comm::network::socket> _socket; /** Socket instance */

  public:
    /**
     * @brief Default constructor
     */
    abstract_channel() : _socket() { }
    /**
     * @brief Default destructor
     */
    virtual ~abstract_channel() { 
      _socket.reset();
    }

    /**
     * @brief Establish a connection with the peer
     * @return 0 on success, -1 otherwise
     */
    virtual const int32_t connect() const = 0;
    /**
     * @brief Accept the next pending connection (listener only)
     * @return The new socket file descriptor on success, -1 otherwise
     */
    virtual const int32_t accept_connection() const =0;
    /**
     * @brief Close the peer connection
     * @return 0 on success, -1 otherwise
     */
    virtual const int32_t disconnect() const = 0;
    /**
     * @brief Send data to peer
     * @param p_string The string data to send
     * @return 0 on success, -1 otherwise
     */
    virtual const int32_t write(const std::string & p_string) const = 0;
    /**
     * @brief Send data to peer
     * @param p_buffer The bytes data to send
     * @return 0 on success, -1 otherwise
     */
    virtual const int32_t write(const std::vector<uint8_t> & p_buffer) const = 0;
    /**
     * @brief Retrieve data sent by peer
     * @param p_buffer The data to read. The size of p_buffer indicates the number of bytes to read. 
     * @return 0 on success, -1 otherwise
     */
    virtual const int32_t read(std::vector<uint8_t> & p_buffer) const = 0;
    /**
     * @brief Retrieve the first byte available
     * @return 0 on success, -1 otherwise
     */
    virtual const uint8_t read() const = 0;
    /**
     * @brief Retrieve the number of bytes available
     * @return 0 on success, -1 otherwise
     */
    virtual const int32_t data_available() const = 0;

    /**
     * @brief Set the NIC name to be used, in case of RAW socket only
     * @param p_nic_name[in] The NIC name. 
     */
    virtual const int32_t set_nic_name(const std::string & p_nic_name) const { return (_socket.get() != NULL) ? _socket->set_nic_name(p_nic_name) : -1; };
    
    /**
     * @brief Retrieve the socket file descriptor
     * @return The socket file descriptor on success, -1 otherwise
     */
    inline virtual const int32_t get_fd() const { return (_socket.get() != NULL) ?  _socket->get_fd() : -1; };
    
  }; // End of class abstract_channel

} // End of namespace comm

using namespace comm;
