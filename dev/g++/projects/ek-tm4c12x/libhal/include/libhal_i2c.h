/**
 * \file      libhal_i2c.h
 * \brief     Header file for the I2C Hardware Abstract Layer library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 * \see Using Feature Set of I2C Master on TM4C129x Microcontrollers - SPMA073–July 2015
 * <a href="https://www.kernel.org/doc/Documentation/i2c/dev-interface">See I2C documentation</a>
 */
#pragma once

#include "libhal.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * \fn int32_t libhal_i2c_setup(const uint8_t p_device_id)
   * \brief Open the default I2C device, and register the target device
   * \param[in] p_i2c_bus_id I2C bus identifier
   * \param[in] p_device_address I2C device address
   * \return The I2C file descriptor on success, -1 otherwise
   */
  extern int32_t libhal_i2c_setup(const uint8_t p_i2c_bus_id, const uint8_t p_device_address);
  /**
   * \fn int32_t libhal_i2c_setup_interface(const int32_t p_device_id)
   * \brief Open the specified I2C device, register the target device
   * \param[in] p_bus_id         I2C bus identifier. Default: /dev/i2c-1
   * \param[in] p_device_address I2C device address
   * \return The I2C file descriptor on success, -1 otherwise
   */
  extern int32_t libhal_i2c_setup_interface(const char *p_i2c_bus_id, const uint8_t p_device_address);
  /**
   * \fn int32_t libhal_i2c_read(const int32_t p_fd)
   * \brief Read one byte from openend I2C bus
   * \param[in] p_fd    The I2C file descriptor 
   * \return The 8-bits read on success, -1 otherwise
   */
  extern int32_t libhal_i2c_read(const int32_t p_fd);
  /**
   * \fn int32_t libhal_i2c_read_register8(const int32_t p_fd, const uint8_t p_register)
   * \brief Read a 8-bits value from a register on the device
   * \param[in] p_fd        The I2C file descriptor 
   * \param[in] p_register  The 8 bits length register to be read 
   * \return The 8-bits read on success, -1 otherwise
   */
  extern int32_t libhal_i2c_read_register8(const int32_t p_fd, const uint8_t p_register);
  /**
   * \fn int32_t libhal_i2c_read_register16(const int32_t p_fd, const uint16_t p_register)
   * \brief Read a 16-bits value from a register on the device
   * \param[in] p_fd        The I2C file descriptor 
   * \param[in] p_register  The 16 bits length register to be read 
   * \return The 16-bits value read on success, -1 otherwise
   */
  extern int32_t libhal_i2c_read_register16(const int32_t p_fd, const uint8_t p_register);
  /**
   * \fn int32_t libhal_i2c_ext_read_register8(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register)
   * \brief Read a 8-bits value from a 16 bit address register on the device, MSB first
   * \param[in] p_fd             The I2C file descriptor 
   * \param[in] p_device_address The I2C device address
   * \param[in] p_register       The 8 bits length register to be read 
   * \return The 8-bits read on success, -1 otherwise
   */
  extern int32_t libhal_i2c_ext_read_register8(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register);
  /**
   * \fn int32_t libhal_i2c_ext_read_register16(const int32_t p_fd, const uint16_t p_register)
   * \brief Read a 8-bits value from a 16 bit address register on the device
   * \param[in] p_fd             The I2C file descriptor 
   * \param[in] p_device_address The I2C device address
   * \param[in] p_register       The 16 bits length register to be read 
   * \return The 8-bits read on success, -1 otherwise
   */
  extern int32_t libhal_i2c_ext_read_register16(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register);
  /**
   * \fn int32_t libhal_i2c_write(const int32_t p_fd, const int32_t p_buffer)
   * \brief Write one byte from openend I2C bus to the 8 bits address register
   * \param[in] p_fd        The I2C file descriptor 
   * \param[in] p_buffer    The byte to write
   * \return 0 on success, -1 otherwise
   */
  extern int32_t libhal_i2c_write(const int32_t p_fd, const int32_t p_value);
  /**
   * \fn int32_t libhal_i2c_write_register8(const int32_t p_fd, const uint32_t p_register, const uint8_t p_value)
   * \brief Write one byte from openend I2C bus to the 8 bits address register
   * \param[in] p_fd        The I2C file descriptor 
   * \param[in] p_register  The register to write
   * \param[in] p_value     The 8-bits value to write
   * \return 0 on success, -1 otherwise
   */
  extern int32_t libhal_i2c_write_register8(const int32_t p_fd, const uint8_t p_register, const uint8_t p_value);
  /**
   * \fn int32_t libhal_i2c_write_register16(const int32_t p_fd, const uint32_t p_register, const uint16_t p_value)
   * \brief Write one byte from openend I2C bus to the 16 bits address register, MSB first
   * \param[in] p_fd        The I2C file descriptor 
   * \param[in] p_register  The register to write
   * \param[in] p_value     The 16-bits value to write
   * \return 0 on success, -1 otherwise
   */
  extern int32_t libhal_i2c_write_register16(const int32_t p_fd, const uint8_t p_register, const uint16_t p_value);
  /**
   * \fn int32_t libhal_i2c_ext_write_register8(const int32_t p_fd, const uint8_t p_device_address, const uint32_t p_register, const uint8_t pvalue)
   * \brief Write one byte from openend I2C bus to the 16 bits address register
   * \param[in] p_fd             The I2C file descriptor 
   * \param[in] p_device_address The I2C device address
   * \param[in] p_register       The register to write
   * \param[in] p_value          The 8-bits value to write
   * \return 0 on success, -1 otherwise
   */
  extern int32_t libhal_i2c_ext_write_register8(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register, const uint8_t p_value);
  /**
   * \fn int32_t libhal_i2c_ext_write_register16(const int32_t p_fd, const uint8_t p_device_address, const uint32_t p_register, const uint16_t p_value)
   * \brief Write one byte from openend I2C bus to the 16 bits address register, MSB first
   * \param[in] p_fd             The I2C file descriptor 
   * \param[in] p_device_address The I2C device address
   * \param[in] p_register       The register to write
   * \param[in] p_value          The 16-bits value to write
   * \return 0 on success, -1 otherwise
   */
  extern int32_t libhal_i2c_ext_write_register16(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register, const uint16_t p_value);
  /**
   * \fn int32_t libhal_i2c_close(const uint8_t p_channel)
   * \brief Close the I2C channel
   * \param[in] p_fd The file descriptor to close
   * \return 0 on success, -1 otherwise
   */
  extern int32_t libhal_i2c_close(const int32_t p_fd);

#ifdef __cplusplus
}
#endif
