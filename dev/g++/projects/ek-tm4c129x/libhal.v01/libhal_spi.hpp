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

#include <mbed.h>

#include "digital_out.hpp"

/**
 * @class spi
 * @brief The SPI Master class used for communicating with SPI slave devices
 */
class spi
{
protected:
  std::unique_ptr<SPI> _spi;
  /** Chip select support */
  std::unique_ptr<digital_out> _cs;

public:
  /**
   * @brief Default constructor
   * SPI channel identifier set to 0
   * Chip select unmanaged
   */
  spi() : _spi(new SPI(p5, p6, p7)), _cs() { _spi->frequency(1000000); _spi->format(8, 3); };
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
   * @param[inout] p_value The 8 bits value to write and the response of the device
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t write_read(uint8_t * p_value);
  /**
   * @brief Write a 16-bits value and set the response
   * @param[inout] p_value The 16 bits value to write and the response of the device
   * @return 0 on success, -1 otherwise
   */
  virtual int32_t write_read(uint16_t * p_value);
private:
  inline void enable_device();
  inline void disable_device();
}; // End of class spi

void spi::enable_device()
{
  if (_cs.get() != NULL) { // Enable device
    _cs->write(digital_state_t::digital_state_low);
  }
}

void spi::disable_device()
{
  if (_cs.get() != NULL) { // Disable device
    _cs->write(digital_state_t::digital_state_high);
  }
}
