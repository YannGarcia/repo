/**
 * @file      libhal_serial.h
 * @brief     Main header file for the Serial Hardware Abstract Layer library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2016 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 * @see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * @see       EK-TM4C129EXL Firmware Development Package User's Guide
 * @see       EK-TM4C1294XL Firmware Development Package User's Guide
 * @see       Tiva™ TM4C129ENCPDT Microcontroller Clause 19
 */
#pragma once

#include "libhal.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @fn int32_t serial_open(const char *p_device, const int32_t p_baud_rate)
   * @brief Open and initialise the serial port (8, N)
   * @param[in] p_device    The serial device name (e.g. /dev/tty0)
   * @param[in] p_baud_rate The baud value
   * @return The serial file descriptor on success, -1 otherwise
   */
  extern int32_t serial_open(const char *p_device, const int32_t p_baud_rate);
  /**
   * @fn void serial_close(const int32_t p_fd)
   * @brief Close the serial port
   * @param[in] p_fd    The serial file descriptor
   */
  extern void serial_close(const int32_t p_fd);
  /**
   * @fn void serial_flush(const int32_t p_fd)
   * @brief Flush the serial buffers (both TX & RX)
   * @param[in] p_fd    The serial file descriptor
   */
  extern void serial_flush(const int32_t p_fd);
  /**
   * @fn void serial_put_char(const int32_t p_fd, const uint8_t p_char)
   * @brief Send a character
   * @param[in] p_fd    The serial file descriptor
   * @param[in] p_char  The character to send
   */
  extern void serial_put_char(const int32_t p_fd, const uint8_t p_char);
  /**
   * @fn void serial_put_string(const int32_t p_fd, const char *p_string)
   * @brief Send a NULL terminated string
   * @param[in] p_fd        The serial file descriptor
   * @param[in] p_string    The string to send
   */
  extern void serial_put_string(const int32_t p_fd, const char *p_string);
  /**
   * @fn void serial_printf(const int32_t p_fd, const char *p_message, ...)
   * @brief Send a printf-like formated message
   * @param[in] p_fd        The serial file descriptor
   * @param[in] p_message   The printf-like formated message
   * @param[in] ...         The printf arguments
   */
  extern void serial_printf(const int32_t p_fd, const char *p_message, ...);
  /**
   * @fn int32_t serial_data_available(const int32_t p_fd)
   * @brief Retrieve the number of bytes avalable for read
   * @param[in] p_fd    The serial file descriptor
   * @return The number of bytes avalable for read on success, -1 otherwise
   */
  extern int32_t serial_data_available(const int32_t p_fd);
  /**
   * @fn int32_t serial_get_char(const int32_t p_fd)
   * @brief Retrieve a single character
   * @param[in] p_fd    The serial file descriptor
   * @return The read character on success, -1 otherwise
   * @remark NULL is a valid character
   * @remark This function will time-out after 10 seconds (\see serial_open)
   */
  extern int32_t serial_get_char(const int32_t p_fd);

#ifdef __cplusplus
}
#endif

