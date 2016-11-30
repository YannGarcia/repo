#if !defined(__TCPCHANNEL_H__)
#define __TCPCHANNEL_H__

#include "abstractChannel.h"
#include "socketAddress.h"

namespace raspberryComm {

  namespace network {

    class tcpChannel : public abstractChannel {

    public:
      tcpChannel(const socketAddress & p_host);
      tcpChannel(const socketAddress & p_host, const socketAddress & p_remote);
      virtual ~tcpChannel();

      int connect();

      int disconnect() { return -1; };

      int write(const std::vector<unsigned char> & p_buffer);
      int read(std::vector<unsigned char> & p_buffer);
      char read();

    }; // End of class tcpChannel

  } // End of namespace network

} // End of namespace raspberryComm

#endif // __TCPCHANNEL_H__
