//============================================================================
// Name        : HelloWorld.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : wiringBbb demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip> // Used for setprecision
#include <cstdio>

#include <unistd.h> // Used for usleep
#include <signal.h> // USe to handle signals
#include <sys/ioctl.h>
#include <linux/i2c.h> // Use direct I2C driver
#include <linux/i2c-dev.h>

#include "wiringBbb.h"
#include "wiringSerial.h"
#include "wiringBbbSPI.h"
#include "wiringBbbI2C.h" // Using I2C functions

using namespace std;

#define LED_RED       23 // P8_13 - gpio0_23 - GPIO[chip]_[pin]: Linux pin number = [chip] * 32 + [pin]
#define LED_GREEN     44 // P8_12 - gpio1_12 - GPIO[chip]_[pin]: Linux pin number = [chip] * 32 + [pin]
#define PWM1A_FAN     50 // P9_14 - gpio1_18
#define PWM2A_LCD     22 // P8.19 - gpio0_22
#define CS_ADC        51 // P9_16 - gpio1_19 
#define CS_DIGIPOT    48 // P9_15 - gpio1_16
#define LCD_EN        46 // P8_16 - gpio1_14
#define LCD_RW        27 // P8_17 - gpio0_27
#define LCD_RS        65 // P8_18 - gpio2_1
#define TEMP_ALERT    67 // P8_08 - gpio2_3
#define GPS_PPS       45 // P8_11 - gpio1_13

#define PWM2A_DIVISOR 64
#define PWM2A_CLOCK   64

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  std::cout << "Caught signal: " << signum << std::endl;

  ::digitalWrite(LED_GREEN, LOW);
  ::digitalWrite(LED_RED, LOW);
  ::wiringBbbShutdownSys();

  // Terminate program
  exit(signum);
} // End of function signal_callback_handler

int isr_gps_pps(void * p_data) {
  //  gpioDesc *gpio = static_cast<gpioDesc *>(p_data);
  std::cout << "GPS PPS" << std::endl;
  return 0;
}

/***********************************************************************
  Main application part
***********************************************************************/

