/**
 * @file      channel_manager.h
 * @brief     Header file for communication channel factory.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <stdexcept> // std::out_of_range
#include <memory> // Used for unique_ptr

#include <poll.h>

#include "abstract_channel.h"

namespace comm {
  
  /**
   * @class channel_manager
   * @brief 
   */
  class channel_manager {
    
    static uint32_t _counter;                               /** Created channel counter */
    std::map<const uint32_t, abstract_channel *> _channels; /** abstract_channel instances */
    std::map<const uint32_t, struct pollfd> _polls;         /** Polling map on created channels */
    bool _polling_in_progress;                              /** Polling progress flag */
    static std::unique_ptr<channel_manager> _instance;      /** Unique instance of this class (singleton) */

  public:
    inline static channel_manager & get_instance() { return *_instance; };

  private:
    channel_manager();

  public:
    ~channel_manager();

    const int32_t create_channel(const channel_type p_channel_type, const comm::network::socket_address & p_host);
    const int32_t create_channel(const channel_type p_channel_type, const comm::network::socket_address & p_host, const comm::network::socket_address & p_remote);
    const int32_t create_channel(const int32_t p_socket, const comm::network::socket_address & p_host, const comm::network::socket_address & p_remote);

    const int32_t remove_channel(const uint32_t p_channel);
    const int32_t poll_channels(const uint32_t p_timeout, std::vector<uint32_t> & p_channels);
    const int32_t poll_channels(const uint32_t p_timeout, std::vector<uint32_t> & p_channelsToPoll, std::vector<uint32_t> & p_channels);

    inline abstract_channel & get_channel(const uint32_t p_channel) const { if (_channels.find(p_channel) == _channels.end()) throw std::out_of_range("Wrong channel identifier" ); return *_channels.at(p_channel); };
    
  private:
    const uint32_t initialise_channel(abstract_channel * p_channel);
    
  }; // End of class channel_manager

} // End of namespace comm

using namespace comm;
