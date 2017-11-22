/*!
 * \file      main.c
 * \brief     Application to validate libhal libary
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2016-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   1.0
 */
#include <limits.h>

#include "libhal.h" /* Hardware Abstraction Level library (Beagle Bone, Raspbery MBED & TI launchpads */
#include "libhal_serial.h"

/*!<
 * @defgroup Booststarter #1 pin assignment
 * @{
 */
#define LED1       p3  /*!< On board LED */
/*!<
 * @brief Main function of the bouncing robot software application
 * @return Shall never returned
 */
int32_t main(void) {
  int32_t uart0;           /*!< UART handle */
  uint32_t counter = 0;

  /* Initialise the HAL */
  libhal_setup();

  digital_write(LED1, digital_state_high);

  // Open serial console
  uart0 = serial_open("/dev/tty0", 115200);

  while (true) {
    serial_printf(uart0, "Please push User button #1 (%d)\r\n", counter++);

    wait_ms(4000);
    
    serial_printf(uart0, "tic tac toc\r\n");
  } /* End of 'while' statement */

  return 0;
} /* End of main function*/

