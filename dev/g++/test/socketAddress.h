#if !defined(__SOCKETADDRESS_H__)
#define __SOCKETADDRESS_H__

#include <string>

#include "ipv4Address.h"
//#include "ipv6Address.h"

namespace raspberryComm {

  namespace network {

    class socketAddress {
      struct sockaddr_in _sockaddr;
      ipv4Address *_host;

    public:
      /**
       * @desc
       * @param p_host Host address. "0.0.0.0" for server (INADDR_ANY)
       * @param p_port Port number.
       */
      socketAddress(const std::string p_host, const int p_port = -1);
      socketAddress(const socketAddress & p_host);
      virtual ~socketAddress();

      const ipv4Address & host() const { return *_host; };
      const struct sockaddr * addr() const { return reinterpret_cast<const struct sockaddr *>(&_sockaddr); };
      inline size_t length() const { return sizeof(struct sockaddr); };

      std::string toString() const;

      // TODO socketAddress * tryParse(const std::string p_host);

    }; // End of class socketAddress

  } // End of namespace network

} // End of namespace raspberryComm

#endif // __SOCKETADDRESS_H__
