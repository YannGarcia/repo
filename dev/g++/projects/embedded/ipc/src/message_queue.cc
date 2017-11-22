/*!
 * \File    message_queue.cpp
 * \@brief   Implementation file for IPC.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#include <iostream>
#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, std::memcpy
#include <stdexcept>

#include "message_queue.hh"

//#include "dump.h"

namespace ipc {

  message_queue::message_queue(const int32_t p_key, const uint64_t p_msg_type, const ipc_rights p_ipc_rights) : _msg_type(p_msg_type), _mq(-1), _is_creator(false) {
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

      // Open the message queue
    if ((_mq = ::msgget(p_key, IPC_EXCL | orights)) == -1) {
      std::ostringstream os;
      os << "message_queue::message_queue: Failed to open message queue: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // Check creation is OK
    if (::msgctl(_mq, IPC_STAT, &_ds) == -1) {
      std::ostringstream os;
      os << "message_queue::message_queue: Failed to stat message queue: " << std::strerror(errno) << std::endl;
      throw new std::runtime_error(os.str());
    }

    // This instance is not the message queue creator, do not remove the IPC
  } // End of constructor

  message_queue::message_queue(const int32_t p_key, const uint64_t p_msg_type) : _msg_type(p_msg_type), _mq(-1), _is_creator(false) {
    std::memset((void *)&_ds, 0xff, sizeof(_ds));

    // Create the messqge queue
    if ((_mq = ::msgget(p_key, IPC_CREAT  | IPC_EXCL | 0600)) == -1) { // Failed to create messqge queue
      std::ostringstream os;
      os << "message_queue::message_queue: Failed to create messqge queue: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // Check creation is OK
    if (::msgctl(_mq, IPC_STAT, &_ds) == -1) {
      std::ostringstream os;
      os << "message_queue::message_queue: Failed to stat messqge queue: " << std::strerror(errno) << std::endl;
      throw std::runtime_error(os.str());
    }

    // This instance is the messqge queue creator
    _is_creator = true;
  } // End of constructor

  message_queue::~message_queue() {
    close();
  } // End of destructor

  const int32_t message_queue::close() {
    if (_is_creator) {
      if (_mq != -1) {
	::msgctl(_mq, IPC_RMID, NULL);
	_mq = -1;
      }
    } else {
      _mq = -1;
    }

    return 0;
  }

  const int32_t message_queue::write(const std::string & p_string) const {
    //      std::clog << ">>> message_queue::write: " << std::dec << p_string.length() << std::endl;

    std::vector<uint8_t> buffer(0x00, p_string.length());
    std::copy(p_string.cbegin(), p_string.cend(), buffer.begin());
    return write(buffer);
  }

  const int32_t message_queue::write(const std::vector<uint8_t> & p_buffer) const {
    //      std::clog << ">>> message_queue::write: " << std::dec << p_buffer.size() << std::endl;

    uint8_t *data = static_cast<uint8_t *>(new uint8_t[sizeof(uint64_t) + p_buffer.size()]);
    std::memcpy(static_cast<void *>(data), static_cast<const void *>(&_msg_type), sizeof(uint64_t));
    std::memcpy(static_cast<void *>(data + sizeof(uint64_t)), static_cast<const void *>(p_buffer.data()),  p_buffer.size());

    //      dump::hexaDump(data, sizeof(uint64_t) + p_buffer.size());

    if (::msgsnd(_mq, static_cast<const void *>(data),  p_buffer.size()/*Size of the payload*/, IPC_NOWAIT) == -1) {
      std::cerr << "message_queue::message_queue: Failed to send message: " << std::strerror(errno) << std::endl;
      delete [] data;
      return -1;
    }
    delete [] data;
      
    //      std::clog << "<<< message_queue::write (0)" << std::endl;
    return 0;
  }

  const int32_t message_queue::read(std::vector<uint8_t> & p_buffer) const {
    uint8_t *data = static_cast<uint8_t *>(new uint8_t[sizeof(uint64_t) + p_buffer.size()]);
    
    int32_t bytesRead = ::msgrcv(_mq, static_cast<void *>(data), p_buffer.size(), _msg_type, IPC_NOWAIT);
    if (bytesRead == -1) {
      if (errno == E2BIG) {
	if (::msgrcv(_mq, static_cast<void *>(data), p_buffer.size(), _msg_type, IPC_NOWAIT | MSG_NOERROR) == -1) {
	  std::cerr << "message_queue::message_queue: Failed to receive message: " << std::strerror(errno) << std::endl;
	  delete [] data;
	  return -2;
	}
      } else if (errno != ENOMSG) { // 'No message of desired type'
	std::cerr << "message_queue::message_queue: Failed to receive message: " << std::strerror(errno) << std::endl;
	delete [] data;
	return -1;
      } else {
	//std::clog << "<<< message_queue::read (0)" << std::endl;
	delete [] data;
	return 0;
      }
    }

    p_buffer.assign(data + sizeof(uint64_t), data + sizeof(uint64_t) + bytesRead);
    delete [] data;

    return 0;
  }

  // std::string message_queue::toString() const {
  //   std::ostringstream os;
  //   os << 
  // 	"Id: " << std::dec << _mq << std::endl << 
  // 	"Owner uid/gid: " << _ds.msg_perm.uid << "/" << _ds.msg_perm.gid << std::endl <<
  // 	"Creator uid/gid: " << _ds.msg_perm.cuid << "/" << _ds.msg_perm.cgid << std::endl <<
  // 	std::endl;

  //   return os.str();
  // }

} // End of namespace ipc
