/**
 * @File    channel_manager.cpp
 * @brief   Implementation file for communication channel factory.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <cstring> // Used for strerror
#include <list>
#include <algorithm> // Used for std::transform

#include <fcntl.h>

#include "channel_manager.h"

#include "raw_channel.h"
#include "udp_channel.h"
#include "tcp_channel.h"

namespace comm {

  std::unique_ptr<channel_manager> channel_manager::_instance(new channel_manager());
  uint32_t channel_manager::_counter = 0;

  channel_manager::channel_manager() : _channels(), _polls(), _polling_in_progress(false) {
  } // End of constructor

  channel_manager::~channel_manager() {
    // Nothing to do
  } // End of destructor

  const int32_t channel_manager::poll_channels(const uint32_t p_timeout, std::vector<uint32_t> & p_channels) {
    // std::clog << ">>> channel_manager::poll_channels" << std::endl;

    // Sanity checks
    p_channels.clear();
    if (_polling_in_progress || (_polls.size()) == 0) {
      std::cerr << "channel_manager::poll_channels: Wrong parameters" << std::endl;
      return -1;
    }

    std::list<struct pollfd> polls;
    std::transform(_polls.begin(), _polls.end(), std::back_inserter(polls), [](std::pair<const uint32_t, struct pollfd> & val) { return val.second; } );
    if (polls.size() == 0) {
      std::cerr << "channel_manager::poll_channels: Wrong file descriptors list" << std::endl;
      return -1;
    }
    _polling_in_progress = true;

    int32_t result = ::poll(&*polls.begin(), polls.size(), p_timeout);
    if (result > 0) {
      std::clog << "channel_manager::poll_channels: fd=" << polls.front().fd << " - result=" << result << " Fill p_channels" << std::endl;
      // Fill p_channels
      for (std::list<struct pollfd>::iterator it = polls.begin(); it != polls.end(); ++it) {
	if (it->revents & POLLIN) {
	  std::clog << "channel_manager::poll_channels: POLLIN event on descriptor " << (uint32_t)it->fd << " / index: " << (uint32_t)(&*it - &*polls.begin())<< std::endl;
	  p_channels.push_back(static_cast<uint32_t>(&*it - &*polls.begin())); // The channel id returned by the channel_manager
	  it->revents = 0;
	  result -= 1;
	  if (result == 0) {
	    std::clog << "channel_manager::poll_channels: exit 'for' loop" << std::endl;
	    break; // Exit 'for' loop
	  }
	} // POLLIN case
      } // End of 'for' statement
    } else if (result == 0) {
      // Nothing to do
    } else {
      std::cerr << "channel_manager::poll_channels: " << strerror(errno) << std::endl;
    }

    _polling_in_progress = false;

    // std::clog << "<<< channel_manager::poll_channels: 0" << std::endl;
    return 0;
  } // End of method poll_channels

  const int32_t channel_manager::poll_channels(const uint32_t p_timeout, std::vector<uint32_t> & p_channelsToPoll, std::vector<uint32_t> & p_channels) {
    // std::clog << ">>> channel_manager::poll_channels (1)" << std::endl;

    // Sanity checks
    p_channels.clear();
    if (_polling_in_progress || (p_channelsToPoll.size()) == 0) {
      std::cerr << "channel_manager::poll_channels (1): Wrong parameters" << std::endl;
      return -1;
    }

    std::list<struct pollfd> polls;
    for (auto it = p_channelsToPoll.begin(); it != p_channelsToPoll.end(); ++it) {
      struct pollfd p = { 0 };
      p.fd = get_channel(*it).get_fd();
      p.events = POLLIN | POLLPRI | POLLHUP;
      p.revents = 0;
      //      std::clog << "channel_manager::poll_channels (1): fd=" << (int)p.fd << std::endl;

      polls.push_back(p);
    } // End of 'for' statement
    if (polls.size() == 0) {
      std::cerr << "channel_manager::poll_channels (1): Wrong file descriptors list" << std::endl;
      return -1;
    }
    _polling_in_progress = true;

    int32_t result = ::poll(&*polls.begin(), polls.size(), p_timeout);
    if (result > 0) {
      //      std::clog << "channel_manager::poll_channels (1): fd=" << polls.front().fd << " - result=" << result << " Fill p_channels" << std::endl;
      // Fill p_channels
      for (std::list<struct pollfd>::iterator it = polls.begin(); it != polls.end(); ++it) {
	if (it->revents & POLLIN) {
	  std::clog << "channel_manager::poll_channels (1): POLLIN event on descriptor " << (uint32_t)it->fd << " / index: " << (uint32_t)(&*it - &*polls.begin())<< std::endl;
	  p_channels.push_back(p_channelsToPoll[static_cast<uint32_t>(&*it - &*polls.begin())]); // The channel id returned by the channel_manager
	  it->revents = 0;
	  result -= 1;
	  if (result == 0) {
	    std::clog << "channel_manager::poll_channels (1): exit 'for' loop" << std::endl;
	    break; // Exit 'for' loop
	  }
	}
      } // End of 'for' statement
    } else if (result == 0) {
      // Nothing to do
    } else {
      std::cerr << "channel_manager::poll_channels (1): " << strerror(errno) << std::endl;
    }

    _polling_in_progress = false;

    //    std::clog << "<<< channel_manager::poll_channels (1): 0" << std::endl;
    return 0;
  }

  const int32_t channel_manager::create_channel(const channel_type p_channel_type, const socket_address & p_remote_address) {
    // Create new instance
    abstract_channel *channel = NULL;
    switch (p_channel_type) {
    case channel_type::udp:
      channel = new udp_channel(p_remote_address);
      break;
    case channel_type::tcp:
      channel = new tcp_channel(p_remote_address);
      break;
    case channel_type::sctp:
      // TODO
      break;
    case channel_type::raw:
      channel = new raw_channel(p_remote_address);
      break;
    default:
      // TODO
      break;
    } // End of 'switch' statement
      // Sanity check
    if (channel == NULL) {
      std::cerr << "channel_manager::create_channel (1): Failed to create device instance" << std::endl;
      return -1;
    }

    return initialise_channel(channel);
  } // End of method create_channel
    
  const int32_t channel_manager::create_channel(const channel_type p_channel_type, const socket_address & p_host_address, const socket_address & p_remote_address) {
    // Create new instance
    abstract_channel *channel = NULL;
    switch (p_channel_type) {
    case channel_type::udp:
      channel = new udp_channel(p_host_address, p_remote_address);
      break;
    case channel_type::tcp:
      channel = new tcp_channel(p_host_address, p_remote_address);
      break;
    case channel_type::sctp:
      // TODO
      break;
    case channel_type::raw:
      channel = new raw_channel(p_host_address, p_remote_address);
      break;
    default:
      // TODO
      break;
    } // End of 'switch' statement
      // Sanity check
    if (channel == NULL) {
      std::cerr << "channel_manager::create_channel (2): Failed to create device instance" << std::endl;
      return -1;
    }

    return initialise_channel(channel);
  } // End of method create_channel

  const int32_t channel_manager::create_channel(const int32_t p_socket, const network::socket_address & p_host_address, const network::socket_address & p_remote_address) {
    abstract_channel *channel = new  tcp_channel(p_socket, p_host_address, p_remote_address);
    
    // Sanity check
    if (channel == NULL) {
      std::cerr << "channel_manager::create_channel (4): Failed to create device instance" << std::endl;
      return -1;
    }

    return initialise_channel(channel);
  } // End of method create_channel
  
  const int32_t channel_manager::remove_channel(const uint32_t p_channel) {
    // Sanity check
    if (_channels.find(p_channel) == _channels.end()) {
      std::cerr << "channel_manager::remove_channel: Unknown channel #" << p_channel << std::endl;
      return -1;
    }

    // Remove channel from the poll list
    std::map<const uint32_t, struct pollfd>::iterator del = _polls.find(p_channel);
    if (del != _polls.end()) {
      _polls.erase(del);
    }

    // Remove channel from the map
    abstract_channel *c = _channels.at(p_channel);
    _channels.erase(p_channel);

    // Delete channel
    delete c;

    return 0;
  } // End of method remove_channel

  const uint32_t channel_manager::initialise_channel(abstract_channel * p_channel) {
    // Store it into the map
    uint32_t idx = channel_manager::_counter++;
    _channels.insert(std::pair<const uint32_t, abstract_channel *>(idx, p_channel));
    
    // Set stream not blocking
    if (::fcntl(p_channel->get_fd(), F_SETFL, O_NONBLOCK, 1) == -1) { 
      std::cerr <<  "channel_manager::initialise_channel: failed to set non-blocking\n" << std::endl;
      //      return false;
    }
    
    // Update the poll list
    struct pollfd p = { 0 };
    p.fd = p_channel->get_fd();
    p.events = POLLIN | POLLPRI | POLLHUP | POLLRDHUP;
    p.revents = 0;
    std::clog << "channel_manager::initialise_channel: fd=" << (int)p.fd << std::endl;
    _polls.insert(std::pair<const uint32_t, struct pollfd>(idx, p));

    return idx;
  }
  
} // End of namespace comm
