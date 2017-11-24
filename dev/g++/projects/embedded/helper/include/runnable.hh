/*!
 * \file      runnable.h
 * \brief     Header file for the runnable helper library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <memory>
#include <atomic>
#include <thread>

/*! \namespace helpers
 *  \brief helpers namespace
 */
namespace helpers {

  /*! \namespace helpers::threads
   *  \brief Thread specific helpers namespace
   */
  namespace thread {

    /*!
     * \class runnable
     * \brief This class implements a interface for class references to be executed as a thread 
     */
    class runnable {

    protected: /*! \protectedsection */
      /*!< Pointer to the thread */
      std::unique_ptr<std::thread> _thread;
      /*!< Indicates if the thread is running */
      std::atomic<bool> _running;

    public: /*! \publicsection */
      /*!
       * \brief Default constructor
       */
      
      runnable() : _running(false) { _thread.reset(nullptr); };
      /*!
       * \brief Default destructor
       */
      virtual ~runnable() { };
      
      /*!
       * \fn void start();
       * \brief Start the thread
       * \inline
       * \virtual
       */
      inline virtual void start() { _thread.reset(new std::thread(std::ref(*this))); };
      /*!
       * \fn void stop();
       * \brief Stop the thread
       * \inline
       * \virtual
       */
      inline virtual void stop() { _running = false; _thread->join(); _thread.reset(nullptr); };
      /*!
       * \brief Cast operator
       */
      inline void operator()() { run(); };
      
    protected: /*! \protectedsection */
      /*!
       * \fn void run();
       * \brief Abstract thread function to be implemented
       * \virtual
       * \pure
       */
      virtual void run() = 0;
      
    }; // End of class runnable

  } // End of namespace thread

} // End of namespace helpers

using namespace helpers::thread;
