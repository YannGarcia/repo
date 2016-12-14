/**
 * @file    i2c.hpp
 * @brief   Header file for I2C/SMBUS.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <cstdint>
#include <memory>

#include <mbed.h>

class i2c
{
protected:
  /** The MBED I2C instance */
  std::unique_ptr<I2C> _i2c;
  /** The I2C device address */
  uint8_t _device_address;
    
public:
  /**
   * @brief Constructor
   * I2C bus identifier is 0
   * @param[in] p_device_address The I2C device address on 7 bits (e.g. 1 0 1 0 A3 A2 A1 R/W for 24LCxx)
   */
  i2c(const uint8_t p_device_address);
  /**
   * @brief Constructor
   * @param[in] p_bus_id The I2C bus identifier (/dev/i2c-<p_bus_id>)
   * @param[in] p_device_address The I2C device address on 7 bits (e.g. 1 0 1 0 A3 A2 A1 R/W for 24LCxx)
   */
  i2c(const uint8_t p_bus_id, const uint8_t p_device_address);
  /**
   * @brief Destructor
   */
  virtual ~i2c();

  /**
   * @brief Read a 8-bits value from a register on the device
   * @param[in] p_register The 8 bits length register to be read
   * @param[out] p_value The 8 bits value of the specified register
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint8_t p_register_id, uint8_t * p_value);
  /**
   * @brief Read a 16-bits value from a register on the device
   * @param[in] p_register The 16 bits length register to be read
   * @param[out] p_value The 18 bits value of the specified register
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint8_t p_register_id, uint16_t * p_value);
  /**
   * @brief Read a 8-bits value from a register on the device
   * @param[in] p_register The 8 bits length register to be read
   * @param[out] p_value The 8 bits value of the specified register
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint16_t p_register_id, uint8_t * p_value);
  /**
   * @brief Read a 16-bits value from a register on the device
   * @param[in] p_register The 16 bits length register to be read
   * @param[out] p_value The 18 bits value of the specified register
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t read_register(const uint16_t p_register_id, uint16_t * p_value);
    
  virtual int32_t write_register(const uint8_t p_register_id, const uint8_t p_value);
  virtual int32_t write_register(const uint8_t p_register_id, const uint16_t p_value);
  virtual int32_t write_register(const uint16_t p_register_id, const uint8_t p_value);
  virtual int32_t write_register(const uint16_t p_register_id, const uint16_t p_value);
}; // End of class I2C
