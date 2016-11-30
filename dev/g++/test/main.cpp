//============================================================================
// Name        : HelloWorld.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : Hello World in C++, Ansi-style
// emacs abstractChannel.h channelManager.h channelManager.cpp socket.h socket.cpp udpChannel.h udpChannel.cpp  tcpChannel.h tcpChannel.cpp rawChannel.h rawChannel.cpp serialChannel.h serialChannel.cpp socketAddress.h socketAddress.cpp ipv4Address.h ipv4Address.cpp runnable.h keyboard.h keyboard.cpp networkThread.h networkThread.cpp serialThread.h serialThread.cpp main.cpp
// clear ; rm ./test ; g++ -O3 -std=c++0x -Wall -I. -lstdc++ -lpthread -otest channelManager.cpp udpChannel.cpp ipv4Address.cpp socketAddress.cpp socket.cpp getOpt.cpp keyboard.cpp mainTrigger.cpp main.cpp
// clear ; rm ./test ; g++ -O3 -std=c++0x -Wall -I/home/fscom/frameworks/wiringPi -I. -lstdc++ -lpthread -lwiringPi -lpcap -otest channelManager.cpp udpChannel.cpp ipv4Address.cpp tcpChannel.h tcpChannel.cpp rawChannel.h rawChannel.cpp serialChannel.h serialChannel.cpp socketAddress.cpp socket.cpp getOpt.cpp keyboard.cpp networkThread.cpp serialThread.cpp main.cpp
//============================================================================

#include <iostream>
#include <iomanip> // Use for setprecision
//#include <chrono>

#include <unistd.h>
#include <signal.h>
#include <limits.h>

#include <pcap.h>

#include "keyboard.h"

#include "getOpt.h"

#include "networkThread.h"
#include "serialThread.h"

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  std::cout << "Caught signal: " << signum << std::endl;

  // Terminate program
  exit(signum);
} // End of function signal_callback_handler

/***********************************************************************
  Main application part
***********************************************************************/

int main(int argc, char *argv[]) {
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
  std::string hostAddr;
  int hostLocalPort, hostRemotePort;
  bool isDebugSet, isHelpRequired;
  std::string commMode;
  getOpt::getOpt opt(argc, argv);
  opt >> getOpt::Option('s', "host", hostAddr, "localhost");
  opt >> getOpt::Option('l', "lport", hostLocalPort, 13010);
  opt >> getOpt::Option('r', "rport", hostRemotePort, 13010);
  opt >> getOpt::Option('d', "debug", isDebugSet, false);
  opt >> getOpt::Option('m', "commMode", commMode, "network");
  opt >> getOpt::Option('v', "help", isHelpRequired, false);
  std::cout <<
    "Arguments: " << 
    " - hostAddr: " << hostAddr << 
    " - Remote port:" << hostRemotePort << 
    " - Local port:" << hostLocalPort << 
    " - debug:" << (const char *)((isDebugSet == true) ? "true" : "false") << 
    " - help:" << (const char *)((isHelpRequired == true) ? "true" : "false") << 
    " - commMode:" << commMode << 
    std::endl;

  // Setup PCAP
  char errbuf[PCAP_ERRBUF_SIZE];
  char *dev = pcap_lookupdev(errbuf);
  if (dev == NULL) {
    std::cerr <<  "Couldn't find default device: " << errbuf << std::endl;
    return EXIT_FAILURE;
  }
  std::clog << "Device: " << dev << std::endl;

  // Start processing thread
  std::cout << "Strike any key to termnate" << std::endl;
  raspberryUtil::thread::runnable *trigger;
  if (commMode.compare("network") == 0) {
    trigger = dynamic_cast<raspberryUtil::thread::runnable *>(new networkThread(hostAddr, hostRemotePort, hostLocalPort));
  } else if (commMode.compare("serial") == 0) {
    trigger = dynamic_cast<raspberryUtil::thread::runnable *>(new serialThread(hostAddr, 9600));
  } else {
    std::cerr << "Invalid argument commMode: should be 'network' or 'serial'" << std::endl;
    return -1;
  }
  trigger->start();
  while (true) {

    usleep(100);
    //std::this_thread::sleep_for(std::chrono::microseconds(100));

    if (raspberryUtil::keyboard::keyboard::kbhit() != 0) {
      std::cout << "key pressed was: '" << (char)getchar() << "'" << std::endl;
      break; // Exit from 'while' statement
    }

  } // End of 'while' statement

  trigger->stop();
  delete trigger;

  return EXIT_SUCCESS;
}


