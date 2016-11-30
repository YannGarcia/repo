//============================================================================
// Name        : testLib.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : raspberryCommlibrary demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip> // Use for setprecision
#include <cstdlib> // Used for memset
#include <cstring> // Used for memcpy, strerror
#include <cmath> // Used floor, round

#include <unistd.h>
#include <signal.h>

#include "kmlImpl.h"

using namespace std;

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  std::cout << "Caught signal: " << signum << std::endl;

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

  kmlImpl & kml = kmlImpl::getInstance();
  unsigned int idCounter = 1;
  kml.createLineStyle(idCounter++, 5, static_cast<unsigned int>(0xffffffff));
  kml.createPlacemark(idCounter++, std::string("4339.8090 N"), std::string("00642.7120 E"), time(NULL), std::to_string(1), 12.7, 10.9, 0.09, 546.717, 0.97); 

  kml.createLinePlacemark(idCounter, time(NULL));
  kml.addLinePlacemark(idCounter, 12.7, 10.9, std::string("4339.8090 N"), std::string("00642.7120 E"), 0.09, 546.717, 0.97);
  kml.addLinePlacemark(idCounter, 12.7, 10.9, std::string("4339.8076 N"), std::string("00642.7140 E"), 0.09, 546.717, 0.97);
  kml.addLinePlacemark(idCounter, 12.7, 10.9, std::string("4339.8056 N"), std::string("00642.7160 E"), 0.09, 546.717, 0.97);
  kml.addLinePlacemark(idCounter, 12.7, 10.9, std::string("4339.8036 N"), std::string("00642.7180 E"), 0.09, 546.717, 0.97);
  kml.addLinePlacemark(idCounter, 12.7, 10.9, std::string("4339.8016 N"), std::string("00642.7200 E"), 0.09, 546.717, 0.97);
  kml.finalyseLinePlacemark(idCounter);
  idCounter += 1;

  std::clog << kml.toString() << std::endl;

 
  return EXIT_SUCCESS;
}
