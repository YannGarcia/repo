/**
 * @file    libhal_spi.c
 * @brief   Main implementation file for the Serial Hardware Abstract Layer library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

#include "libhal_serial.h"

uint32_t uart_modules[3][7] = {
  { SYSCTL_PERIPH_UART0, UART0_BASE, GPIO_PA0_U0RX, GPIO_PA1_U0TX, GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_PIN_1 },
  { SYSCTL_PERIPH_UART1, UART1_BASE, GPIO_PB0_U1RX, GPIO_PB1_U1TX, GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_1 },
  { SYSCTL_PERIPH_UART2, UART2_BASE, GPIO_PA6_U2RX, GPIO_PA7_U2TX, GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_7 }
};

int serial_open(const char *p_device, const int p_baud_rate) {

  // TODO Case /dev/tty0: module 0...
  uint8_t fd = 0;
  // Configure the UART for 115,200, 8-N-1 operation.
  SysCtlPeripheralEnable(uart_modules[fd][0]/*SYSCTL_PERIPH_UART0*/);
  UARTConfigSetExpClk(uart_modules[fd][1]/*UART0_BASE*/, get_sys_clock_freq(), p_baud_rate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  // Set GPIO A0 and A1 as UART pins.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTEnable(fd);

  return fd;
}

void serial_close(const int p_fd) {
  UARTEnable(p_fd);
}

void serial_flush(const int p_fd) {
  // Nothing to do
}

void serial_put_char (const int p_fd, const uint8_t p_char) {
  UARTCharPut(p_fd, (unsigned char)p_char);
}

void serial_put_string(const int p_fd, const char *p_string) {
  // Sanity check
  if ((p_string == NULL) || (strlen(p_string) == 0)) {
    return;
  }
  
  // Loop while there are more characters to send.
  uint32_t ui32Count = strlen(p_string);
  uint8_t *ptr = (uint8_t *)p_string;
  while(ui32Count--) {
    // Write the next character to the UART
    UARTCharPutNonBlocking(UART0_BASE, *ptr++);
  } // End of 'while' statement
}

void serial_printf(const int p_fd, const char *p_message, ...) {
  va_list argp;
  char *buffer = (char *)malloc(1024);

  va_start (argp, p_message);
  vsnprintf (buffer, 1023, p_message, argp);
  va_end (argp);

  serial_put_string(p_fd, buffer);
  free(buffer);
}

int serial_data_available(const int p_fd) {
  if (!UARTCharsAvail(p_fd)) {
    return -1;
  }

  return 1;
}

int serial_get_char(const int p_fd) {
  return UARTCharGet(p_fd) & 0xFF;
}
