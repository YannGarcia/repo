#include <cstring>

#include "tcpChannel.h"

#include "converter.h"

namespace beagleboneComm {

  namespace network {

    tcpChannel::tcpChannel(const socketAddress & p_host) { // Constructor for a TCP client
      std::clog << ">>> tcpChannel::tcpChannel (1)" << std::endl;

      if ((_socket = new socket(p_host, socket::tcp)) == NULL) { 
	std::cerr << "tcpChannel::connect(1): " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind();
      _socket->listen();
    }

    tcpChannel::tcpChannel(const socketAddress & p_host, const socketAddress & p_remote) { // Constructor for a TCP server
      std::clog << ">>> tcpChannel::tcpChannel (2)" << std::endl;

      if ((_socket = new socket(p_host, p_remote, socket::tcp)) == NULL) { 
	std::cerr << "tcpChannel::connect(2): " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->bind(); // TODO Check if it's correct to add bind/list here
      _socket->listen();
    }

    tcpChannel::tcpChannel(const int p_socket, const socketAddress & p_host, const socketAddress & p_remote) { // Constructor for a TCP server
      std::clog << "tcpChannel::tcpChannel (3)" << std::endl;
      if ((_socket = new socket(p_socket, p_host, p_remote)) == NULL) { 
	std::cerr << "tcpChannel::connect(3): " << std::strerror(errno) << std::endl;
	// TODO Raise an exception
      }
      _socket->setNoDelay(true);
    }

    tcpChannel::~tcpChannel() {
      std::clog << "tcpChannel::~tcpChannel" << std::endl;
      // Socket deleted by abstractChannel dtor
    }

    int tcpChannel::connect() {
      return _socket->connect();
    }

    int tcpChannel::acceptConnection() {
      return _socket->accept();
    }

    int tcpChannel::write(const std::string & p_string) {
      if (p_string.length() == 0) {
	return 0;
      }
      
      return _socket->send(converter::getInstance().string2bytes(p_string));
    }

    int tcpChannel::write(const std::vector<unsigned char> & p_buffer) {
      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->send(p_buffer);
    }

    int tcpChannel::read(std::vector<unsigned char> & p_buffer) {
      std::clog << ">>> tcpChannel::read" << std::endl;

      if (p_buffer.size() == 0) {
	return 0;
      }

      return _socket->receive(p_buffer);
    }

    char tcpChannel::read() {
      std::clog << ">>> tcpChannel::read (1)" << std::endl;

      int length = 1;
      unsigned char buffer[1] = { 0 };
      if (_socket->receive(&buffer[0], &length) < 0) {
	return '\00';
      }
      return (char)buffer[0];
    }

  } // End of namespace network

} // End of namespace beagleboneComm
