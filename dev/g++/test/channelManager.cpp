#include <cstring> // Used for memset

#include "channelManager.h"
#include "udpChannel.h"
#include "tcpChannel.h"
#include "rawChannel.h"
#include "serialChannel.h"

namespace raspberryComm {

  using namespace network;
  using namespace serial;

  channelManager *channelManager::_instance = new channelManager();
  int channelManager::_counter = 0;

  channelManager::channelManager() : _channels() {
    // Set default timeout for 'select'
    ::memset((void *)&_selectTimeOut, 0x00, sizeof(_selectTimeOut));
    _selectTimeOut.tv_usec = 500; // 500ms timeout for select

    _maxFd = 0;
    FD_ZERO(&_readFs);
    FD_ZERO(&_writeFs);
  } // End of constructor

  channelManager::~channelManager() {
    // TODO Properly deregister channels if require
  } // End of destructor

  void channelManager::pollChannels() {

    int result = select(_maxFd, &_readFs, NULL, NULL, &_selectTimeOut);
    if (result < 0) {
      std::cerr << "channelManager::pollChannels: " << std::strerror(errno) << std::endl;
    } else if (result == 0) { // Timeout
    } else {
      // TODO To be continued
    }
  } // End of method pollChannels

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
      return -1;
    }

    // Store it into the map
    int idx = channelManager::_counter++;
    _channels.insert(std::pair<int, abstractChannel *>(idx, channel));

    // Update the poll list
    FD_SET(idx, &_readFs);
    FD_SET(idx, &_writeFs);
    _maxFd = channelManager::_counter++;

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
      return -1;
    }

    // Store it into the map
    int idx = channelManager::_counter++;
    _channels.insert(std::pair<int, abstractChannel *>(idx, channel));

    // Update the poll list
    FD_SET(idx, &_readFs);
    FD_SET(idx, &_writeFs);
    _maxFd = channelManager::_counter++;

    return idx;
  } // End of method createChannel
    
  int channelManager::createChannel(const abstractChannel::ChannelType p_channelType, const std::string & p_device, const int p_speed) {
    // Sanity check
    if (p_device.empty()) {
      std::cerr << "channelManager::createChannel: Wrong parameters" << std::endl;
      return -1;
    }

    // Create new instance
    abstractChannel *channel = new serialChannel(p_device, p_speed);
    // Sanity check
    if (channel == NULL) {
      return -1;
    }

    // Store it into the map
    int idx = channelManager::_counter++;
    _channels.insert(std::pair<int, abstractChannel *>(idx, channel));

    // Update the poll list
    FD_SET(idx, &_readFs);
    FD_SET(idx, &_writeFs);
    _maxFd = channelManager::_counter++;

    return idx;
  } // End of method createChannel
    
  int channelManager::removeChannel(const int p_channel) {
    // Sanity check
    if (_channels.find(p_channel) == _channels.end()) {
      return -1;
    }

    // Remove channel from the poll list
    FD_CLR(p_channel, &_readFs);
    FD_CLR(p_channel, &_writeFs);

    // Remove channel from the map
    abstractChannel *c = _channels.at(p_channel);
    _channels.erase(p_channel);

    // Delete channel
    delete c;

    return 0;
  } // End of method createChannel

} // End of namespace raspberryComm
