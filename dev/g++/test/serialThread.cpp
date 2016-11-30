#include <iostream>
#include <iomanip>

#include "serialThread.h"
#include "serialChannel.h"
#include "channelManager.h"

serialThread::serialThread(const std::string & p_device, const int p_speed) : 
  _device(p_device),
  _speed(p_speed) {
  std::clog << ">>> serialThread::ctor" << std::endl;

  std::clog << "device: " << p_device << std::endl;
  std::clog << "speed: " << p_speed << std::endl;
  _serial = raspberryComm::channelManager::getInstance().createChannel(raspberryComm::abstractChannel::serial, p_device, p_speed);
  std::clog << "Channel id: " << std::dec << _serial << std::endl;
}

void serialThread::stop() {
  std::clog << ">>> serialThread::stop" << std::endl;

  raspberryComm::channelManager::getInstance().getChannel(_serial).disconnect();

  raspberryComm::channelManager::getInstance().removeChannel(_serial);
}

void serialThread::run() {
  std::clog << ">>> serialThread::run" << std::endl;

  _running = true;
  while (_running == true) {
    std::string buffer;
    char c = raspberryComm::channelManager::getInstance().getChannel(_serial).read();
    while ((c != '\r') && (_running == true) && (buffer.length() < 80)) {
      buffer += c;
      usleep(10);
      c = raspberryComm::channelManager::getInstance().getChannel(_serial).read();
    } // End of 'while' statement
    if (c == '\r') buffer += c; // 80c reached
    std::clog << "Receive data: " << buffer << std::endl;
  } // End of 'while' statement

  std::clog << "<<< serialThread::run" << std::endl;
}