int main(const int argc, const char * argv[]) {
  // Register signal and signal handler
  if (signal (SIGINT, signal_callback_handler) == SIG_ERR) {
    std::cerr << "Cannot handle SIGINT" << std::endl;
    exit (EXIT_FAILURE);
  }
  if (signal (SIGTERM, signal_callback_handler) == SIG_ERR) {
    std::cerr << "Cannot handle SIGTERM" << std::endl;
    exit (EXIT_FAILURE);
  }

  // Initialize the BBB library
  int result = ::wiringBbbSetupSys();

  // Setup pin mode
  ::pinMode(LED_GREEN, OUTPUT);
  ::pinMode(LED_RED, OUTPUT);
  //::pinMode(CS_ADC, OUTPUT);
  /*::pinMode(CS_DIGIPOT, OUTPUT);
  ::pinMode(LCD_EN, OUTPUT);
  ::pinMode(LCD_RW, OUTPUT);
  ::pinMode(LCD_RS, OUTPUT);*/
  ::pinMode(GPS_PPS, INPUT);

  // Set pins output
  ::digitalWrite(LED_GREEN, HIGH);
  ::digitalWrite(LED_RED, HIGH);
  ::digitalWrite(CS_ADC, HIGH); // Disable device
  /*::digitalWrite(CS_DIGIPOT, HIGH); // Disable device
  ::digitalWrite(LCD_EN, HIGH); // Enable LCD
  ::digitalWrite(LCD_RW, LOW); // Set read mode
  ::digitalWrite(LCD_RS, LOW); // Set ???
  */

  ///////////////////////////////////////////////////////////////////////////////////////////
  // ISR
  ::wiringBbbISR(GPS_PPS, INT_EDGE_RISING, isr_gps_pps);

  ///////////////////////////////////////////////////////////////////////////////////////////
  // UART2

  // Send init sequence using $PMTK
  int fduart = ::serialOpen("/dev/ttyO2", 9600);
  if (fduart < 0) {
    std::cerr << "Unable to open serial device" << std::endl;
    ::wiringBbbShutdownSys();
    return -1;
  }
  std::string init("$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"); // Set frequency to 1Hz
  serialPuts(fduart, init.c_str());
  init.assign("$PMTK220,1000*1F"); // Turn on all the available data
  serialPuts(fduart, init.c_str());
  // Loop, getting and printing characters
  int i = 0;
  while(i++ < 100) {
    putchar(::serialGetchar(fduart)) ;
    fflush(stdout) ;
  } // End of 'while' statement
  close(fduart);
  
  ///////////////////////////////////////////////////////////////////////////////////////////
  // I2C
  /*int fdrtc = ::wiringBbbI2CSetup(0x68);
  if (fdrtc == -1) {
    printf("Failed to initialize RTC");
    ::wiringBbbShutdownSys();
    return -1;
  }
  unsigned char ctrlRegister = ::wiringBbbI2CReadReg8(fdrtc, 0x07);
  printf("ctrlRegister=0x%02x\n", ctrlRegister);
  unsigned char secondsRegister = ::wiringBbbI2CReadReg8(fdrtc, 0x00);
  printf("secondsRegister=0x%02x\n", secondsRegister);
  secondsRegister &= 0x7f; // Enable clock bit
  printf("Write secondsRegister=0x%02x\n", secondsRegister);
  ::wiringBbbI2CWriteReg8(fdrtc, 0x00, secondsRegister);
  sleep(2);
  secondsRegister = ::wiringBbbI2CReadReg8(fdrtc, 0x00);
  printf("#seconds=0x%02x\n", secondsRegister);  
  ::close(fdrtc);*/

  ///////////////////////////////////////////////////////////////////////////////////////////
  // PWM
  /*::pinMode(PWM2A_LCD, PWM_OUTPUT);
  ::pwmSetMode(PWM_MODE_MS);
  //::pwmSetRange(PWM2A_LCD, PWM2A_DIVISOR); // Divisor Clock / n
  //::pwmSetClock(PWM2A_LCD, PWM2A_CLOCK);  // Divisor PWM_FIXED_FREQUENCY / Clock
  ::pwmWrite(PWM2A_LCD, 30); // Duty cycle = 30%
  */

  ///////////////////////////////////////////////////////////////////////////////////////////
  // SPI
  /*int _spiChannel = 0;
  int fdadc = ::wiringBbbSPISetup(_spiChannel, 1000000);
  if (fdadc == -1) {
    printf("Failed to initialize ADC");
    ::wiringBbbShutdownSys();
    return -1;
  }

  ::digitalWrite(CS_ADC, LOW); // Enable device
  unsigned char _channels = 0x07; // Chennel number
  // Set start bit
  unsigned char mask = 0x04 | 0x02; // Start bit set to 1 - See DS21298E-page 19 Clause 5.0 SERIAL COMMUNICATIONS
  // Set channel address
  unsigned char cmd0;
  unsigned char cmd1;
  cmd0 = mask | ((_channels & 0x04) >> 2); // Extract D2 bit - See DS21298E-page 19 Clause 5.0 SERIAL COMMUNICATIONS
  cmd1 = _channels << 6; // MCP3204 has 8 channels in single-ended mode
  printf("SPI: send command %02x\n", cmd0);
  ::wiringBbbSPIDataRW(_spiChannel, &cmd0, sizeof(unsigned char)); // Don't care of the result - See DS21298E-page 21 Clause 6.1 Using the MCP3204/3208 with Microcontroller (MCU) SPI Ports
  printf("SPI: send command %02x\n", cmd1);
  ::wiringBbbSPIDataRW(_spiChannel, &cmd1, sizeof(unsigned char)); // DS21298E-page 21 See FIGURE 6-1: SPI Hwunication using 8-bit segments (Mode 0,0: SCLK idles low)
  unsigned short sample = cmd1 << 8;
  cmd0 = 0x00; // Dummy value
  printf("SPI: send command %02x\n", cmd0);
  ::wiringBbbSPIDataRW(_spiChannel, &cmd0, sizeof(unsigned char));
  sample |= cmd0;
  printf("SPI: 0x%02x - 0x%02x\n", (unsigned char)sample, (unsigned char)(sample >> 8));

  ::digitalWrite(CS_ADC, HIGH); // Enable device
  ::close(fdadc);
    
  sample >>= 1; // Adjust composite integer for 12 valid bits
  sample &= 0x0FFF; // Mask out upper nibble of integer
  float adcResult = (sample * 0.001225585);
  printf("SPI: %3.5f\n\n", adcResult);*/
  
  // Infinite loop
  do {
    ::digitalWrite(LED_RED, LOW);
    ::digitalWrite(LED_GREEN, HIGH);
    sleep(1);
    ::digitalWrite(LED_RED, HIGH);
    ::digitalWrite(LED_GREEN, LOW);
    sleep(1);
  } while (true);
  
  ::wiringBbbShutdownSys();
  return EXIT_SUCCESS;
}
