#include <iostream>
#include <iomanip> // Use for setprecision

#include <unistd.h>
#include <signal.h>
#include <limits.h>

// beagleboneUtils includes
#include "getOpt.h"
#include "keyboard.h"

// rtcmon includes
#include "mainApp.h"

#include "rtcmon.h"

static mainApp *g_app = NULL;

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  std::cout << "rtcmon: Caught signal: " << signum << std::endl;
  
  if (g_app != NULL) {
    g_app->stop();
    // Unitialization done by destructor
    delete g_app;
  }

  // Terminate program
  exit(signum);
} // End of function signal_callback_handler

/***********************************************************************
  Main application part
***********************************************************************/

int main(const int argc, const char * argv[]) {
   // Register signal and signal handler
  if (signal (SIGINT, signal_callback_handler) == SIG_ERR) {
    std::cerr <<  "rtcmon: Cannot handle SIGINT" << std::endl;
    exit (EXIT_FAILURE);
  }
  if (signal (SIGTERM, signal_callback_handler) == SIG_ERR) {
    std::cerr << "rtcmon: Cannot handle SIGTERM" << std::endl;
    exit (EXIT_FAILURE);
  }
  
  // Parse command line arguments
  bool isDebugSet, daemonMode;
  std::string host, remote;
  getOpt::getOpt opt(argc, argv);
  opt >> getOpt::Option('d', "debug", isDebugSet, false);
  opt >> getOpt::Option('m', "daemon", daemonMode, false);
  opt >> getOpt::Option('h', "host", host, "192.168.1.73");
  opt >> getOpt::Option('r', "remote", remote, "ntp.unice.fr");
  std::cout <<
    "rtcmon::Arguments: " << 
    " - debug:" << (const char *)((isDebugSet == true) ? "true" : "false") << 
    " - daemon:" << (const char *)((daemonMode == true) ? "true" : "false") << 
    " - host:" << host << 
    " - remote:" << remote << 
    std::endl;
    
  if (!daemonMode) {
    // Create the real application
    // Unitialization done by destructor
    g_app = new mainApp(host, remote);
    // Start application
    g_app->start();
    while (true) {
      
      usleep(50);
      
      if (keyboard::keyboard::kbhit() != 0) {
	std::cout << "rtcmon: key pressed was: '" << (char)getchar() << "'" << std::endl;
	break; // Exit from 'while' statement
      }
      
    } // End of 'while' statement
    g_app->stop();
    delete g_app;
    g_app = NULL;
  } else {
    int forkId = fork();
    if (forkId < 0) { // Fork error
      std::cerr << "rtcmon: wifimqpping: fork error: " << std::endl;
      exit(-1);
    } else if (forkId == 0) { // Chid side
      setsid(); // Obtain a new process group
      // Create the real application
      // Unitialization done by destructor
      g_app = new mainApp(host, remote);
      // Start application
      g_app->start();
      while (true) sleep(10);
      // The child will terminate on kill signal
    } else { // Parent side
      std::cout << "rtcmon: Parent exiting..." << std::endl;
      exit(EXIT_SUCCESS);
    }
  }
  
  return EXIT_SUCCESS;
} // End of function main
