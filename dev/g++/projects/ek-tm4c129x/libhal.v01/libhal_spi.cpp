/**
 * @file    spi.cpp
 * @brief   Implementation file for SPI/SMBUS.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdexcept>

#include "libhal_spi.hpp"

spi::spi(const uint8_t p_spi_channel_id) : _spi(), _cs() {
  if (p_spi_channel_id == 0) {
    _spi.reset(new SPI(p5, p6, p7));
  } else {
    _spi.reset(new SPI(p11, p12, p13));
  }
  _spi->frequency(1000000); // Set the frequency of the SPI interface
  _spi->format(8, 3);
} // End of constructor

spi::spi(const uint8_t p_spi_channel_id, const pin_name p_cs) : _spi(), _cs() {
  if (p_spi_channel_id == 0) {
    _spi.reset(new SPI(p5, p6, p7));
  } else {
    _spi.reset(new SPI(p11, p12, p13));
  }
  _spi->frequency(1000000); // Set the frequency of the SPI interface
  _spi->format(8, 3);
    
  if (p_cs != NC) {
    _cs.reset(new digital_out(p_cs, digital_state_t::digital_state_high)); // Disable the SPI device
  }
} // End of constructor

spi::~spi() {
  if (_spi.get() != NULL) {
    _spi.reset();
  }
  if (_cs.get() != NULL) {
    _cs.reset();
  }
} // End of destructor    

int32_t spi::write_read(uint8_t * p_value) {
  // Sanity check                                                                                                                                     
  if (_spi.get() != NULL) {
    return -1;
  }

  enable_device();
  *p_value = _spi->write(*p_value);
  disable_device();

  return 0;
}

int32_t spi::write_read(uint16_t * p_value) {
  // Sanity check                                                                                                                                     
  if (_spi.get() != NULL) {
    return -1;
  }
  
  enable_device();
  *p_value = static_cast<uint16_t>(_spi->write(*p_value));
  disable_device();

  return 0;
}

