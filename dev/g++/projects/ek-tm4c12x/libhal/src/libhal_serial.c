/**
 * @file      libhal_serial.c
 * @brief     Main implementation file for the Serial Hardware Abstract Layer library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 * @see Tiva™ TM4C129ENCPDT Microcontroller Clause 19
 */

#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"

#include "libhal_serial.h"

/*!< UART module parameters */
static uint32_t uart_modules[3][7] = {
  { SYSCTL_PERIPH_UART0, UART0_BASE, GPIO_PA0_U0RX, GPIO_PA1_U0TX, GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_PIN_1 },
  { SYSCTL_PERIPH_UART1, UART1_BASE, GPIO_PB0_U1RX, GPIO_PB1_U1TX, GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_1 },
  { SYSCTL_PERIPH_UART2, UART2_BASE, GPIO_PA6_U2RX, GPIO_PA7_U2TX, GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_7 }
};
/*!< Mapping from an integer between 0 and 15 to its ASCII character equivalent. */
static const char * const uint8_to_hex = "0123456789abcdef";

/**
 * @fn void serial_vprintf(const int32_t p_fd, const char *p_string, va_list p_va_list)
 * @brief UART vprintf function
 * @param[in] p_fd       The serial file descriptor
 * @param[in] p_string   The 'ptintf' format string
 * @param[in] p_va_list  The 'printf' parameters
 */
void serial_vprintf(const int32_t p_fd, const char *p_string, va_list p_va_list);

