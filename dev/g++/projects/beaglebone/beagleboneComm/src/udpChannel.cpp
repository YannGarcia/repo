#include <cstring>

#include "udpChannel.h"

#include "converter.h"

namespace beagleboneComm {

  namespace network {

    udpChannel::udpChannel(const socketAddress & p_host) { // Constructorfor an UDP client
      if ((_socket = new socket(p_host)) == NULL) { // Default ChannelFamily is UDP
	std::cerr << "udpChannel::udpChannel: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      std::clog << "udpChannel::udpChannel: fd=" << _socket->getFd() << std::endl;
      _socket->bind();
    }

    udpChannel::udpChannel(const socketAddress & p_host, const socketAddress & p_remote) { // Constructorfor an UDP server
      if ((_socket = new socket(p_host, p_remote)) == NULL) { // Default ChannelFamily is UDP
	std::cerr << "udpChannel::udpChannel: " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      std::clog << "udpChannel::udpChannel: fd=" << _socket->getFd() << std::endl;
      _socket->bind();
    }

    udpChannel::~udpChannel() {
      std::clog << "udpChannel::~udpChannel" << std::endl;
      // Socket deleted by abstractChannel dtor
    }

    int udpChannel::connect() {
      return _socket->connect();
    }

    int udpChannel::write(const std::string & p_string) {
      if (p_string.length() == 0) {
	return 0;
      }
      
      return _socket->send(converter::getInstance().string2bytes(p_string));
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
      int length = 1;
      unsigned char buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], &length) < 0) {
	return '\00';
      }
      return (char)buffer[0];
    }

  } // End of namespace network

} // End of namespace beagleboneComm
