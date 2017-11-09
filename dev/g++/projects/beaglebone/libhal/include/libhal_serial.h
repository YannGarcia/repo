/**
 * \file    libhal_spi.h
 * \brief   Main header file for the Serial Hardware Abstract Layer library.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#if !defined(__BEAGLEBONEBLACK_SERIAL_H__)
#define __BEAGLEBONEBLACK_SERIAL_H__

#include "libhal.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * \fn int serial_open(const char *p_device, const int p_baud_rate)
   * \brief Open and initialise the serial port (8, N)
   * \param[in] p_device    The serial device name
   * \param[in] p_baud_rate      The baud value
   * \return The serial file descriptor on success, -1 otherwise
   */
  extern int serial_open(const char *p_device, const int p_baud_rate);
  /**
   * \fn void serial_close(const int p_fd)
   * \brief Close the serial port
   * \param[in] p_fd    The serial file descriptor
   */
  extern void serial_close(const int p_fd);
  /**
   * \fn void serial_flush(const int p_fd)
   * \brief Flush the serial buffers (both TX & RX)
   * \param[in] p_fd    The serial file descriptor
   */
  extern void serial_flush(const int p_fd);
  /**
   * \fn void serial_put_char(const int p_fd, const uint8_t p_char)
   * \brief Send a character
   * \param[in] p_fd    The serial file descriptor
   * \param[in] p_char  The character to send
   */
  extern void serial_put_char(const int p_fd, const uint8_t p_char);
  /**
   * \fn void serial_put_string(const int p_fd, const char *p_string)
   * \brief Send a NULL terminated string
   * \param[in] p_fd        The serial file descriptor
   * \param[in] p_string    The string to send
   */
  extern void serial_put_string(const int p_fd, const char *p_string);
  /**
   * \fn void serial_printf(const int p_fd, const char *p_message, ...)
   * \brief Send a printf-like formated message
   * \param[in] p_fd        The serial file descriptor
   * \param[in] p_message   The printf-like formated message
   * \param[in] ...         The arguments
   */
  extern void serial_printf(const int p_fd, const char *p_message, ...);
  /**
   * \fn int serial_data_available(const int p_fd)
   * \brief Retrieve the number of bytes avalable for read
   * \param[in] p_fd    The serial file descriptor
   * \return The number of bytes avalable for read on success, -1 otherwise
   */
  extern int serial_data_available(const int p_fd);
  /**
   * \fn int serial_get_char(const int p_fd)
   * \brief Retrieve a single character
   * \param[in] p_fd    The serial file descriptor
   * \return The read character on success, -1 otherwise
   * \remark NULL is a valid character
   * \remark This function will time-out after 10 seconds (\see serial_open)
   */
  extern int serial_get_char(const int p_fd);

#ifdef __cplusplus
}
#endif

#endif // __BEAGLEBONEBLACK_SERIAL_H__
