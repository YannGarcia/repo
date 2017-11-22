/*!
 * \file    i2c.hpp
 * \brief   Header file for I2C/SMBUS.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <cstdint>

/*!
 * \class i2c
 * \brief The I2C Master class used for communicating with I2C slave devices
 * <a href="https://www.kernel.org/doc/Documentation/i2c/dev-interface">See I2C documentation</a>
 */
class i2c {
protected:
  /*! The I2C bus identifier */
  uint8_t _bus_id;
  /*! The I2C file descriptor */
  int32_t _fd;
  /*! The I2C device address */
  uint8_t _device_address;

public:
  /*!
   * \brief Default constructor
   * I2C bus identifier is /dev/i2c-0
   * \param[in] p_device_address The I2C device address on 7 bits (e.g. 1 0 1 0 A3 A2 A1 R/W for 24LCxx)
   */
  i2c(const uint8_t p_device_address);
  /*!
   * \brief Constructor
   * \param[in] p_bus_id The I2C bus identifier (/dev/i2c-<p_bus_id>)
   * \param[in] p_device_address The I2C device address on 7 bits (e.g. 1 0 1 0 A3 A2 A1 R/W for 24LCxx)
   */
  i2c(const uint8_t p_bus_id, const uint8_t p_device_address);
  /*!
   * \brief Destructor
   */
  virtual ~i2c();
  
  /*!
   * \brief Read a 8-bits value from a register on the device
   * \param[in] p_register The 8 bits address length register to be read 
   * \param[out] p_value The 8 bits value of the specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint8_t p_register_id, uint8_t * p_value);
  /*!
   * \brief Read a 16-bits value from a register on the device
   * \param[in] p_register The 8 bits address length register to be read 
   * \param[out] p_value The 16 bits value of the specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint8_t p_register_id, uint16_t * p_value);
  /*!
   * \brief Read a 8-bits value from a register on the device
   * \param[in] p_register The 16 bits address length register to be read 
   * \param[out] p_value The 8 bits value of the specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint16_t p_register_id, uint8_t * p_value);
  /*!
   * \brief Read a 16-bits value from a register on the device
   * \param[in] p_register The 16 bits address length register to be read 
   * \param[out] p_value The 16 bits value of the specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint16_t p_register_id, uint16_t * p_value);
  /*!
   * \brief Write a 8-bits value to a register on the device
   * \param[in] p_register The 8 bits length register to be written
   * \param[in] p_value The 8 bits value of te specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t write_register(const uint8_t p_register_id, const uint8_t p_value);
  /*!
   * \brief Write a 8-bits value to a register on the device
   * \param[in] p_register The 8 bits length register to be written
   * \param[in] p_value The 8 bits value of the specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t write_register(const uint8_t p_register_id, const uint16_t p_value);
  /*!
   * \brief Write a 8-bits value to a register on the device
   * \param[in] p_register The 16 bits address length register to be written
   * \param[in] p_value The 8 bits value of te specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t write_register(const uint16_t p_register_id, const uint8_t p_value);
  /*!
   * \brief Write a 8-bits value to a register on the device
   * \param[in] p_register The 16 bits adddress length register to be written
   * \param[in] p_value The 8 bits value of the specified register 
   * \return 0 on success, -1 otherwise
   */
  virtual int32_t write_register(const uint16_t p_register_id, const uint16_t p_value);
}; // End of class i2c
