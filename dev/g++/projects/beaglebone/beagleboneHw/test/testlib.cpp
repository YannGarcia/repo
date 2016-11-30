//============================================================================
// Name        : HelloWorld.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : beagleboneHwlibrary demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip> // Use for setprecision
#include <thread>
#include <cstdlib> // Used for memset
#include <cstring> // Used for memcpy, strerror

#include <unistd.h>
#include <signal.h>

#include "beagleboneHwInit.h"
#include "mcp4xxxx_spi.h"
#include "mcp320x_spi.h"
#include "mcp23x17_i2c.h"
#include "ds1307_i2c.h"
#include "mcp7941x_i2c.h"
#include "mcp24lc_i2c.h"
#include "mcp980x_i2c.h"

#include "digitalIn.h"
#include "digitalOut.h"
#include "digitalInOut.h"

using namespace std;

struct UserChoice {
  char choice;
  bool isShutdown;
};

#define LED                 7 // GPIO7 - pin 7
#define CS_MCP3208         10 // CE0 - pin 24
#define CS_MCP41050        11 // GPIO1 - pin 25
#define SQWOUT              2 // GPIO2 - pin 13
#define XIO_PORT_LED        mcp23x17_i2c::PortB
#define XIO_IO_LED          mcp23x17_i2c::Io6
#define XIO_PORT_PB         mcp23x17_i2c::PortB
#define XIO_IO_PB           mcp23x17_i2c::Io1
#define XIO_PORT_ADC        mcp23x17_i2c::PortA
#define XIO_IO_ADC          mcp23x17_i2c::Io5

// SPI devices
mcp320x_spi *g_adc = NULL;
mcp4xxxx_spi *g_digiPot = NULL;

// I2C devices
ds1307_i2c *g_rtc = NULL;
mcp7941x_i2c *g_rtc1 = NULL;
mcp23x17_i2c *g_xgpio = NULL;
mcp24lc_i2c *g_eeprom = NULL;
mcp980x_i2c *g_tempSensor = NULL;

// Processing variables
static bool g_exitRequested = false;
static UserChoice g_userChoice;

// Power On thread support
bool g_resumeThread = false;
void PowerOnLed(const int p_timeout);
thread g_threadPowerOnLed(PowerOnLed, 1000);

void PowerOnLed(const int p_timeout) {
  while (!g_resumeThread) {
    delay(p_timeout);
  } // End of 'while' statement

  digitalOut powerLed(LED);
  int powerOnLevel = 0;
  do {
    powerLed.write(powerOnLevel);
    delay(p_timeout);
    powerOnLevel ^= 1;
  } while (!g_exitRequested); // End of 'do-while' statement
  /*
  pinMode(LED, OUTPUT);
  do {
    digitalWrite(LED, powerOnLevel);
    delay(p_timeout);
    powerOnLevel ^= HIGH;
  } while (!g_exitRequested); // End of 'do-while' statement
  digitalWrite(LED, LOW);*/
  cout << "Power On thread exited" << endl;
} // End of 'PowerOnLed' thread function

