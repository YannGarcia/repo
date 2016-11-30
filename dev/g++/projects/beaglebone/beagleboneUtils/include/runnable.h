#if !defined(__RUNNABLE_H__)
#define __RUNNABLE_H__

#include <iostream> // TODO To be removed

//#include <atomic>
#include <thread>

namespace beagleboneUtils {

  namespace thread {

    class runnable {

    protected:
      std::thread *_thread;
      //      std::atomic<bool> _running;
      bool _running;

    public:
      runnable() : _running(false) { std::clog << ">>> runnable::ctor" << std::endl; _thread = NULL; };
      virtual ~runnable() { };

      inline virtual void start() { _thread = new std::thread(std::ref(*this)); };
      inline virtual void stop() { std::clog << ">>> runnable::stop" << std::endl; _running = false; _thread->join(); delete _thread; _thread = NULL; };
      inline void operator()() { run(); };
    protected:
      virtual void run() = 0;
      
    }; // End of class runnable

  } // End of namespace thread

} // End of namespace beagleboneUtils

using namespace beagleboneUtils::thread;

#endif // __RUNNABLE_H__
