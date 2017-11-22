/**
 * \file    message_queue.h
 * \brief   Header file for Message Queue IPC.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <vector>
#include <cstdint>

#include <sys/msg.h>

#include "abstract_ipc.hh"
#include "ipc_rights.hh"

namespace ipc {

  /**
   * @desc This class implements POSIX message queue
   */
  class message_queue : public abstract_ipc {
  protected:
    uint64_t _msg_type; /** Message type, must be > 0 */
    int32_t _mq;
    struct msqid_ds _ds;
    bool _is_creator;

  public:
    /**
       @desc Open an existing message queue area
    */
    message_queue(const int32_t p_key, const uint64_t p_msg_type, const ipc_rights p_ipc_rights);
    /**
       @desc Create a new message queue area
    */
    message_queue(const int32_t p_key, const uint64_t p_msg_type);
    virtual ~message_queue();

    /**
     * \brief Send data to peer
     * \param p_string The string data to send
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t write(const std::string & p_string) const;
    /**
     * \brief Send data to peer
     * \param p_buffer The bytes data to send
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t write(const std::vector<uint8_t> & p_buffer) const;
    /**
     * \brief Retrieve data sent by peer
     * \param p_buffer The data to read. The size of p_buffer indicates the number of bytes to read. 
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t read(std::vector<uint8_t> & p_buffer) const;
    /**
     * \brief Retrieve the first byte available
     * \return 0 on success, -1 otherwise
     */
    virtual const uint8_t read() const { return 0x00; };
    /**
     * \brief Retrieve the number of bytes available
     * \return 0 on success, -1 otherwise
     */
    virtual const int32_t data_available() const { return 0; };
    
    /**
     * \brief Retrieve the shared memory descriptor
     * \return The shared memory descriptor on success, -1 otherwise
     */
    inline virtual const int32_t get_fd() const { return _mq; };
    
  protected:
    const int32_t close();
  }; // End of class message_queue

} // End of namespace ipc

using namespace ipc;
