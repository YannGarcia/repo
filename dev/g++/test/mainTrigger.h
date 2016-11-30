#if !defined(__MAINTRIGGER_H__)
#define __MAINTRIGGER_H__

#include "runnable.h"

#include "abstractChannel.h"
#include "channelManager.h"

class mainTrigger : public raspberryUtil::thread::runnable {
  raspberryComm::network::socketAddress _host;
  raspberryComm::network::socketAddress _remote;
  int _udp;

 public:
  mainTrigger(std::string & p_hostAddr, const int p_hostLocalPort, const int p_hostRemotePort);
  virtual ~mainTrigger() { };

  void stop();
  void run();
}; // End of class mainTrigger

#endif //  __MAINTRIGGER_H__

