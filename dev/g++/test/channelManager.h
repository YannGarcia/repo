#if !defined(__CHANNELMANAGER_H__)
#define __CHANNELMANAGER_H__

#include <string>
#include <unordered_map>
#include <stdexcept> // std::out_of_range

#include <sys/time.h>
#include <unistd.h>

#include "abstractChannel.h"

namespace raspberryComm {

  class channelManager {
    
    static int _counter;
    std::unordered_map<int, abstractChannel *> _channels; // TODO Use a simple list ???
    int _maxFd;
    fd_set _readFs;
    fd_set _writeFs;
    struct timeval _selectTimeOut;

  public:
    static channelManager *_instance;

    inline static channelManager & getInstance() { return *_instance; };
  private:
    channelManager();
    ~channelManager();

  public:
    int createChannel(const abstractChannel::ChannelType p_channelType, const network::socketAddress & p_host);
    int createChannel(const abstractChannel::ChannelType p_channelType, const network::socketAddress & p_host, const network::socketAddress & p_remote);
    int createChannel(const abstractChannel::ChannelType p_channelType, const std::string & p_device, const int p_speed);

    int removeChannel(const int p_channel);
    void pollChannels();

    inline abstractChannel & getChannel(const int p_channel) { if (_channels.find(p_channel) == _channels.end()) throw std::out_of_range("Wrong channel identifier" ); return *_channels.at(p_channel); };

  }; // End of class channelManager

} // End of namespace raspberryComm

#endif //__CHANNELMANAGER_H__
