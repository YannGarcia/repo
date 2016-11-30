#if !defined(__SOCKET_H__)
#define __SOCKET_H__

#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "ipv4Address.h"
// TODO #include "ipv6Address.h"
#include "socketAddress.h"

namespace raspberryComm {

  namespace network {

    class socket {
    public:
      enum ChannelFamily : unsigned char {
	udp = 0x00,
	tcp = 0x01,
	sctp = 0x02,
	raw = 0x03
      };

    private:
      socket::ChannelFamily _family;
      int _socket;
      struct sockaddr_in _host;
      struct sockaddr_in _remote;
 
    public:
      socket(const ipv4Address & p_host, const unsigned short p_port, const ChannelFamily p_family = socket::udp);
      socket(const socketAddress & p_host, const ChannelFamily p_family = socket::udp);
      socket(const socketAddress & p_host, const socketAddress & p_remote, const ChannelFamily p_family = socket::udp);
      ~socket();

      int connect();
      int bind();
      int listen(const int backlog = 0);

      int send(const std::vector<unsigned char> & p_buffer);
      int receive(std::vector<unsigned char> & p_buffer, struct sockaddr_in * p_from);
      int receive(std::vector<unsigned char> & p_buffer);
      int receive(unsigned char *p_buffer, const int p_length);

      inline void setNoDelay(bool flag) { if (_socket != -1) { setOption(IPPROTO_TCP, TCP_NODELAY, (flag == true) ? 1 : 0); } };
      inline void setBlocking(bool flag) { };

      inline void setOption(int level, int option, int value) { setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (void *)&value, sizeof(value)); };
      inline void setOption(int level, int option, unsigned int value) { };
      inline void setOption(int level, int option, unsigned char value) { };

    private:
      int sendTo(const std::vector<unsigned char> & p_buffer);
      int receiveFrom(std::vector<unsigned char> & p_buffer, struct sockaddr_in * p_from);
      int receiveFrom(unsigned char *p_buffer, const int p_length, struct sockaddr_in * p_from);      
    }; // End of class socket

  } // End of namespace network

} // End of namespace raspberryComm

#endif // __SOCKET_H__

