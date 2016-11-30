/**
 * @Fil    ipc_manager.cpp
 * @brief   Implementation file for IPC factory.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <cstring> // Used for strerror
#include <list>
#include <algorithm> // Used for std::transform

#include <fcntl.h>

#include "ipc_manager.h"

#include "shared_memory.h"
#include "message_queue.h"

namespace ipc {

  std::unique_ptr<ipc_manager> ipc_manager::_instance(new ipc_manager());
  uint32_t ipc_manager::_counter = 0;

  ipc_manager::ipc_manager() : _ipcs(), _polls(), _polling_in_progress(false) {
  } // End of constructor

  ipc_manager::~ipc_manager() {
    // Nothing to do
  } // End of destructor

  const int32_t ipc_manager::poll_ipcs(const uint32_t p_timeout, std::vector<uint32_t> & p_ipcs) {
    // std::clog << ">>> ipc_manager::poll_ipcs" << std::endl;

    // Sanity checks
    p_ipcs.clear();
    if (_polling_in_progress || (_polls.size()) == 0) {
      std::cerr << "ipc_manager::poll_ipcs: Wrong parameters" << std::endl;
      return -1;
    }

    std::list<struct pollfd> polls;
    std::transform(_polls.begin(), _polls.end(), std::back_inserter(polls), [](std::pair<const uint32_t, struct pollfd> & val) { return val.second; } );
    if (polls.size() == 0) {
      std::cerr << "ipc_manager::poll_ipcs: Wrong file descriptors list" << std::endl;
      return -1;
    }
    _polling_in_progress = true;

    int32_t result = ::poll(&*polls.begin(), polls.size(), p_timeout);
    if (result > 0) {
      std::clog << "ipc_manager::poll_ipcs: fd=" << polls.front().fd << " - result=" << result << " Fill p_ipcs" << std::endl;
      // Fill p_ipcs
      for (std::list<struct pollfd>::iterator it = polls.begin(); it != polls.end(); ++it) {
	if (it->revents & POLLIN) {
	  std::clog << "ipc_manager::poll_ipcs: POLLIN event on descriptor " << (uint32_t)it->fd << " / index: " << (uint32_t)(&*it - &*polls.begin())<< std::endl;
	  p_ipcs.push_back(static_cast<uint32_t>(&*it - &*polls.begin())); // The IPC id returned by the ipc_manager
	  it->revents = 0;
	  result -= 1;
	  if (result == 0) {
	    std::clog << "ipc_manager::poll_ipcs: exit 'for' loop" << std::endl;
	    break; // Exit 'for' loop
	  }
	} // POLLIN case
      } // End of 'for' statement
    } else if (result == 0) {
      // Nothing to do
    } else {
      std::cerr << "ipc_manager::poll_ipcs: " << strerror(errno) << std::endl;
    }

    _polling_in_progress = false;

    // std::clog << "<<< ipc_manager::poll_ipcs: 0" << std::endl;
    return 0;
  } // End of method poll_ipcs

  const int32_t ipc_manager::poll_ipcs(const uint32_t p_timeout, std::vector<uint32_t> & p_ipcsToPoll, std::vector<uint32_t> & p_ipcs) {
    // std::clog << ">>> ipc_manager::poll_ipcs (1)" << std::endl;

    // Sanity checks
    p_ipcs.clear();
    if (_polling_in_progress || (p_ipcsToPoll.size()) == 0) {
      std::cerr << "ipc_manager::poll_ipcs (1): Wrong parameters" << std::endl;
      return -1;
    }

    std::list<struct pollfd> polls;
    for (auto it = p_ipcsToPoll.begin(); it != p_ipcsToPoll.end(); ++it) {
      struct pollfd p = { 0 };
      p.fd = get_ipc(*it).get_fd();
      p.events = POLLIN | POLLPRI | POLLHUP;
      p.revents = 0;
      //      std::clog << "ipc_manager::poll_ipcs (1): fd=" << (int)p.fd << std::endl;

      polls.push_back(p);
    } // End of 'for' statement
    if (polls.size() == 0) {
      std::cerr << "ipc_manager::poll_ipcs (1): Wrong file descriptors list" << std::endl;
      return -1;
    }
    _polling_in_progress = true;

    int32_t result = ::poll(&*polls.begin(), polls.size(), p_timeout);
    if (result > 0) {
      //      std::clog << "ipc_manager::poll_ipcs (1): fd=" << polls.front().fd << " - result=" << result << " Fill p_ipcs" << std::endl;
      // Fill p_ipcs
      for (std::list<struct pollfd>::iterator it = polls.begin(); it != polls.end(); ++it) {
	if (it->revents & POLLIN) {
	  std::clog << "ipc_manager::poll_ipcs (1): POLLIN event on descriptor " << (uint32_t)it->fd << " / index: " << (uint32_t)(&*it - &*polls.begin())<< std::endl;
	  p_ipcs.push_back(p_ipcsToPoll[static_cast<uint32_t>(&*it - &*polls.begin())]); // The IPC id returned by the ipc_manager
	  it->revents = 0;
	  result -= 1;
	  if (result == 0) {
	    std::clog << "ipc_manager::poll_ipcs (1): exit 'for' loop" << std::endl;
	    break; // Exit 'for' loop
	  }
	}
      } // End of 'for' statement
    } else if (result == 0) {
      // Nothing to do
    } else {
      std::cerr << "ipc_manager::poll_ipcs (1): " << strerror(errno) << std::endl;
    }

    _polling_in_progress = false;

    //    std::clog << "<<< ipc_manager::poll_ipcs (1): 0" << std::endl;
    return 0;
  }

  const int32_t ipc_manager::create_ipc(const ipc_type p_ipc_type, const ipc_rights p_ipc_rights, const int32_t p_key, const uint64_t p_value, const uint32_t p_offset) {
    // Create new instance
    abstract_ipc *ipc = NULL;
    switch (p_ipc_type) {
    case ipc_type::shared_memory:
      ipc = new shared_memory(p_key, static_cast<uint32_t>(p_value), p_offset, p_ipc_rights);
      break;
    case ipc_type::message_queue:
      ipc = new message_queue(p_key, p_value, p_ipc_rights);
      break;
    default:
      // TODO
      break;
    } // End of 'switch' statement
    // Sanity check
    if (ipc == NULL) {
      std::cerr << "ipc_manager::create_ipc (1): Failed to create device instance" << std::endl;
      return -1;
    }

    return initialise_ipc(ipc);
  } // End of method create_ipc
    
  const int32_t ipc_manager::create_ipc(const ipc_type p_ipc_type, const int32_t p_key, const uint64_t p_value, const uint32_t p_offset) {
    // Create new instance
    abstract_ipc *ipc = NULL;
    switch (p_ipc_type) {
    case ipc_type::shared_memory:
      ipc = new shared_memory(p_key, static_cast<uint32_t>(p_value));
      break;
    case ipc_type::message_queue:
      ipc = new message_queue(p_key, p_value);
      break;
    default:
      // TODO
      break;
    } // End of 'switch' statement
    // Sanity check
    if (ipc == NULL) {
      std::cerr << "ipc_manager::create_ipc (2): Failed to create device instance" << std::endl;
      return -1;
    }

    return initialise_ipc(ipc);
  } // End of method create_ipc

  const int32_t ipc_manager::remove_ipc(const uint32_t p_ipc) {
    // Sanity check
    if (_ipcs.find(p_ipc) == _ipcs.end()) {
      std::cerr << "ipc_manager::remove_ipc: Unknown ipc #" << p_ipc << std::endl;
      return -1;
    }

    // Remove IPC from the poll list
    std::map<const uint32_t, struct pollfd>::iterator del = _polls.find(p_ipc);
    if (del != _polls.end()) {
      _polls.erase(del);
    }

    // Remove IPC from the map
    abstract_ipc *c = _ipcs.at(p_ipc);
    _ipcs.erase(p_ipc);

    // Delete IPC
    delete c;

    return 0;
  } // End of method remove_ipc

  const uint32_t ipc_manager::initialise_ipc(abstract_ipc * p_ipc) {
    // Store it into the map
    uint32_t idx = ipc_manager::_counter++;
    _ipcs.insert(std::pair<const uint32_t, abstract_ipc *>(idx, p_ipc));
    
    // Update the poll list
    struct pollfd p = { 0 };
    p.fd = p_ipc->get_fd();
    p.events = POLLIN | POLLPRI | POLLHUP | POLLRDHUP;
    p.revents = 0;
    // std::clog << "ipc_manager::initialise_ipc: fd=" << (int)p.fd << std::endl;
    _polls.insert(std::pair<const uint32_t, struct pollfd>(idx, p));

    return idx;
  }
  
} // End of namespace ipc
