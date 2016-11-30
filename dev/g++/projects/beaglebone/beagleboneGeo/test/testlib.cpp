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

#include "geometry.h"

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


  std::string str("4339.80569 N");
  double lat = geometry::getInstance().nmea2decimal(str);
  std::clog << "lat(decimal)=" << std::setprecision(12) << lat << std::endl;
  std::clog << "lat(nmea)=" << geometry::getInstance().decimal2nmea(lat) << std::endl;

  str.assign("00642.71356 E");
  double lon = geometry::getInstance().nmea2decimal(str);
  std::clog << "long=" << std::setprecision(12) << lon << std::endl;
  std::clog << "long(nmea)=" << geometry::getInstance().decimal2nmea(lon, 3) << std::endl;
 
  return EXIT_SUCCESS;
}
