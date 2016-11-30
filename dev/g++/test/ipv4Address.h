#if !defined(__IPV4ADDRESS_H__)
#define __IPV4ADDRESS_H__

#include <netinet/in.h> // Used for struct in_addr
#include <string>

namespace raspberryComm {

  namespace network {

    class ipv4Address {
      struct in_addr _inaddr;

    public:
      ipv4Address(const std::string & p_address);
      ipv4Address(const struct in_addr * p_inaddr);
      ipv4Address(const ipv4Address & p_address);
      virtual ~ipv4Address() { }

      inline const void * addr() const { return (const void *)&_inaddr; };
      inline size_t length() const { return sizeof(struct in_addr); };

      std::string toString() const;

      static ipv4Address * parse(const std::string& p_address);
      static bool tryParse(const std::string& p_address, ipv4Address & p_result);
    }; // End of class ipv4Address

  } // End of namespace network

} // End of namespace raspberryComm

#endif // __IPV4ADDRESS_H__
