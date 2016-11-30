#include <iostream>
#include <iomanip> // Use for setprecision
#include <thread>

#include <unistd.h>
#include <signal.h>
#include <limits.h>

// beagleboneUtils includes
#include "getOpt.h"
#include "keyboard.h"

// beagleboneHw
#include "beagleboneHwInit.h"
#include "digitalOut.h"

// wifimapping includes
#include "mainApp.h"

#include "wifimapping.h"

#include "hwconfig.h"

static mainApp *g_app = NULL;

// Power On thread support
bool g_resumeThread = false;
bool g_exitThread = false;
void PowerOnLed(const int p_timeout);
std::thread g_threadPowerOnLed(PowerOnLed, 1000);

void PowerOnLed(const int p_timeout) {
  while (!g_resumeThread) {
    delay(p_timeout);
  } // End of 'while' statement

  digitalOut powerLed(LED_GREEN_PORT);
  int powerOnLevel = 0;
  do {
    powerLed.write(powerOnLevel);
    delay(p_timeout);
    powerOnLevel ^= 1;
  } while (!g_exitThread); // End of 'do-while' statement
  powerLed.write(0);
  std::cout << "Power On thread exited" << std::endl;
} // End of 'PowerOnLed' thread function



// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  std::cout << "Caught signal: " << signum << std::endl;
  
  if (g_app != NULL) {
    g_app->stop();
    // Unitialization done by destructor
    delete g_app;
  }
  
  // Join PowerOn thread
  g_exitThread = true;
  std::cout << "Join threads" << std::endl;
  g_threadPowerOnLed.join();

  // Terminate program
  exit(signum);
} // End of function signal_callback_handler

/***********************************************************************
  Main application part
***********************************************************************/

int main(const int argc, const char * argv[]) {
   // Register signal and signal handler
  if (signal (SIGINT, signal_callback_handler) == SIG_ERR) {
    std::cerr <<  "Cannot handle SIGINT" << std::endl;
    exit (EXIT_FAILURE);
  }
  if (signal (SIGTERM, signal_callback_handler) == SIG_ERR) {
    std::cerr << "Cannot handle SIGTERM" << std::endl;
    exit (EXIT_FAILURE);
  }
  
  // Parse command line arguments
  bool isDebugSet, daemonMode, autoMode;
  getOpt::getOpt opt(argc, argv);
  opt >> getOpt::Option('a', "autoMode", autoMode, true);
  opt >> getOpt::Option('d', "debug", isDebugSet, false);
  opt >> getOpt::Option('m', "daemon", daemonMode, false);
  /*std::cout <<
    "wifimapping::Arguments: " << 
    " - autoMode:" << (const char *)((autoMode == true) ? "true" : "false") <<
    " - debug:" << (const char *)((isDebugSet == true) ? "true" : "false") << 
    " - daemon:" << (const char *)((daemonMode == true) ? "true" : "false") << 
    std::endl;*/
  
  // Initialize the WinringPI library
  beagleboneHwInit::getInstance().initialize();

  // Spawn power on thread
  g_resumeThread = true;

  int retCode = -1;
  if (!daemonMode) {
    // Create the real application
    // Unitialization done by destructor
    g_app = new mainApp();
    // Initialize application
    g_app->initialize(autoMode);
    // Start application
    g_app->start();
    while (true) {
      
      usleep(50);
      
      if (keyboard::keyboard::kbhit() != 0) {
	std::cout << "key pressed was: '" << (char)getchar() << "'" << std::endl;
	break; // Exit from 'while' statement
      }
      
    } // End of 'while' statement
    g_app->stop();
    delete g_app;
    g_app = NULL;
    retCode = 0;
  } else {
    int forkId = fork();
    if (forkId < 0) { // Fork error
      // Uninitialize the WiringBBB library: done by beagleboneHwInit destructor

      std::cerr << "wifimapping: fork error: " << std::endl;
    } else if (forkId == 0) { // Chid side
      setsid(); // Obtain a new process group
      // Create the real application
      // Unitialization done by destructor
      g_app = new mainApp();
      // Initialize application
      g_app->initialize(autoMode);
      // Start application
      g_app->start();
      while (true) sleep(10);
      // The child will terminate on kill signal
      retCode = 0;
    } else { // Parent side
      std::cout << "Parent exiting..." << std::endl;
      retCode = 0;
    }
  }

  // Join PowerOn thread
  g_exitThread = true;
  std::cout << "Join threads" << std::endl;
  g_threadPowerOnLed.join();
  
  // Uninitialize the WiringBBB library: done by beagleboneHwInit destructor
    
  return retCode;
} // End of function main
