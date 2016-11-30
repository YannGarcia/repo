//============================================================================
// Name        : testLib.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : beagleboneCommlibrary demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>

#include <unistd.h>
#include <signal.h>

#include "thingspeakImpl.h"

using namespace std;

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  cout << "Caught signal: " << signum << endl;
  // Cleanup and close up stuff here

  // Terminate application
  exit(EXIT_SUCCESS);
} // End of function signal_callback_handler

int main() {
  // Register signal and signal handler
  if (signal (SIGINT, signal_callback_handler) == SIG_ERR) {
    cerr <<  "Cannot handle SIGINT!" << endl;
    exit (EXIT_FAILURE);
  }
  if (signal (SIGTERM, signal_callback_handler) == SIG_ERR) {
    fprintf (stderr, "Cannot handle SIGTERM!\n");
    exit (EXIT_FAILURE);
  }

  thingspeakImpl ts(std::string("TBCRYSUBMABGA8QT"));
  ts.setField(1, 20.48);
  ts.pushData();

  return 0;
}
