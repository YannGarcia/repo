#include <iostream>
#include <iomanip>
#include <sstream> // std::ostringstream
#include <stdexcept> // std::out_of_range
#include <cstring> // Used std::strerror

#include "serialChannel.h"

#include "wiringPi.h"
#include "wiringSerial.h"

namespace raspberryComm {

  namespace serial {

    serialChannel::serialChannel(const std::string & p_device, const int speed) {
      if ((_fd = ::serialOpen(p_device.c_str(), speed)) < 0) {
	std::ostringstream os;
	os << "serialChannel::serialChannel: Wrong parameters: " << std::strerror(errno) << std::endl;
	throw std::out_of_range(os.str()); 
      }
      
    } // End of ctor

    serialChannel::~serialChannel() {
      if (!(_fd < 0)) {
	::serialClose(_fd);
      }
    }

    int serialChannel::connect() {
      // Nothing to do
      return 0;
    }

    int serialChannel::write(const std::vector<unsigned char> & p_buffer) {
      return -1;
    }

    int serialChannel::read(std::vector<unsigned char> & p_buffer) {
      if (::serialDataAvail(_fd) <= 0) {
	return -1;
      }

      int count = ::read(_fd, p_buffer.data(), p_buffer.size());
      return count;
    }

    char serialChannel::read() {
      /*      if (::serialDataAvail(_fd) <= 0) {
	std::clog << "no data av." << std::endl;
	return '\00';
	}*/

      char c = static_cast<char>(::serialGetchar(_fd) & 0xff);
      //      std::clog << "serialChannel::read: read '" << c << "'" << std::endl;
      return c;
    }

  } // End of namespace serial

} // End of namespace raspberryComm
