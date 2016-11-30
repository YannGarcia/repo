//============================================================================
// Name        : networkTcpThread.cpp
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

#include "networkTcpThread.h"
#include "channelManager.h"

networkTcpThread::networkTcpThread(const std::string & p_hostAddr, const int p_hostLocalPort) : _host(p_hostAddr, p_hostLocalPort) {
  std::clog << ">>> networkTcpThread::ctor" << std::endl;

  std::clog << "socket: " << _host.toString() << std::endl;
  _tcp = beagleboneComm::channelManager::getInstance().createChannel(beagleboneComm::abstractChannel::tcp, _host);
}

void networkTcpThread::stop() {
  std::clog << ">>> networkTcpThread::stop" << std::endl;

  beagleboneComm::channelManager::getInstance().getChannel(_tcp).disconnect();
  beagleboneComm::channelManager::getInstance().removeChannel(_tcp);

  beagleboneUtils::thread::runnable::stop();
}

void networkTcpThread::run() {
  std::clog << ">>> networkTcpThread::run" << std::endl;

  /*  int index = 0;
  std::vector<char> wait;
  wait.push_back('|');
  wait.push_back('/');
  wait.push_back('-');
  wait.push_back('/');
  wait.push_back('-');
  wait.push_back('/');
  wait.push_back('-');*/
  _running = true;
  std::string text("Hello World");
  while (_running == true) {
    int newcx = beagleboneComm::channelManager::getInstance().getChannel(_tcp).acceptConnection();
    std::clog << "Got new connection: " << newcx << std::endl;
    if (newcx == -1) { 
      std::cerr << "networkTcpThread::run: accept failed" << std::endl;
      sleep(2);
    } else {
      std::clog << "Send value: " << text << std::endl;
      beagleboneComm::channelManager::getInstance().getChannel(newcx).write(std::vector<unsigned char>(text.begin(), text.end()));
      bool exit = false;
      while (!exit && (_running == true)) {
	/*	std::vector<unsigned char> buffer(256, 0x00);
	std::clog << "Call read" << std::endl;
	int result = beagleboneComm::channelManager::getInstance().getChannel(newcx).read(buffer);
	if (result < 0) {
	  std::clog << "networkTcpThread::run: error: " << result << std::endl;
	  sleep(2);
	} else if (buffer.size() > 0) {
	  std::clog << "networkTcpThread::run: data received: '" << reinterpret_cast<const char *>(buffer.data()) << "'" << std::endl;
	sleep(2);
      } else {
	  std::clog << "networkTcpThread::run: no data received, wait 2s" << std::endl;
	  sleep(2);
	  }*/
	std::vector<int> channelsToPoll;
	channelsToPoll.push_back(newcx);
	std::vector<int> fds;
	beagleboneComm::channelManager::getInstance().pollChannels(1000, channelsToPoll, fds); // 1000 ms
	if (fds.size() != 0) { 
	  std::clog << "fds size: " << fds.size() << " - " << (int)fds[0] << std::endl;
	  std::clog << "Something append on: " << (int)fds[0] << std::endl;
	  std::vector<unsigned char> result(256);
	  if (beagleboneComm::channelManager::getInstance().getChannel(fds[0]).read(result) < 0) {
	    beagleboneComm::channelManager::getInstance().removeChannel(fds[0]);
	    exit = true;
	  } else if (result.size() != 0) {
	    std::clog << "Receive data: " << std::endl;
	    std::clog << "result: '" << reinterpret_cast<const char *>(result.data()) << "'" << std::endl;
	  } else { // If poll raised an event and fds is empty, cx was lost
	    std::clog << "networkTcpThread::run: no data received, wait 2s" << std::endl;
	    exit = true;
	  }
	}

      } // End of 'while' statement
	
      beagleboneComm::channelManager::getInstance().removeChannel(newcx);
    }
    
  } // End of 'while' statement
  
  std::clog << "<<< networkTcpThread::run" << std::endl;
}

