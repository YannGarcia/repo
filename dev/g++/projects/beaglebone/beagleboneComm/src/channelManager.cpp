#include <cstring> // Used for strerror
#include <list>
#include <algorithm> // Used for std::transform

#include "channelManager.h"

#include "udpChannel.h"
#include "tcpChannel.h"
#include "rawChannel.h"
#include "serialChannel.h"

namespace beagleboneComm {

  using namespace network;
  using namespace serial;

  std::unique_ptr<channelManager> channelManager::_instance(new channelManager());
  int channelManager::_counter = 0;

  channelManager::channelManager() : _channels(), _polls() {
    _pollingInProgress = false;
  } // End of constructor

  channelManager::~channelManager() {
    // Nothing to do
  } // End of destructor

  int channelManager::pollChannels(const int p_timeout, std::vector<int> & p_channels) {
    //    std::clog << ">>> channelManager::pollChannels" << std::endl;

    // Sanity checks
    p_channels.clear();
    if (_pollingInProgress || (_polls.size()) == 0) {
      std::cerr << "channelManager::pollChannels: Wrong parameters" << std::endl;
      return -1;
    }

    std::list<struct pollfd> polls;
    std::transform(_polls.begin(), _polls.end(), std::back_inserter(polls), [](std::pair<const int, struct pollfd> & val) { return val.second; } );
    if (polls.size() == 0) {
      std::cerr << "channelManager::pollChannels: Wrong file descriptors list" << std::endl;
      return -1;
    }
    _pollingInProgress = true;

    int result = ::poll(&*polls.begin(), polls.size(), p_timeout);
    if (result > 0) {
      std::clog << "channelManager::pollChannels: fd=" << polls.front().fd << " - result=" << result << " Fill p_channels" << std::endl;
      // Fill p_channels
      for (std::list<struct pollfd>::iterator it = polls.begin(); it != polls.end(); ++it) {
	if (it->revents & POLLIN) {
	  std::clog << "channelManager::pollChannels: POLLIN event on descriptor " << (int)it->fd << " / index: " << (int)(&*it - &*polls.begin())<< std::endl;
	  p_channels.push_back(reinterpret_cast<int>((&*it - &*polls.begin()))); // The channel id returned by the channelManager
	  it->revents = 0;
	  result -= 1;
	  if (result == 0) {
	    std::clog << "channelManager::pollChannels: exit 'for' loop" << std::endl;
	    break; // Exit 'for' loop
	  }
	}
      } // End of 'for' statement
    } else if (result == 0) {
      // Nothing to do
    } else {
      std::cerr << "channelManager::pollChannels: " << strerror(errno) << std::endl;
    }

    _pollingInProgress = false;

    //    std::clog << "<<< channelManager::pollChannels: 0" << std::endl;
    return 0;
  } // End of method pollChannels

  int channelManager::pollChannels(const int p_timeout, std::vector<int> & p_channelsToPoll, std::vector<int> & p_channels) {
    //    std::clog << ">>> channelManager::pollChannels (1)" << std::endl;

    // Sanity checks
    p_channels.clear();
    if (_pollingInProgress || (p_channelsToPoll.size()) == 0) {
      std::cerr << "channelManager::pollChannels (1): Wrong parameters" << std::endl;
      return -1;
    }

    std::list<struct pollfd> polls;
    for (auto it = p_channelsToPoll.begin(); it != p_channelsToPoll.end(); ++it) {
      struct pollfd p = { 0 };
      p.fd = getChannel(*it).getFd();
      p.events = POLLIN | POLLPRI | POLLHUP;
      p.revents = 0;
      //      std::clog << "channelManager::pollChannels (1): fd=" << (int)p.fd << std::endl;

      polls.push_back(p);
    } // End of 'for' statement
    if (polls.size() == 0) {
      std::cerr << "channelManager::pollChannels (1): Wrong file descriptors list" << std::endl;
      return -1;
    }
    _pollingInProgress = true;

    int result = ::poll(&*polls.begin(), polls.size(), p_timeout);
    if (result > 0) {
      //      std::clog << "channelManager::pollChannels (1): fd=" << polls.front().fd << " - result=" << result << " Fill p_channels" << std::endl;
      // Fill p_channels
      for (std::list<struct pollfd>::iterator it = polls.begin(); it != polls.end(); ++it) {
	if (it->revents & POLLIN) {
	  std::clog << "channelManager::pollChannels (1): POLLIN event on descriptor " << (int)it->fd << " / index: " << (int)(&*it - &*polls.begin())<< std::endl;
	  p_channels.push_back(p_channelsToPoll[reinterpret_cast<int>((&*it - &*polls.begin()))]); // The channel id returned by the channelManager
	  it->revents = 0;
	  result -= 1;
	  if (result == 0) {
	    std::clog << "channelManager::pollChannels (1): exit 'for' loop" << std::endl;
	    break; // Exit 'for' loop
	  }
	}
      } // End of 'for' statement
    } else if (result == 0) {
      // Nothing to do
    } else {
      std::cerr << "channelManager::pollChannels (1): " << strerror(errno) << std::endl;
    }

    _pollingInProgress = false;

    //    std::clog << "<<< channelManager::pollChannels (1): 0" << std::endl;
    return 0;
  }

