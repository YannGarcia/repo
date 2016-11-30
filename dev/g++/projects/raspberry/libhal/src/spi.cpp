/**
 * @file    spi.cpp
 * @brief   Implementation file for SPI/SMBUS.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdexcept>

#include "libhal_spi.h"
#include "libhal_spi.hpp"

spi::spi() : _spi_channel_id(0x00), _fd(-1), _cs() {
  // Setup SPI support
  if ((_fd = ::libhal_spi_setup(_spi_channel_id, 1000000)) == -1) {
    throw std::runtime_error("spi::spi: ::libhal_spi_setup failure");
  }
} // End of constructor

spi::spi(const uint8_t p_spi_channel_id) : _spi_channel_id(p_spi_channel_id), _fd(-1), _cs() {
  // Setup SPI support
  if ((_fd = ::libhal_spi_setup(_spi_channel_id, 1000000)) == -1) {
    throw std::runtime_error("spi::spi: ::libhal_spi_setup failure");
  }
} // End of constructor

spi::spi(const uint8_t p_spi_channel_id, const pin_name p_cs) : _spi_channel_id(p_spi_channel_id), _fd(-1), _cs() {
  // Setup SPI support
  if ((_fd = ::libhal_spi_setup(_spi_channel_id, 1000000)) == -1) {
    throw std::runtime_error("spi::spi: ::libhal_spi_setup failure");
  }
  if (p_cs != NC) {
    _cs.reset(new digital_out(p_cs, digital_state_t::digital_state_high)); // Disable the SPI device
  }
} // End of constructor

spi::~spi() {
  if (_fd != -1) {
    ::libhal_spi_close(_fd);
    _fd = -1;
  }
  if (_cs.get() != NULL) {
    _cs.reset();
  }
} // End of destructor

int32_t spi::data_read_write(uint8_t * p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  enable_device();
  int32_t result = ::libhal_spi_data_read_write(_spi_channel_id, p_value, sizeof(uint8_t));
  disable_device();

  return result;
}

int32_t spi::data_read_write(uint16_t * p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  enable_device();
  *p_value = ((*p_value << 8) & 0xff00) | ((*p_value >> 8) & 0x00ff);
  int32_t result = ::libhal_spi_data_read_write(_spi_channel_id, reinterpret_cast<const uint8_t *>(p_value), sizeof(uint16_t));
  disable_device();

  return result;
}
