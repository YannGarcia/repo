/**
 * @file    libhal_spi.c
 * @brief   Main implementation file for the SPI Hardware Abstract Layer library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "libhal_spi.h"

const static char        *spi_dev0  = "/dev/spidev1.0"; /*!< BeagleBone Blck SPI component #1 */
const static char        *spi_dev1  = "/dev/spidev1.1"; /*!< BeagleBone Blck SPI component #2 */
static uint8_t           spi_mode[2];                   /*!< Current SPI Component modes */
static uint8_t           spi_transfer_word_speed[2];    /*!< Current SPI Component frequency */
const static uint16_t    spi_delay = 0;

static uint32_t          spi_speeds[2];
static int32_t               spi_fds[2];

int32_t libhal_spi_setup(const uint8_t p_channel, const uint32_t p_speed) {
  int32_t fd;
  uint8_t channel = p_channel & 0x01;

  if ((fd = open(channel == 0 ? spi_dev0 : spi_dev1, O_RDWR)) == -1) {
    libhal_failure(HAL_ALMOST, "Unable to open SPI device: %s\n", strerror (errno));
    return -1;
  }
  
  spi_speeds[channel] = p_speed;
  spi_fds[channel] = fd;
  spi_mode[channel] = 0; // No CS, Mode 0,0

  // Set SPI parameters
  if (ioctl(fd, SPI_IOC_WR_MODE, &spi_mode[channel]) == -1) {
    libhal_failure(HAL_ALMOST, "SPI Mode Change failure: %s\n", strerror (errno));
    return -1;
  }

  if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_transfer_word_speed[channel]) == -1) {
    libhal_failure(HAL_ALMOST, "SPI BPW Change failure: %s\n", strerror (errno));
    return -1;
  }

  if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &p_speed) == -1) {
    libhal_failure(HAL_ALMOST, "SPI Speed Change failure: %s\n", strerror (errno));
    return -1;
  }

  return fd;
}

int32_t libhal_spi_get_fd(const uint8_t p_channel) {
  return spi_fds[p_channel & 0x01];
}

int32_t libhal_spi_data_read_write(const uint8_t p_channel, const uint8_t *p_buffer, const uint32_t p_length) {
  struct spi_ioc_transfer spi;
  uint8_t channel = p_channel & 0x01;
  
  spi.tx_buf        = (unsigned long)p_buffer;
  spi.rx_buf        = (unsigned long)p_buffer;
  spi.len           = p_length;
  spi.delay_usecs   = spi_delay;
  spi.speed_hz      = spi_speeds[channel];
  spi.bits_per_word = spi_transfer_word_speed[channel];

  if (ioctl(spi_fds[channel], SPI_IOC_MESSAGE(1/*p_length*/), &spi) == -1) {
    libhal_failure(HAL_ALMOST, "SPI data exchange failure: %s\n", strerror (errno));
    return -1;
  }

  return 0;
}

int32_t libhal_spi_format(const uint8_t p_channel, const uint8_t p_transfer_word_speed, const uint8_t p_mode) {
  uint8_t channel = p_channel & 0x01;

  spi_mode[channel] = p_mode;
  spi_transfer_word_speed[channel] = p_transfer_word_speed;

  if (ioctl(spi_fds[channel], SPI_IOC_WR_MODE, &spi_mode[channel]) == -1) {
    libhal_failure(HAL_ALMOST, "SPI Mode Change failure: %s\n", strerror (errno));
    return -1;
  }

  if (ioctl(spi_fds[channel], SPI_IOC_WR_BITS_PER_WORD, &spi_transfer_word_speed[p_channel]) == -1) {
    libhal_failure(HAL_ALMOST, "SPI BPW Change failure: %s\n", strerror (errno));
    return -1;
  }

  return 0;
}

int32_t libhal_spi_close(const int32_t p_fd) {
  // Sanity check
  if (p_fd == -1) {
    return -1;
  }
  
  return close(p_fd);
}