void DisplayMenuAndGetChoice() {
  // Display title 
  cout << endl << endl << "------------------------------------------------------------------------------------------------------" << endl;
  cout << "Test beagleboneHw library app V0.0.1" << endl;
  // Display the menu
  std::cout << "\tSPI tests:" << std::endl;
  std::cout << "\t\tIncrease digipot:\t\t\ta" << std::endl;
  std::cout << "\t\tDecrease digipot:\t\t\tb" << std::endl;
  std::cout << "\t\tShutdown digipot:\t\t\tc" << std::endl;
  std::cout << "\t\tReset pot       :\t\t\td" << std::endl;
  std::cout << "\t\tDigipot voltage :\t\t\te" << std::endl;
  std::cout << "\t\tLed voltage     :\t\t\tf" << std::endl;

  std::cout << "\tDS1307 tests (I2C):" << std::endl;
  std::cout << "\t\tRead time       :\t\t\tg" << std::endl;
  std::cout << "\t\tSet time        :\t\t\th" << std::endl;
  std::cout << "\t\tSet 24H mode    :\t\t\ti" << std::endl;
  std::cout << "\t\tSet 12H mode    :\t\t\tj" << std::endl;
  std::cout << "\t\tEnable SQWE     :\t\t\tk" << std::endl;
  std::cout << "\t\tEnable clock    :\t\t\tl" << std::endl;
  std::cout << "\t\tChange OUT      :\t\t\tm" << std::endl;
  std::cout << "\t\tSave buffer     :\t\t\tn" << std::endl;
  std::cout << "\t\tRead buffer     :\t\t\to" << std::endl;

  std::cout << "\tMCP23x17 tests (I2C):" << std::endl;
  std::cout << "\t\tLed on          :\t\t\tp" << std::endl;
  std::cout << "\t\tLed off         :\t\t\tq" << std::endl;
  std::cout << "\t\tRead led port   :\t\t\tr" << std::endl;
  std::cout << "\t\tADC on          :\t\t\ts" << std::endl;
  std::cout << "\t\tADC off         :\t\t\tt" << std::endl;
  std::cout << "\t\tRead ADC port   :\t\t\tu" << std::endl;
  std::cout << "\t\tRead PB port    :\t\t\tv" << std::endl;
  std::cout << "\tMCP24LC256 tests (I2C):" << std::endl;
  std::cout << "\t\tSave string     :\t\t\tw" << std::endl;
  std::cout << "\t\tRead string     :\t\t\tx" << std::endl;

  std::cout << "\tMCP9801 Temperature Sensor (I2C):" << std::endl;
  std::cout << "\t\tAmbiant temp.   :\t\t\ty" << std::endl;
  std::cout << "\t\tSet alert temp. :\t\t\tz" << std::endl;

  std::cout << "\tMCP7941x tests (I2C):" << std::endl;
  std::cout << "\t\tRead time       :\t\t\t0" << std::endl;
  std::cout << "\t\tSet time        :\t\t\t1" << std::endl;
  std::cout << "\t\tSet 24H mode    :\t\t\t2" << std::endl;
  std::cout << "\t\tSet 12H mode    :\t\t\t3" << std::endl;
  std::cout << "\t\tEnable SQWE     :\t\t\t4" << std::endl;
  std::cout << "\t\tEnable clock    :\t\t\t5" << std::endl;
  std::cout << "\t\tChange OUT      :\t\t\t6" << std::endl;
  std::cout << "\t\tSave buffer     :\t\t\t7" << std::endl;
  std::cout << "\t\tRead buffer     :\t\t\t8" << std::endl;

  std::cout << "Enter your choice: " << std::flush;
  // Get selection
  g_userChoice.choice = getchar();
  cin.ignore(1, '\n');
  cout << endl << "------------------------------------------------------------------------------------------------------" << endl << flush;
}

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  cout << "Caught signal: " << signum << endl;
  g_exitRequested = true;
  // Cleanup and close up stuff here
  if (g_adc != NULL) delete g_adc;
  if (g_digiPot != NULL) delete g_digiPot;
  if (g_xgpio != NULL) delete g_xgpio;
  if (g_rtc != NULL) delete g_rtc;
  if (g_rtc1 != NULL) delete g_rtc1;
  if (g_eeprom != NULL) delete g_eeprom;
  if (g_tempSensor != NULL) delete g_tempSensor;

  // Join PowerOn thread
  cout << "Join threads" << endl;
  g_threadPowerOnLed.join();

  // Terminate application
  exit(EXIT_SUCCESS);
} // End of function signal_callback_handler

int gpioCallback(void *p_arg) {
  unsigned char isrFlags, gpio;
  g_xgpio->readFromIsr(XIO_PORT_PB, isrFlags, gpio);
  cout << "gpioCallback: isrFlags=0x" << hex << setw(2) << setfill('0') << (int)isrFlags << " - value=0x" << (int)gpio << endl;
  return 0;
}

