/**
 * @file    shared_memory.cpp
 * @brief   Implementation file for IPC.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy
#include <stdexcept>

#include "shared_memory.h"

namespace ipc {

  shared_memory::shared_memory(const int32_t p_key, const uint32_t p_size, const uint32_t p_offset, const ipc_rights p_ipc_rights) : _sm(-1), _address(NULL), _is_creator(false) {
    std::memset((void *)&_ds, 0xff, sizeof(_ds));

    // Set access flag
    int32_t orights = 0600;
    switch (p_ipc_rights) {
    case ipc_rights::ro:
      orights = 0400;
      break;
    case ipc_rights::rw:
      break;
    } // End of 'switch' statement

      // Open the shared memory
    if ((_sm = ::shmget(p_key, p_size, IPC_EXCL | orights)) == -1) {
      std::ostringstream os;
      os << "shared_memory::shared_memory: Failed to open shared memory: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // Check creation is OK
    if (::shmctl(_sm, IPC_STAT, &_ds) == -1) {
      std::ostringstream os;
      os << "shared_memory::shared_memory: Failed to stat shared memory: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // Attach it
    if ((_address = (uint8_t *)::shmat(_sm, NULL, 0)) == (uint8_t *)-1) { // Failed to attach the shared memory
      std::ostringstream os;
      os << "shared_memory::shared_memory: Failed to attach the shared memory: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }
    _address += p_offset;

    // This instance is not the shared memory creator, do not remove the IPC
  } // End of constructor

  shared_memory::shared_memory(const int32_t p_key, const uint32_t p_size) : _sm(-1), _address(NULL), _is_creator(false) {
    std::memset((void *)&_ds, 0xff, sizeof(_ds));

    // Create the shared memory
    if ((_sm = ::shmget(p_key, p_size, IPC_CREAT  | IPC_EXCL | 0600)) == -1) { // Failed to create shared memory
      std::ostringstream os;
      os << "shared_memory::shared_memory: Failed to create shared memory: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // Check creation is OK
    if (::shmctl(_sm, IPC_STAT, &_ds) == -1) {
      std::ostringstream os;
      os << "shared_memory::shared_memory: Failed to stat shared memory: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // Attach it
    if ((_address = (uint8_t *)::shmat(_sm, NULL, 0)) == (uint8_t *)-1) { // Failed to attach the shared memory
      std::ostringstream os;
      os << "shared_memory::shared_memory: Failed to attach the shared memory: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // This instance is the shared memory creator
    _is_creator = true;
  } // End of constructor

  shared_memory::~shared_memory() {
    close();
  } // End of destructor

  const int32_t shared_memory::close() {
    // std::clog << ">>> shared_memory::close" << std::endl;

    if (_address != NULL) {
      ::shmdt(_address);
      _address = NULL;
    }
    if (_is_creator) {
      if (_sm != -1) {
	::shmctl(_sm, IPC_RMID, NULL);
	_sm = -1;
      }
    } else {
      _sm = -1;
    }

    return 0;
  }

  const int32_t shared_memory::write(const std::string & p_string) const {
    std::copy(p_string.cbegin(), p_string.cend(), _address);

    return 0;
  }

  const int32_t shared_memory::write(const std::vector<uint8_t> & p_buffer) const {
    std::copy(p_buffer.cbegin(), p_buffer.cend(), _address);

    return 0;
  }

  const int32_t shared_memory::read(std::vector<uint8_t> & p_buffer) const {
    std::copy(static_cast<const uint8_t *>(_address), static_cast<const uint8_t *>(_address + p_buffer.size()), p_buffer.begin()); 

    return 0;
  }

  // std::string shared_memory::toString() const {
  //   std::ostringstream os;
  //   os << 
  //     "Id: " << std::dec << _sm << std::endl << 
  //     "Address: " << std::hex << std::setfill('0') << (int)_address << std::dec << std::endl <<  
  //     "Owner uid/gid: " << _ds.shm_perm.uid << "/" << _ds.shm_perm.gid << std::endl <<
  //     "Creator uid/gid: " << _ds.shm_perm.cuid << "/" << _ds.shm_perm.cgid << std::endl <<
  //     std::endl;
  //   return os.str();
  // }

} // End of namespace ipc
