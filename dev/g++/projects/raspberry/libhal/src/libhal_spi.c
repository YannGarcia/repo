/**
 * @file    libhal_spi.c
 * @brief   Main implementation file for the SPI Hardware Abstract Layer library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <unistd.h> // Used by close() function

#include "libhal_spi.h"

#include "wiringPiSPI.h"

int32_t libhal_spi_setup(const uint8_t p_channel, const uint32_t p_speed) {
  return wiringPiSPISetup((int)p_channel, (int)p_speed);
}

int32_t libhal_spi_data_read_write(const uint8_t p_channel, const uint8_t *p_buffer, const uint32_t p_length) {
  return wiringPiSPIDataRW((int)p_channel, (unsigned char *)p_buffer, (int)p_length);
}

int32_t libhal_spi_format(const uint8_t p_channel, const uint8_t p_transfer_word_speed, const uint8_t p_mode) {
  return wiringPiSPISetupMode((int)p_channel, (int)p_transfer_word_speed, (int)p_mode);
}

int32_t libhal_spi_close(const int32_t p_fd) {
  // Sanity check
  if (p_fd == -1) {
    return -1;
  }
  
  return close(p_fd);
}
