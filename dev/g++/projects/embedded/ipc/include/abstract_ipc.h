/**
 * \file    abstract_ipc.h
 * \brief   Header file for IPC interface.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <vector>

namespace ipc {
  
  /**
   * \class abstract_ipc
   * \brief This class unifies the IPC support (e.g. message queue, shared memory...)
   */
  class abstract_ipc {
  public:
    /**
     * \brief Default constructor
     */
    abstract_ipc() { };
    /**
     * \brief Default destructor
     */
    virtual ~abstract_ipc() { };

    /**
     * \brief Send data to peer
     * \param p_string The string data to send
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t write(const std::string & p_string) const = 0;
    /**
     * \brief Send data to peer
     * \param p_buffer The bytes data to send
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t write(const std::vector<uint8_t> & p_buffer) const = 0;
    /**
     * \brief Retrieve data sent by peer
     * \param p_buffer The data to read. The size of p_buffer indicates the number of bytes to read. 
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t read(std::vector<uint8_t> & p_buffer) const = 0;
    /**
     * \brief Retrieve the first byte available
     * \return 0 on success, -1 otherwise
     */
    virtual const uint8_t read() const = 0;
    /**
     * \brief Retrieve the number of bytes available
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t data_available() const = 0;
    
    /**
     * \brief Retrieve the IPC descriptor
     * \return The IPC descriptor on success, -1 otherwise
     */
    virtual const int32_t get_fd() const = 0;
    /* inline virtual const int32_t get_fd() const { return (_ipc.get() != NULL) ?  _ipc->get_fd() : -1; }; */
    
  }; // End of class abstract_ipc

} // End of namespace ipc

using namespace ipc;
