//============================================================================
// Name        : HelloWorld.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : raspberryGps library demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip> // Use for setprecision
#include <fstream>

#include <unistd.h>
#include <signal.h>
#include <limits.h>

#include "keyboard.h"
#include "dump.h"

#include "gpsDevicesMgr.h"
#include "gpsProtocolsMgr.h"

#include "dump.h"

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

  // Setup GPS session
  // Create GPS protocol analyzer
  std::unique_ptr<gpsProtocolsAnalyzer> nmea;
  gpsProtocolsMgr::getInstance().createProtocolAnalyzer(gpsProtocolsMgr::nmea2000, nmea);
  // Create an instance og GPS
  std::unique_ptr<gpsDevices> gps(nullptr);
  gpsDevicesMgr::getInstance().createGpsDevice(gpsDevicesMgr::ublox, gps);
  // Initialise it
  gps->initialize(nmea);
  // Start GPS session
  gps->start();

  while (true) {

    // Get GPS data
    std::vector<unsigned char> result;
    gps->getResult(result);
    std::clog << "# of datas: " << result.size() << std::endl;
    dump::hexaDump(result);

    usleep(500);

    if (keyboard::keyboard::kbhit() != 0) {
      std::cout << "key pressed was: '" << (char)getchar() << "'" << std::endl;
      break; // Exit from 'while' statement
    }

  } // End of 'while' statement

  // Terminate GPS session
  gps->stop();
  gps->uninitialize();

  // Reset pointers
  gps.reset();
  nmea.reset();

  return EXIT_SUCCESS;
}
