#include <iostream>
#include <iomanip>
#include <sstream> // std::ostringstream
#include <stdexcept> // std::out_of_range
#include <cstring> // Used std::strerror

#include <unistd.h> // Used for usleep and close

#include "serialChannel.h"

namespace beagleboneComm {

  namespace serial {

    serialChannel::serialChannel(const std::string & p_device, const int p_speed) {
      std::clog << ">>> serialChannel::serialChannel: " << p_device << ", " << p_speed << std::endl;

      if ((_fd = ::serialOpen(p_device.c_str(), p_speed)) < 0) {
	_fd = -1;
	std::ostringstream os;
	os << "serialChannel::serialChannel: Wrong parameters: " << std::strerror(errno) << std::endl;
	throw std::out_of_range(os.str()); 
      }
      
      std::clog << "<<< serialChannel::serialChannel: " << (int)_fd << " - " << (int)(this->getFd()) << std::endl;
    } // End of ctor

    serialChannel::~serialChannel() {
      std::clog << ">>> serialChannel::~serialChannel: " << (int)_fd << std::endl;

      if (_fd != -1) {
	::serialClose(_fd);
	_fd = -1;
      }
    }

    int serialChannel::connect() {
      // Nothing to do
      return 0;
    }

    int serialChannel::write(const std::string & p_string) {
      //      std::clog << "<<< serialChannel::write: " << p_string << " - " << (int)(this->getFd()) << std::endl;
      
      if (p_string.length() == 0) {
	return 0;
      }
      
      ::serialPuts(_fd, p_string.c_str());
      return p_string.length();
    }

    int serialChannel::write(const std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }
      ::serialPuts(_fd, reinterpret_cast<const char *>(p_buffer.data()));

      return p_buffer.size();
    }

    int serialChannel::read(std::string & p_line) {
      p_line.clear();
      
      char c = static_cast<char>(::serialGetchar(_fd) & 0xff);
      while (c != '\r') {
	p_line += c;
	c = static_cast<char>(::serialGetchar(_fd) & 0xff);
      } // End of 'while' statement
      if (c == '\r') p_line += c;

      return p_line.length();
    }

    int serialChannel::read(std::vector<unsigned char> & p_buffer) {
      if (::serialDataAvail(_fd) <= 0) {
	return -1;
      }

      int count = ::read(_fd, p_buffer.data(), p_buffer.size());
      return count;
    }

    char serialChannel::read() {
      /*if (::serialDataAvail(_fd) <= 0) {
	return '\00';
	}*/

      char c = static_cast<char>(::serialGetchar(_fd) & 0xff);
      std::clog << "serialChannel::read: read '" << c << "'" << std::endl;
      return c;
      //      return static_cast<char>(::serialGetchar(_fd) & 0xff);
    }

  } // End of namespace serial

} // End of namespace beagleboneComm
