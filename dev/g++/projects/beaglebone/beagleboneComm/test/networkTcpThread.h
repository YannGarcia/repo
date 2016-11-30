#if !defined(__NETWORKTCPTHREAD_H__)
#define __NETWORKTCPTHREAD_H__

#include "runnable.h"

#include "abstractChannel.h"

class networkTcpThread : public beagleboneUtils::thread::runnable {
  beagleboneComm::network::socketAddress _host;
  int _tcp;

 public:
  networkTcpThread(const std::string & p_hostAddr, const int p_hostLocalPort);
  virtual ~networkTcpThread() { };

  void stop();
  void run();
}; // End of class networkTcpThread

#endif // __NETWORKTCPTHREAD_H__