  int channelManager::createChannel(const abstractChannel::ChannelType p_channelType, const socketAddress & p_host) {
    // Create new instance
    abstractChannel *channel = NULL;
    switch (p_channelType) {
    case abstractChannel::udp:
      channel = new udpChannel(p_host);
      break;
    case abstractChannel::tcp:
      channel = new tcpChannel(p_host);
      break;
    case abstractChannel::sctp:
      // TODO
      break;
    case abstractChannel::raw:
      channel = new rawChannel(p_host);
      break;
    default:
      // TODO
      break;
    } // End of 'switch' statement
      // Sanity check
    if (channel == NULL) {
      std::cerr << "channelManager::createChannel (1): Failed to create device instance" << std::endl;
      return -1;
    }

    // Store it into the map
    int idx = channelManager::_counter++;
    _channels.insert(std::pair<int, abstractChannel *>(idx, channel));

    // Update the poll list
    struct pollfd p = { 0 };
    p.fd = channel->getFd();
    p.events = POLLIN | POLLPRI | POLLHUP;
    p.revents = 0;
    std::clog << "channelManager::createChannel (1): fd=" << (int)p.fd << std::endl;
    _polls.insert(std::pair<const int, struct pollfd>(idx, p));

    return idx;
  } // End of method createChannel
    
  int channelManager::createChannel(const abstractChannel::ChannelType p_channelType, const socketAddress & p_host, const socketAddress & p_remote) {
    // Create new instance
    abstractChannel *channel = NULL;
    switch (p_channelType) {
    case abstractChannel::udp:
      channel = new udpChannel(p_host, p_remote);
      break;
    case abstractChannel::tcp:
      channel = new tcpChannel(p_host);
      break;
    case abstractChannel::sctp:
      // TODO
      break;
    case abstractChannel::raw:
      channel = new rawChannel(p_host);
      break;
    default:
      // TODO
      break;
    } // End of 'switch' statement
      // Sanity check
    if (channel == NULL) {
      std::cerr << "channelManager::createChannel (2): Failed to create device instance" << std::endl;
      return -1;
    }

    // Store it into the map
    int idx = channelManager::_counter++;
    _channels.insert(std::pair<int, abstractChannel *>(idx, channel));

    // Update the poll list
    struct pollfd p = { 0 };
    p.fd = channel->getFd();
    p.events = POLLIN | POLLPRI | POLLHUP;
    p.revents = 0;
    std::clog << "channelManager::createChannel (2): fd=" << (int)p.fd << std::endl;
    _polls.insert(std::pair<const int, struct pollfd>(idx, p));

    return idx;
  } // End of method createChannel
    
  int channelManager::createChannel(const abstractChannel::ChannelType p_channelType, const std::string & p_device, const int p_speed) {
    // Sanity check
    if (p_device.empty() || (p_channelType != abstractChannel::serial)) {
      std::cerr << "channelManager::createChannel: Wrong parameters" << std::endl;
      return -1;
    }

    // Create new instance
    abstractChannel *channel = new serialChannel(p_device, p_speed);
    // Sanity check
    if (channel == NULL) {
      std::cerr << "channelManager::createChannel (3): Failed to create device instance" << std::endl;
      return -1;
    }

    // Store it into the map
    int idx = channelManager::_counter++;
    _channels.insert(std::pair<int, abstractChannel *>(idx, channel));
    std::clog << "channelManager::createChannel (3): fd= " << (int)(channel->getFd()) << std::endl;

    // Update the poll list
    struct pollfd p = { 0 };
    p.fd = channel->getFd();
    p.events = POLLIN | POLLPRI | POLLHUP;
    p.revents = 0;
    _polls.insert(std::pair<const int, struct pollfd>(idx, p));

    return idx;
  } // End of method createChannel

  int channelManager::createChannel(const int p_socket, const network::socketAddress & p_host, const network::socketAddress & p_remote) {
    abstractChannel *channel = new  tcpChannel(p_socket, p_host, p_remote);
    
    // Sanity check
    if (channel == NULL) {
      std::cerr << "channelManager::createChannel (4): Failed to create device instance" << std::endl;
      return -1;
    }
    
    // Store it into the map
    int idx = channelManager::_counter++;
    _channels.insert(std::pair<int, abstractChannel *>(idx, channel));
    
    // Update the poll list
    struct pollfd p = { 0 };
    p.fd = channel->getFd();
    p.events = POLLIN | POLLPRI | POLLHUP;
    p.revents = 0;
    std::clog << "channelManager::createChannel (4): fd=" << (int)p.fd << std::endl;
    _polls.insert(std::pair<const int, struct pollfd>(idx, p));

    return idx;
  } // End of method createChannel
  
  int channelManager::removeChannel(const int p_channel) {
    // Sanity check
    if (_channels.find(p_channel) == _channels.end()) {
      std::cerr << "channelManager::removeChannel: Unknown channel #" << p_channel << std::endl;
      return -1;
    }

    // Remove channel from the poll list
    std::map<const int, struct pollfd>::iterator del = _polls.find(p_channel);
    if (del != _polls.end()) {
      _polls.erase(del);
    }

    // Remove channel from the map
    abstractChannel *c = _channels.at(p_channel);
    _channels.erase(p_channel);

    // Delete channel
    delete c;

    return 0;
  } // End of method createChannel

} // End of namespace beagleboneComm
