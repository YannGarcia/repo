//============================================================================
// Name        : networkThread.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : beagleboneCommlibrary demonstrator/test application in C++, Ansi-style
//               It uses Java client server sample from http://systembash.com/content/a-simple-java-udp-server-and-udp-client/
//============================================================================

#include <iostream>
#include <iomanip>
#include <algorithm> // Used for std::find

#include <unistd.h>

#include "networkThread.h"
#include "channelManager.h"

networkThread::networkThread(const std::string & p_hostAddr, const int p_hostLocalPort, const int p_hostRemotePort) : 
  _host(p_hostAddr, p_hostLocalPort),
  _remote(p_hostAddr, p_hostRemotePort) {
  std::clog << ">>> networkThread::ctor" << std::endl;

  std::clog << "socket: " << _host.toString() << std::endl;
  std::clog << "socket: " << _remote.toString() << std::endl;
  _udp = beagleboneComm::channelManager::getInstance().createChannel(beagleboneComm::abstractChannel::udp, _host, _remote);
  beagleboneComm::channelManager::getInstance().getChannel(_udp).connect();
}

void networkThread::stop() {
  std::clog << ">>> networkThread::stop" << std::endl;

  beagleboneUtils::thread::runnable::stop();

  beagleboneComm::channelManager::getInstance().getChannel(_udp).disconnect();
  beagleboneComm::channelManager::getInstance().removeChannel(_udp);
}

void networkThread::run() {
  std::clog << ">>> networkThread::run" << std::endl;

  int index = 0;
  std::vector<char> wait;
  wait.push_back('|');
  wait.push_back('/');
  wait.push_back('-');
  wait.push_back('/');
  wait.push_back('-');
  wait.push_back('/');
  wait.push_back('-');
  _running = true;
  std::string text("Hello World");
  while (_running == true) {

    beagleboneComm::channelManager::getInstance().getChannel(_udp).write(std::vector<unsigned char>(text.begin(), text.end()));
    std::vector<int> fds;
    beagleboneComm::channelManager::getInstance().pollChannels(100, fds); // 100 ms
    std::vector<int>::iterator it = std::find(fds.begin(), fds.end(), _udp);
    if (it != fds.end()) { // Some data are available for _udp channel
      std::vector<unsigned char> result(256);
      beagleboneComm::channelManager::getInstance().getChannel(_udp).read(result);
      std::clog << "Receive data: " << std::endl;
      int idx = 0;
      for (std::vector<unsigned char>::iterator it = result.begin(); it != result.end(); ++it, idx++) {
	std::clog << "result[" << idx << "] = 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)*it << std::endl;
      } // End of 'for' statement
    } else {
      std::clog << wait[index] << '\b';
      index = (index + 1) % wait.size();
    }
  } // End of 'while' statement

  std::clog << "<<< networkThread::run" << std::endl;
}

