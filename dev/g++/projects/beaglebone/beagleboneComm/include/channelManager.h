#if !defined(__CHANNELMANAGER_H__)
#define __CHANNELMANAGER_H__

#include <string>
#include <unordered_map>
#include <map>
#include <stdexcept> // std::out_of_range
#include <memory> // Used for unique_ptr

#include <poll.h>

#include "abstractChannel.h"

namespace beagleboneComm {

  class channelManager {
    
    static int _counter;
    std::unordered_map<int, abstractChannel *> _channels; // TODO Use a simple list ???
    std::map<const int, struct pollfd> _polls;
    static std::unique_ptr<channelManager> _instance;
    bool _pollingInProgress;

  public:
    inline static channelManager & getInstance() { return *_instance; };

  private:
    channelManager();

  public:
    ~channelManager();

    int createChannel(const abstractChannel::ChannelType p_channelType, const network::socketAddress & p_host);
    int createChannel(const abstractChannel::ChannelType p_channelType, const network::socketAddress & p_host, const network::socketAddress & p_remote);
    int createChannel(const int p_socket, const network::socketAddress & p_host, const network::socketAddress & p_remote);
    int createChannel(const abstractChannel::ChannelType p_channelType, const std::string & p_device, const int p_speed);

    int removeChannel(const int p_channel);
    int pollChannels(const int p_timeout, std::vector<int> & p_channels);
    int pollChannels(const int p_timeout, std::vector<int> & p_channelsToPoll, std::vector<int> & p_channels);

    inline abstractChannel & getChannel(const int p_channel) { if (_channels.find(p_channel) == _channels.end()) throw std::out_of_range("Wrong channel identifier" ); return *_channels.at(p_channel); };

  }; // End of class channelManager

} // End of namespace beagleboneComm

using namespace beagleboneComm;

#endif //__CHANNELMANAGER_H__
