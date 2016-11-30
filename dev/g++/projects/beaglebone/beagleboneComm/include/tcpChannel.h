#if !defined(__TCPCHANNEL_H__)
#define __TCPCHANNEL_H__

#include <stdexcept>

#include "abstractChannel.h"
#include "socketAddress.h"

namespace beagleboneComm {

  namespace network {

    class tcpChannel : public abstractChannel {

    public:
      tcpChannel(const socketAddress & p_host);
      tcpChannel(const socketAddress & p_host, const socketAddress & p_remote);
      tcpChannel(const int p_socket, const socketAddress & p_host, const socketAddress & p_remote);
      virtual ~tcpChannel();

      int connect();
      int acceptConnection();

      int disconnect() { return -1; };

      int write(const std::string & p_string);
      int write(const std::vector<unsigned char> & p_buffer);
      int read(std::vector<unsigned char> & p_buffer);
      char read();
      inline int dataAvailable() { throw std::runtime_error("Not implemented yet"); };
    }; // End of class tcpChannel

  } // End of namespace network

} // End of namespace beagleboneComm

#endif // __TCPCHANNEL_H__
