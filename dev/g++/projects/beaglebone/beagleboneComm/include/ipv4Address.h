#if !defined(__IPV4ADDRESS_H__)
#define __IPV4ADDRESS_H__

#include <sstream> // Used istringstream

#include <netinet/in.h> // Used for struct in_addr
#include <string>
#include <vector>

namespace beagleboneComm {

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

      inline std::string toString() const {
	std::string ip(inet_ntoa(_inaddr));
	return ip;
      };
      inline std::vector<unsigned char> toNumeric() const {
	std::vector<unsigned char> buffer(4, 0x00);
	std::istringstream s(inet_ntoa(_inaddr));
	char dot; // Used to receive dot character
	s >> buffer.data() >> dot >> (buffer.data() + 1) >> dot >> (buffer.data() + 2) >> dot >> (buffer.data() + 3);
	return buffer;
      };
      
      static ipv4Address * parse(const std::string& p_address);
      static bool tryParse(const std::string& p_address, ipv4Address & p_result);
    }; // End of class ipv4Address

  } // End of namespace network

} // End of namespace beagleboneComm

#endif // __IPV4ADDRESS_H__
