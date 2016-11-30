#if !defined(__UDPCHANNEL_H__)
#define __UDPCHANNEL_H__

#include <stdexcept>

#include "abstractChannel.h"
#include "socketAddress.h"

namespace beagleboneComm {

  namespace network {

    class udpChannel : public abstractChannel {

    public:
      udpChannel(const socketAddress & p_host);
      udpChannel(const socketAddress & p_host, const socketAddress & p_remote);
      virtual ~udpChannel();

      int connect();

      int disconnect() { return -1; };

      int write(const std::string & p_string);
      int write(const std::vector<unsigned char> & p_buffer);
      int read(std::vector<unsigned char> & p_buffer);
      char read();
      inline int dataAvailable() { throw std::runtime_error("Not implemented yet"); };
    }; // End of class udpChannel

  } // End of namespace network

} // End of namespace beagleboneComm

#endif // __UDPCHANNEL_H__
