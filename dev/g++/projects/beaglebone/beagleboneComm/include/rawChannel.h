#if !defined(__RAWCHANNEL_H__)
#define __RAWCHANNEL_H__

#include <stdexcept>

#include "abstractChannel.h"
#include "socketAddress.h"

namespace beagleboneComm {

  namespace network {

    class rawChannel : public abstractChannel {

    public:
      rawChannel(const socketAddress & p_host);
      rawChannel(const socketAddress & p_host, const socketAddress & p_remote);
      virtual ~rawChannel();

      int connect();

      int disconnect() { return -1; };

      int write(const std::string & p_string);
      int write(const std::vector<unsigned char> & p_buffer);
      int read(std::vector<unsigned char> & p_buffer);
      char read();
      inline int dataAvailable() { throw std::runtime_error("Not implemented yet"); };
    }; // End of class rawChannel

  } // End of namespace network

} // End of namespace beagleboneComm

#endif // __RAWCHANNEL_H__
