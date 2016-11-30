#include <cstring>

#include "udpChannel.h"

namespace raspberryComm {

  namespace network {

    udpChannel::udpChannel(const socketAddress & p_host) {
      if ((_socket = new socket(p_host)) == NULL) { // Default ChannelFamily is UDP
	std::cerr << "udpChannel::connect: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind();
    }

    udpChannel::udpChannel(const socketAddress & p_host, const socketAddress & p_remote) {
      if ((_socket = new socket(p_host, p_remote)) == NULL) { // Default ChannelFamily is UDP
	std::cerr << "udpChannel::connect: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind();
    }

    udpChannel::~udpChannel() {
      std::clog << "udpChannel::~udpChannel" << std::endl;
      // Socket deleted by abstractChannel dtor
    }

    int udpChannel::connect() {
      return _socket->connect();
    }

    int udpChannel::write(const std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->send(p_buffer);
    }

    int udpChannel::read(std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->receive(p_buffer);
    }

    char udpChannel::read() {
      unsigned char buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], 1) < 0) {
	return '\00';
      }
      return (char)buffer[0];
    }

  } // End of namespace network

} // End of namespace raspberryComm
