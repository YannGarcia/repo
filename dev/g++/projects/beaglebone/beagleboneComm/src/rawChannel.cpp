#include <cstring>
#include <stdexcept> // std::out_of_range

#include "rawChannel.h"

#include "converter.h"

namespace beagleboneComm {

  namespace network {

    rawChannel::rawChannel(const socketAddress & p_host) {
      if ((_socket = new socket(p_host, socket::raw)) == NULL) {
	std::cerr << "rawChannel::connect: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind();
    }

    rawChannel::rawChannel(const socketAddress & p_host, const socketAddress & p_remote) {
      if ((_socket = new socket(p_host, p_remote, socket::raw)) == NULL) {
	std::cerr << "rawChannel::connect: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind();
    }

    rawChannel::~rawChannel() {
      std::clog << "rawChannel::~rawChannel" << std::endl;
      // Socket deleted by abstractChannel dtor
    }

    int rawChannel::connect() {
      return _socket->connect();
    }

    int rawChannel::write(const std::string & p_string) {
      if (p_string.length() == 0) {
	return 0;
      }
      
      return _socket->send(converter::getInstance().string2bytes(p_string));
    }

    int rawChannel::write(const std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->send(p_buffer);
    }

    int rawChannel::read(std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->receive(p_buffer);
    }

    char rawChannel::read() {
      int length = 1;
      unsigned char buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], &length) < 0) {
	return '\00';
      }
      return (char)buffer[0];
    }

  } // End of namespace network

} // End of namespace beagleboneComm
