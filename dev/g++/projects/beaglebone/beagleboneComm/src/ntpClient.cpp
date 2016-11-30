#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <sys/time.h> // Used settimeofday
#include <math.h> // Used abs

#include "ntpClient.h"

#include "socketAddress.h"

/** The reverse of the above, needed if we want to set our microsecond clock (via clock_settime) based on the incoming time in NTP format. Basically exact. */
#define USEC(x) ( ( (x) >> 12 ) - 759 * ( ( ( (x) >> 10 ) + 32768 ) >> 16 ) )
/** NTP server port */
#define NTP_PORT 123

namespace beagleboneComm {

  namespace network {

    ntpClient::ntpClient(const std::string & p_ntpServer, const std::string & p_hostAddr, const int p_hostPort) : _udp(nullptr), _remoteAddr(p_ntpServer, NTP_PORT), _hostAddr(p_hostAddr, p_hostPort), _ntpSend(48, 0x00), _ntpRecv(512, 0x00) {
      
      _ntpSend[0] = 0x23; // NTP flags/version = NTPv4 - All other byte set to 0
      _ntpSend[1] = 0x03; // Stratum = 3;
      _ntpSend[2] = 0x09; // poll=9s
      _ntpSend[3] = 0xEC;  // Peer Clock Precision
      // 8 bytes of zero for Root Delay & Root Dispersion
      // Reference-ID: _remoteAddr
      std::vector<unsigned char> refId(_remoteAddr.toNumeric());
      _ntpSend[12] = refId[0];
      _ntpSend[13] = refId[1];
      _ntpSend[14] = refId[2];
      _ntpSend[15] = refId[3];

      _udp.reset(new udpChannel(_hostAddr, _remoteAddr));
      // Do not call udpChannel::connect
    } // End of Constructor

    ntpClient::~ntpClient() {
      std::clog << ">>> ntpClient::~ntpClient" << std::endl;

      _udp.reset(nullptr);
    } // End of Destructor

    int ntpClient::setTime() {
      // Send the NTP request
      if (_udp->write(_ntpSend) == -1) {
	std::ostringstream os;
	os << "ntpClient::setTime: Failed to send data: " << std::strerror(errno) << std::endl;
	return -1;
      }
      // Wait for the response
      if (_udp->read(_ntpRecv) == -1) {
	std::ostringstream os;
	os << "ntpClient::setTime: Failed awaiting data: " << std::strerror(errno) << std::endl;
	return -1;
      }

      // Get transmit time
      time_t ctime = time(NULL);
      time_t tmit = ntohl(* reinterpret_cast<unsigned int *>(_ntpRecv.data() + 32) );
      time_t fine = ntohl(* reinterpret_cast<unsigned int *>(_ntpRecv.data() + 32 + sizeof(unsigned int)));
      // Convert into UNIX time
      tmit -= 2208988800U; // 1970 - 1900 in seconds
      std::clog << "ntpClient::setTime: received time is " << tmit << " - " << std::ctime(&tmit) << std::endl;
      std::clog << "ntpClient::setTime: system time is   " << ctime << " - " << std::ctime(&ctime) << std::endl;
      std::clog << "ntpClient::setTime: received time is " << "diffTime: " << tmit - ctime << std::endl;

      if (abs(tmit - ctime) > 10) {
	// Update system time
	struct timeval tv_set;
	// It would be even better to subtract half the slop
	tv_set.tv_sec  = tmit;
	// Divide xmttime.fine by 4294.967296
	tv_set.tv_usec = USEC(fine);
	// Update system time
	if (settimeofday(&tv_set, NULL) < 0) {
	  std::cerr << "ntpClient::setTime: Failed to update system time: " << strerror(errno)  << std::endl;
	  return -1;
	}

	return 0;
      }

      return -2;
    }
    
  } // End of namespace network
  
} // End of namespace beagleboneComm
  
