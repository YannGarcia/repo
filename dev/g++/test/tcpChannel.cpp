#include <cstring>

#include "tcpChannel.h"

namespace raspberryComm {

  namespace network {

    tcpChannel::tcpChannel(const socketAddress & p_host) {
      if ((_socket = new socket(p_host, socket::tcp)) == NULL) { 
	std::cerr << "tcpChannel::connect: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind();
    }

    tcpChannel::tcpChannel(const socketAddress & p_host, const socketAddress & p_remote) {
      if ((_socket = new socket(p_host, p_remote, socket::tcp)) == NULL) { 
	std::cerr << "tcpChannel::connect: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind();
    }

    tcpChannel::~tcpChannel() {
      std::clog << "tcpChannel::~tcpChannel" << std::endl;
      // Socket deleted by abstractChannel dtor
    }

    int tcpChannel::connect() {
      return _socket->connect();
    }

    int tcpChannel::write(const std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->send(p_buffer);
    }

    int tcpChannel::read(std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->receive(p_buffer);
    }

    char tcpChannel::read() {
      unsigned char buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], 1) < 0) {
	return '\00';
      }
      return (char)buffer[0];
    }

  } // End of namespace network

} // End of namespace raspberryComm
