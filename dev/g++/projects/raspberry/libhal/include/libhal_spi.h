/**
 * @File    libhal_spi.h
 * @brief   Main header file for the SPI Hardware Abstract Layer library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include "libhal.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @fn int32_t libhal_spi_setup(const uint8_t p_channel, const uint32_t p_speed)
   * @brief Initialise the SPI component with SPI Mode 0,0
   * @param[in] p_channel   Channel to be used
   * @param[in] p_speed     SPI clock frequency
   * @return 0 on success, -1 otherwise
   */
  int32_t libhal_spi_setup(const uint8_t p_channel, const uint32_t p_speed);
   /**
    * @fn int32_t libhal_spi_get_fd(const uint8_t p_channel)
    * @brief Retrieve file-descriptor for the given channel (used by ioctl function) 
    * @param[in] p_channel   Channel to be used
    * @return The file descriptor on success, -1 otherwise
    */
  int32_t libhal_spi_get_fd(const uint8_t p_channel);
   /**
    * @fn int32_t libhal_spi_data_read_write(const uint8_t p_channel, const uint8_t *p_transfer_word_speed, const uint32_t p_length)
    * @brief Read and write blocks of data over the SPI bus 
    * @param[in] p_channel   Channel to be used
    * @param[in] p_buffer    The buffer for receiving data or to send
    * @param[in] p_length    The length of the data to process
    * @return The number of bytes processed on success, -1 otherwise
    * @remark To make MISO working fine, the SPI_CLOCK shall be configured as INPUT in the DTS file
    */
  int32_t libhal_spi_data_read_write(const uint8_t p_channel, const uint8_t *p_buffer, const uint32_t p_length);
  /**
   * @fn int32_t libhal_spi_format(const int32_t p_channel, const uint8_t p_bpw, const uint8_t p_mode)
   * @brief Change SPI frequency and SPI mode 
   * @param[in] p_channel               Channel to be used
   * @param[in] p_transfer_word_speed   The new SPI frequency
   * @param[in] p_mode                  The new SPI mode
   * @return 0 on success, -1 otherwise
   */
  int32_t libhal_spi_format(const uint8_t p_channel, const uint8_t p_transfer_word_speed, const uint8_t p_mode);
  /**
   * @fn int32_t libhal_spi_close(const uint8_t p_channel)
   * @brief Close the SPI channel
   * @param[in] p_fd The file descriptor to close
   * @return 0 on success, -1 otherwise
   */
  int32_t libhal_spi_close(const int32_t p_fd);

#ifdef __cplusplus
}
#endif
