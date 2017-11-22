/*!
 * \file    share_memory.h
 * \brief   Header file for IPC.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <cstdint>

#include <sys/shm.h>

#include "abstract_ipc.hh"
#include "ipc_rights.hh"

namespace ipc {

  /**
   * @desc This class implements POSIX named shared memmory
   */
  class shared_memory : public abstract_ipc {
  protected:
    int32_t _sm;
    uint8_t *_address;
    struct shmid_ds _ds;
    bool _is_creator;

  public:
    /**
     * @desc Open an existing named shared memory area
     */
    shared_memory(const int32_t p_key, const uint32_t p_size, const uint32_t p_offset, const ipc_rights p_ipc_rights);
    /**
       @desc Create a new named shared memory area
    */
    shared_memory(const int32_t p_key, const uint32_t p_size);
    virtual ~shared_memory();

    inline const uint8_t * get_base_address() const { return _address; };

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
    inline virtual const int32_t get_fd() const { return _sm; };
    
    /* virtual std::string to_string() const; */

  protected:
    /**
     * \brief Clase the shared memory instance
     */
    const int32_t close();
  }; // End of class shared_memory

} // End of namespace ipc

using namespace ipc;
