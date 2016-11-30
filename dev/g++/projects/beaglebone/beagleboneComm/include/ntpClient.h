#if !defined(__NTPCLIENT_H__)
#define __NTPCLIENT_H__

#include <memory>

#include "udpChannel.h"

namespace beagleboneComm {

  namespace network {

    class ntpClient {
      std::unique_ptr<udpChannel> _udp;
      socketAddress _remoteAddr;
      socketAddress _hostAddr;
      std::vector<unsigned char> _ntpSend;
      std::vector<unsigned char> _ntpRecv;

    public:
      ntpClient(const std::string & p_ntpServer, const std::string & p_hostAddr, const int p_hostPort = 8888);
      virtual ~ntpClient();

      /**
       * @desc Request time to the NTP server and update the system time if required
       * @return 0 if the system time was updated, -2 if time difference is less tha 9 seconds, -1 on error
       */
      int setTime();
    }; // End of class ntpClient

  } // End of namespace network

} // End of namespace beagleboneComm

using namespace beagleboneComm;

#endif // __NTPCLIENT_H__
