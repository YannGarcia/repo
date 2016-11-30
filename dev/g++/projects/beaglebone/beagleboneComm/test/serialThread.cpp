//============================================================================
// Name        : serialThread.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : beagleboneCommlibrary demonstrator/test application in C++, Ansi-style
//               It uses Java client server sample from http://systembash.com/content/a-simple-java-udp-server-and-udp-client/
//============================================================================

#include <iostream>
#include <iomanip>
#include <algorithm> // Used for std::find

#include <unistd.h> // Used for usleep

#include "serialThread.h"
#include "channelManager.h"

#include "converter.h"

serialThread::serialThread(const std::string & p_device, const int p_speed) : 
  _device(p_device),
  _speed(p_speed) {
  std::clog << ">>> serialThread::ctor" << std::endl;

  std::clog << "device: " << p_device << std::endl;
  std::clog << "speed: " << p_speed << std::endl;
  _serial = beagleboneComm::channelManager::getInstance().createChannel(beagleboneComm::abstractChannel::serial, p_device, p_speed);
  std::clog << "Channel id: " << std::dec << _serial << std::endl;
  beagleboneComm::channelManager::getInstance().getChannel(_serial).connect();
}

void serialThread::stop() {
  std::clog << ">>> serialThread::stop" << std::endl;

  beagleboneUtils::thread::runnable::stop();

  beagleboneComm::channelManager::getInstance().getChannel(_serial).disconnect();
  beagleboneComm::channelManager::getInstance().removeChannel(_serial);
}

void serialThread::run() {
  std::clog << ">>> serialThread::run" << std::endl;
  
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

  // Send init sequence using $PMTK
  std::string init("$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"); // Turn on all the available data
  beagleboneComm::channelManager::getInstance().getChannel(_serial).write(init);
  init.assign("$PMTK220,1000*1F"); // Set frequency to 1Hz
  beagleboneComm::channelManager::getInstance().getChannel(_serial).write(init);
  
  while (_running == true) {
    std::vector<int> fds;
    beagleboneComm::channelManager::getInstance().pollChannels(100, fds); // 100 ms
    std::vector<int>::iterator it = std::find(fds.begin(), fds.end(), _serial);
    if (it != fds.end()) { // Some data are available for _serial channel
      std::vector<unsigned char> result(beagleboneComm::channelManager::getInstance().getChannel(_serial).dataAvailable());
      beagleboneComm::channelManager::getInstance().getChannel(_serial).read(result);
      
      /*    std::string buffer;
	    char c = beagleboneComm::channelManager::getInstance().getChannel(_serial).read();
	    while ((c != '\r') && (_running == true) && (buffer.length() < 80)) {
	    buffer += c;
	    usleep(10);
	    c = beagleboneComm::channelManager::getInstance().getChannel(_serial).read();
	    } // End of 'while' statement
    if (c == '\r') buffer += c; // 80c reached
      */
      std::string buffer = converter::getInstance().bytes2string(result);
      std::clog << "Receive data: '" << buffer << "'" << std::endl;
    } else {
      std::clog << wait[index] << '\b';
      index = (index + 1) % wait.size();
    }
  } // End of 'while' statement
  
  std::clog << "<<< serialThread::run" << std::endl;
}

