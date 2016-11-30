/**
 * @file    spi.hpp
 * @brief   Header file for SPI.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <cstdint>
#include <memory>

#include "digital_out.hpp"

/**
 * @class spi
 * @brief The SPI Master class used for communicating with SPI slave devices
 */
class spi {
protected:
  /** The SPI channel idetifier */
  uint8_t _spi_channel_id;
  /** The SPI file descriptor */
  int32_t _fd;
  /** Chip select support */
  std::unique_ptr<digital_out> _cs;

public:
  /**
   * @brief Default constructor
   * SPI channel identifier set to 0
   * Chip select unmanaged
   */
  spi();
  /**
   * @brief Constructor
   * Chip select unmanaged
   * @param [in] p_spi_channel_id The SPI channel identifier
   */
  spi(const uint8_t p_spi_channel_id);
  /**
   * @brief Constructor
   * @param [in] p_spi_channel_id The SPI channel identifier
   * @param [in] p_cs The chip select pin name
   */
  spi(const uint8_t p_spi_channel_id, const pin_name p_cs);
  /**
   * @brief Destructor
   */
  virtual ~spi();
  
  /**
   * @brief Write a 8-bits value and set the response
   * The device is enable during the data exchange operation
   * @param[inout] p_value The 8 bits value to write and the response of the device
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t data_read_write(uint8_t * p_value);
  /**
   * @brief Write a 16-bits value and set the response
   * This method is used to send two bytes command in the same operation
   * The device is enable during the data exchange operation
   * @param[inout] p_value The two bytes command
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t data_read_write(uint16_t * p_value);
  
private:
  /**
   * @brief Enable the SPI device is managed
   */
  inline void enable_device();
  /**
   * @brief Disable the SPI device is managed
   */
  inline void disable_device();
}; // End of class spi

void spi::enable_device() {
  if (_cs.get() != NULL) { // Enable device
    _cs->write(digital_state_t::digital_state_low);
  }
}

void spi::disable_device() {
  if (_cs.get() != NULL) { // Disable device
    _cs->write(digital_state_t::digital_state_high);
  }
}