int32_t serial_open(const char *p_device, const int32_t p_baud_rate) {

  // TODO Case /dev/tty0: module 0...
  uint8_t fd = 0;
  // Configure the UART for 115,200, 8-N-1 operation.
  SysCtlPeripheralEnable(uart_modules[fd][0]/*SYSCTL_PERIPH_UART0*/);
  UARTConfigSetExpClk(uart_modules[fd][1]/*UART0_BASE*/, get_sys_clock_freq(), p_baud_rate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  // Set GPIO A0 and A1 as UART pins.
  GPIOPinConfigure(uart_modules[fd][2]/*GPIO_PA0_U0RX*/);
  GPIOPinConfigure(uart_modules[fd][3]/*GPIO_PA1_U0TX*/);
  GPIOPinTypeUART(uart_modules[fd][4]/*GPIO_PORTA_BASE*/, uart_modules[fd][5]/*GPIO_PIN_0*/ | uart_modules[fd][6]/*GPIO_PIN_1*/);
  UARTEnable(uart_modules[fd][1]/*UART0_BASE*/);

  return fd;
}

void serial_close(const int32_t p_fd) {
  UARTEnable(uart_modules[p_fd][1]/*UART0_BASE*/);
}

void serial_flush(const int32_t p_fd) {
  // Nothing to do
}

void serial_put_char (const int32_t p_fd, const uint8_t p_char) {
  UARTCharPut(uart_modules[p_fd][1], (unsigned char)p_char);
}

void serial_put_string(const int32_t p_fd, const char *p_string) {
  // Sanity check
  if ((p_string == NULL) || (strlen(p_string) == 0)) {
    return;
  }
  
  // Loop while there are more characters to send.
  uint32_t ui32Count = strlen(p_string);
  uint8_t *ptr = (uint8_t *)p_string;
  while(ui32Count--) {
    // Write the next character to the UART
    UARTCharPut(uart_modules[p_fd][1]/*UART0_BASE*/, *ptr++);
  } // End of 'while' statement
}

void serial_printf(const int32_t p_fd, const char *p_message, ...) {
  va_list argp;

  va_start(argp, p_message);
  serial_vprintf(p_fd, p_message, argp);
  va_end(argp);
}

int32_t serial_data_available(const int32_t p_fd) {
  if (!UARTCharsAvail(uart_modules[p_fd][1])) {
    return -1;
  }

  return 1;
}

int32_t serial_get_char(const int32_t p_fd) {
  return UARTCharGet(uart_modules[p_fd][1]) & 0xFF;
}

void serial_vprintf(const int32_t p_fd, const char *p_string, va_list p_va_list) {
    uint32_t uint32_idx, uint32_value, uint32_pos, ui32Count, uint32_base, ui32Neg, idx;
    char *str, buffer[16], cFill;

    // Loop while there are more characters in the string.
    while(*p_string) {
        //Find the first non-% character, or the end of the string.
        for(uint32_idx = 0; (p_string[uint32_idx] != '%') && (p_string[uint32_idx] != '\0'); uint32_idx++);
        // Write this portion of the string.
        for (idx = 0; idx < uint32_idx; idx++) {
            UARTCharPut(uart_modules[p_fd][1], *(p_string + idx));
        }
        // Skip the portion of the string that was written.
        p_string += uint32_idx;
        // See if the next character is a %.
        if (*p_string == '%') {
            // Skip the %.
            p_string++;
            // Set the digit count to zero, and the fill character to space (in other words, to the defaults).
            ui32Count = 0;
            cFill = ' ';
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for using not one but two of the beasts.
again:
            // Determine how to handle the next character.
            switch(*p_string++) {
                // Handle the digit characters.
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': {
                    // If this is a zero, and it is the first digit, then the fill character is a zero instead of a space.
                    if ((p_string[-1] == '0') && (ui32Count == 0)) {
                        cFill = '0';
                    }
                    // Update the digit count.
                    ui32Count *= 10;
                    ui32Count += p_string[-1] - '0';
                    // Get the next character.
                    goto again;
                }
                case 'c': {// Handle the %c command.
                    // Get the value from the varargs.
                    uint32_value = va_arg(p_va_list, uint32_t);
                    // Print out the character.
                    UARTCharPut(uart_modules[p_fd][1], (char)uint32_value);
                    // This command has been handled.
                    break;
                }
                case 'd':
                case 'i': { // Handle the %d and %i commands.
                    // Get the value from the varargs.
                    uint32_value = va_arg(p_va_list, uint32_t);
                    // Reset the buffer position.
                    uint32_pos = 0;
                    // If the value is negative, make it positive and indicate that a minus sign is needed.
                    if ((int32_t)uint32_value < 0) {
                        // Make the value positive.
                        uint32_value = -(int32_t)uint32_value;
                        // Indicate that the value is negative.
                        ui32Neg = 1;
                    } else {
                        //Indicate that the value is positive so that a minus sign isn't inserted.
                        ui32Neg = 0;
                    }
                    // Set the base to 10.
                    uint32_base = 10;
                    // Convert the value to ASCII.
                    goto convert;
                }
                case 's': {// Handle the %s command.
                    // Get the string pointer from the varargs.
                    str = va_arg(p_va_list, char *);
                    // Determine the length of the string.
                    for(uint32_idx = 0; str[uint32_idx] != '\0'; uint32_idx++);
                    // Write the string.
                    for (idx = 0; idx < uint32_idx; idx++) {
                        UARTCharPut(uart_modules[p_fd][1], *(str + idx));
                    }
                    // Write any required padding spaces
                    if (ui32Count > uint32_idx) {
                        ui32Count -= uint32_idx;
                        while(ui32Count--) {
                            UARTCharPut(uart_modules[p_fd][1], ' ');
                        }
                    }
                    // This command has been handled.
                    break;
                }
                case 'u': { // Handle the %u command.
                    // Get the value from the varargs.
                    uint32_value = va_arg(p_va_list, uint32_t);
                    // Reset the buffer position.
                    uint32_pos = 0;
                    // Set the base to 10.
                    uint32_base = 10;
                    // Indicate that the value is positive so that a minus sign isn't inserted.
                    ui32Neg = 0;
                    // Convert the value to ASCII.
                    goto convert;
                }
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p': {
                    // Get the value from the varargs.
                    uint32_value = va_arg(p_va_list, uint32_t);
                    // Reset the buffer position.
                    uint32_pos = 0;
                    // Set the base to 16.
                    uint32_base = 16;
                    // Indicate that the value is positive so that a minus sign isn't inserted.
                    ui32Neg = 0;
                    // Determine the number of digits in the string version of the value.
convert:
                    for(uint32_idx = 1; (((uint32_idx * uint32_base) <= uint32_value) && (((uint32_idx * uint32_base) / uint32_base) == uint32_idx)); uint32_idx *= uint32_base, ui32Count--);
                    // If the value is negative, reduce the count of padding characters needed.
                    if (ui32Neg) {
                        ui32Count--;
                    }
                    // If the value is negative and the value is padded with zeros, then place the minus sign before the padding.
                    if (ui32Neg && (cFill == '0')) {
                        // Place the minus sign in the output buffer.
                        buffer[uint32_pos++] = '-';
                        // The minus sign has been placed, so turn off the negative flag.
                        ui32Neg = 0;
                    }
                    // Provide additional padding at the beginning of the string conversion if needed.
                    if ((ui32Count > 1) && (ui32Count < 16)) {
                        for(ui32Count--; ui32Count; ui32Count--) {
                            buffer[uint32_pos++] = cFill;
                        }
                    }
                    // If the value is negative, then place the minus sign before the number.
                    if (ui32Neg) {
                        // Place the minus sign in the output buffer.
                        buffer[uint32_pos++] = '-';
                    }
                    // Convert the value into a string.
                    for(; uint32_idx; uint32_idx /= uint32_base) {
                        buffer[uint32_pos++] = uint8_to_hex[(uint32_value / uint32_idx) % uint32_base];
                    }
                    // Write the string.
                    for (idx = 0; idx < uint32_pos; idx++) {
                        UARTCharPut(uart_modules[p_fd][1], *(buffer + idx));
                    }
                    // This command has been handled.
                    break;
                }
                case '%': {// Handle the %% command.
                    // Simply write a single %.
                    UARTCharPut(uart_modules[p_fd][1], *(p_string - 1));
                    // This command has been handled.
                    break;
                }
                default: {// Handle all other commands.
                    // Indicate an error.
                    serial_put_string(p_fd, "ERROR");
                    // This command has been handled.
                    break;
                }
            }
        }
    }
} // End of function serial_vprintf