#define MAX_ADDRESSES_LENGTH 4
int main(const int argc, const char * argv[]) {
  unsigned short _addresses[MAX_ADDRESSES_LENGTH] = {
    9, 
    99, 
    999, 
    9999
  };
  unsigned short _memoryValues[MAX_ADDRESSES_LENGTH] = {
    (unsigned short)0xbabe, 
    (unsigned short)0xdeca, 
    (unsigned short)0xbeef, 
    (unsigned short)0xdead
  };
  unsigned int _addressIdx = 0;

  // Register signal and signal handler
  if (signal (SIGINT, signal_callback_handler) == SIG_ERR) {
    cerr <<  "Cannot handle SIGINT!" << endl;
    exit (EXIT_FAILURE);
  }
  if (signal (SIGTERM, signal_callback_handler) == SIG_ERR) {
    fprintf (stderr, "Cannot handle SIGTERM!\n");
    exit (EXIT_FAILURE);
  }

  // Initialize the wiringBbb library
  beagleboneHwInit::getInstance().initialize();
  // Spawn power on thread
  g_resumeThread = true;
  // Create instance of SPI devices
  g_adc = new mcp320x_spi(CS_MCP3208, mcp320x_spi::_3208);
  g_digiPot = new mcp4xxxx_spi(CS_MCP41050);
  // Create instance of I2C devices
  g_xgpio = new mcp23x17_i2c(7); // <A2,A0>=111b - i2cdetect -y 1 returns 0x27
  g_xgpio->setIoDirection(XIO_PORT_LED, XIO_IO_LED, mcp23x17_i2c::Output);
  g_xgpio->setIoDirection(XIO_PORT_ADC, XIO_IO_ADC, mcp23x17_i2c::Output);
  g_xgpio->setIoDirection(XIO_PORT_PB, XIO_IO_PB, mcp23x17_i2c::Input, true);
  g_xgpio->setIntrCallback(XIO_PORT_PB, &gpioCallback);
  g_rtc = new ds1307_i2c(); // i2cdetect -y 1 returns 0x68
  g_rtc->setHourMode();
  g_rtc1 = new mcp7941x_i2c(); // i2cdetect -y 1 returns 0x6f
  g_rtc1->setHourMode();
  bool enableClock = true;
  g_rtc->enableClock(enableClock);
  g_rtc1->enableClock(enableClock);
  g_eeprom = new mcp24lc_i2c(0x01); // <A2,A0>=001b - i2cdetect -y 1 returns 0x51
  g_tempSensor = new mcp980x_i2c(0x00); // <A2,A0>=000b - i2cdetect -y 1 returns 0x48

  /*  cout << "[" << _addressIdx << "] Store " << hex << setw(2) << setfill('0') << _memoryValues[_addressIdx] << " at address " << dec << _addresses[_addressIdx] << endl;
  if (g_eeprom->write(_addresses[_addressIdx], _memoryValues[_addressIdx]) < 0) {
    cerr << "Failed to call mcp24lc_i2c::write: " << strerror(errno) << endl;
  } else {
    short value = 0;
    if (g_eeprom->read(_addresses[_addressIdx], (short *)&value) < 0) {
      cerr << "Failed to call mcp24lc_i2c::read: " << strerror(errno) << endl;
    } else {
      cout << "[" << _addressIdx << "] Read " << hex << setw(2) << setfill('0') << (short)value << " at address " << dec << _addresses[_addressIdx] << endl;
      _addressIdx = (_addressIdx + 1) % MAX_ADDRESSES_LENGTH;
    }
  }
  
  signal_callback_handler(0);
  return 0;
  */

  // Process user entries
  bool shutdown = true;
  bool enableSqwe = false;
  struct tm tm_ = {0};
  ds1307_i2c::OutLevel outLevel = ds1307_i2c::High;
  while (!g_exitRequested) {
    DisplayMenuAndGetChoice();
    switch (g_userChoice.choice) {
    case 'a': // Increase digipot
      g_digiPot->increment();
      break;
    case 'b': // Decrease digipot
      g_digiPot->decrement();
      break;
    case 'c': // Shutdown pot
      g_digiPot->shutdown(shutdown);
      // Flip shutdown flag 
      shutdown = !shutdown;
      break;
    case 'd': // Reset
      g_digiPot->reset();
      break;
    case 'e': // Read digipot voltage
      cout << "Digipot voltage: " << setprecision(5) << g_adc->read(mcp320x_spi::CH7) << endl;
      break;
    case 'f': // Read LED voltage 
      cout << "Led voltage: " << setprecision(5) << g_adc->read(mcp320x_spi::CH1) << endl;
      break;
    case 'g': // Read time
      tm_ = g_rtc->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case 'h': // Set time
      g_rtc->setTime(time(NULL));
      tm_ = g_rtc->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case 'i': // Set mode 24H
      g_rtc->setHourMode(ds1307_i2c::mode24H);
      tm_ = g_rtc->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case 'j': // Set mode 12H
      g_rtc->setHourMode(ds1307_i2c::mode12H);
      tm_ = g_rtc->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case 'k': // Enable/disable SQWE
      enableSqwe = !enableSqwe;
      g_rtc->enableSqwe(enableSqwe);
      break;
    case 'l': // Enable/disable Clock
      enableClock = !enableClock;
      g_rtc->enableClock(enableClock);
      break;
    case 'm': { // Enable/disable Clock
        ds1307_i2c::OutLevel outLevel = static_cast<ds1307_i2c::OutLevel>(~static_cast<unsigned char>(outLevel));
        g_rtc->setOutLevel(outLevel);
      }
      break;
    case 'n': { // Write RTC memory
        std::vector<unsigned char> buffer;
        buffer.push_back(0xde);
        buffer.push_back(0x5e);
        buffer.push_back(0xad);
        if (g_rtc->writeMemory(9, buffer) < 0) {
	  cerr << "Failed to call ds1307_i2c::writeMemory: " << strerror(errno) << endl;
	} else {
	  cout << "ds1307_i2c::writeMemrory done at address 9" << endl;
	}
      }
      break;
    case 'o': { // Read RTC memory
        std::vector<unsigned char> buffer(3);
        g_rtc->readMemory(9, buffer);
	cout << "ds1307_i2c::readMemrory at address 9: #bytes read: " << dec << buffer.size()<< endl;
	for (auto it = buffer.cbegin(); it != buffer.cend(); ++it) {
	  cout << hex << setw(2) << setfill('0') << "0x" << *it << ' ';
	} // End of 'for' statement
	cout << endl;
      }
      break;
    case 'p': // Led on
      g_xgpio->digitalWrite(XIO_PORT_LED, XIO_IO_LED, mcp23x17_i2c::High);
      break;
    case 'q': // Led off
      g_xgpio->digitalWrite(XIO_PORT_LED, XIO_IO_LED, mcp23x17_i2c::Low);
      break;
    case 'r': // Read led off
      mcp23x17_i2c::IoLevel ioLevel;
      g_xgpio->digitalRead(XIO_PORT_LED, XIO_IO_LED, ioLevel);
      cout << "mcp23x17_i2c::PortA/Pin3 level: " << (char *)((ioLevel == mcp23x17_i2c::High) ? "High" : "Low") << endl;
      break;
    case 's': // ADC on
      g_xgpio->digitalWrite(XIO_PORT_ADC, XIO_IO_ADC, mcp23x17_i2c::High);
      break;
    case 't': // ADC off
      g_xgpio->digitalWrite(XIO_PORT_ADC, XIO_IO_ADC, mcp23x17_i2c::Low);
      break;
    case 'u': // Read led off
      cout << "XIO ADC voltage: " << setprecision(5) << g_adc->read(mcp320x_spi::CH0) << endl;
      break;
    case 'v': // Read led off
      cout << "PB voltage: " << setprecision(5) << g_adc->read(mcp320x_spi::CH4) << endl;
      mcp23x17_i2c::IoLevel isPbPushed;
      g_xgpio->digitalRead(XIO_PORT_PB, XIO_IO_PB, isPbPushed);
      cout << "PB level: " << (char *)((isPbPushed == mcp23x17_i2c::High) ? "High" : "Low") << endl;
      break;
    case 'w':
      cout << "[" << _addressIdx << "] Store " << hex << setw(2) << setfill('0') << _memoryValues[_addressIdx] << " at address " << dec << _addresses[_addressIdx] << endl;
      if (g_eeprom->write(_addresses[_addressIdx], _memoryValues[_addressIdx]) < 0) {
	cerr << "Failed to call mcp24lc_i2c::write: " << strerror(errno) << endl;
      }
      break;
    case 'x': {
        short value = 0;
        if (g_eeprom->read(_addresses[_addressIdx], (short *)&value) < 0) {
	  cerr << "Failed to call mcp24lc_i2c::read: " << strerror(errno) << endl;
	} else {
	  cout << "[" << _addressIdx << "] Read " << hex << setw(2) << setfill('0') << value << " at address " << dec << _addresses[_addressIdx] << endl;
	  _addressIdx = (_addressIdx + 1) % MAX_ADDRESSES_LENGTH;
	}
      }
      break;
    case 'y': { // Read Temperature Sensor
        float temp = g_tempSensor->readTemp();
        cout << "Ambiant temperature: " << temp << endl;
      }
      break;
    case 'z': { // Set arlert temp
        float alertTemp = 30; // 30 degrees Celsius
	g_tempSensor->setAlertTemp(alertTemp);
        cout << "Alert temperature: " << alertTemp << endl;
      }
      break;
    case '0': // Read time
      tm_ = g_rtc1->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case '1': // Set time
      g_rtc1->setTime(time(NULL));
      tm_ = g_rtc1->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case '2': // Set mode 24H
      g_rtc1->setHourMode(mcp7941x_i2c::mode24H);
      tm_ = g_rtc1->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case '3': // Set mode 12H
      g_rtc1->setHourMode(mcp7941x_i2c::mode12H);
      tm_ = g_rtc1->getTime();
      cout << "Curent time: " << asctime(&tm_) << endl;
      break;
    case '4': // Enable/disable SQWE
      enableSqwe = !enableSqwe;
      g_rtc1->enableSqwe(enableSqwe);
      break;
    case '5': // Enable/disable Clock
      enableClock = !enableClock;
      g_rtc1->enableClock(enableClock);
      break;
    case '6': { // Enable/disable Clock
        mcp7941x_i2c::OutLevel outLevel = static_cast<mcp7941x_i2c::OutLevel>(~static_cast<unsigned char>(outLevel));
        g_rtc1->setOutLevel(outLevel);
      }
      break;
    case '7': { // Write RTC memory
        std::vector<unsigned char> buffer;
        buffer.push_back(0xde);
        buffer.push_back(0x5e);
        buffer.push_back(0xad);
        if (g_rtc1->writeMemory(9, buffer) < 0) {
	  cerr << "Failed to call mcp7941x_i2c::writeMemory: " << strerror(errno) << endl;
	} else {
	  cout << "mcp7941x_i2c::writeMemrory done at address 9" << endl;
	}
      }
      break;
    case '8': { // Read RTC memory
        std::vector<unsigned char> buffer(3);
        g_rtc1->readMemory(9, buffer);
	cout << "mcp7941x_i2c::readMemrory at address 9: #bytes read: " << dec << buffer.size()<< endl;
	for (auto it = buffer.cbegin(); it != buffer.cend(); ++it) {
	  cout << hex << setw(2) << setfill('0') << "0x" << *it << ' ';
	} // End of 'for' statement
	cout << endl;
      }
      break;
    } // End of 'switch' statement

  } // End of 'while' statement

  cout << "Terminate application" << endl;
  return EXIT_SUCCESS;
} // End of 'main' function 

