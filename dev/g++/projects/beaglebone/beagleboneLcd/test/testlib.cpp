//============================================================================
// Name        : HelloWorld.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : beagleboneCommlibrary demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip> // Used for setprecision
#include <memory> // Used for unique_ptr

#include <unistd.h>
#include <signal.h>
#include <limits.h>

#include "getOpt.h"
#include "converter.h"
#include "keyboard.h"

#include "lcdManager.h"

#define PWM2A_LCD     22 // P8.19 - gpio0_22

using namespace std;

static std::unique_ptr<abstractLcd> lcd;

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  std::cout << "Caught signal: " << signum << std::endl;

  if (lcd != NULL) {
    lcd->uninitialize();
    lcd.reset(nullptr);
  }

  // Uninitialize the wiringBbb library
  ::wiringBbbShutdownSys();

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
  bool isDebugSet;
  getOpt::getOpt opt(argc, argv);
  opt >> getOpt::Option('d', "debug", isDebugSet, false);
  std::cout <<
    "Arguments: " << 
    " - debug:" << (const char *)((isDebugSet == true) ? "true" : "false") << 
    std::endl;

  // LCD datas port in 4 bits mode
  std::vector<unsigned char> dataPorts;
  dataPorts.push_back(76); // DB7 - P8_39
  dataPorts.push_back(77); // DB6 - P8_40
  dataPorts.push_back(74); // DB5 - P8_41
  dataPorts.push_back(75); // DB4 - P8_42
  // Initialize the wiringBbb library
  ::wiringBbbSetup();
  lcdManager::getInstance().createLcdDevice(abstractLcd::HD44780U, lcd);
  //lcd->setPowerMode(abstractLcd::mode3_3V, PWM2A_LCD); // Set power mode to 3.3V => PWM could be required to generate the negative voltage on the Vlcd
  lcd->setPowerMode();
  lcd->initialize(abstractLcd::mode_4bits, abstractLcd::lcd20x4, dataPorts, 46/*en*/, 65/*rs*//*, 27rw*/);

  std::cout << "LCD initialization done" << std::endl;
  lcd->clear();
  std::string str("Hello World!");
  lcd->write(str);
  std::cout << "LCD Hello World! done" << std::endl;
  lcd->setCursor(2, 10);
  std::cout << "LCD setCursor done" << std::endl;
  str.assign("From Me");
  lcd->write(str);
  std::cout << "LCD From Me done" << std::endl;

  while (true) {

    usleep(50);

    if (beagleboneUtils::keyboard::keyboard::kbhit() != 0) {
      std::cout << "key pressed was: '" << (char)getchar() << "'" << std::endl;
      break; // Exit from 'while' statement
    }

  } // End of 'while' statement

  lcd->clear();
  lcd->cursorOff();
  lcd->uninitialize();
  lcd.reset(nullptr);

  // Uninitialize the wiringBbb library
  ::wiringBbbShutdownSys();

  return EXIT_SUCCESS;
}
