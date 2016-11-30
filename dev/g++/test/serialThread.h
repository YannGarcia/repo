#if !defined(__NETWORTHREAD_H__)
#define __SERIALTHREAD_H__

#include "runnable.h"

#include "abstractChannel.h"

class serialThread : public raspberryUtil::thread::runnable {
  std::string _device;
  int _speed;
  int _serial;

 public:
  serialThread(const std::string & p_device, const int p_speed);
  virtual ~serialThread() { };

  void stop();
  void run();
}; // End of class serialThread

#endif // __SERIALTHREAD_H__
