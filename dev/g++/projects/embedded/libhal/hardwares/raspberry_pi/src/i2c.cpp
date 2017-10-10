/**
 * @file    i2c.cpp
 * @brief   Implementation file for I2C/SMBUS.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdexcept>

#include "libhal_i2c.h"
#include "libhal_i2c.hpp"

i2c::i2c(const uint8_t p_device_address) : _bus_id(0x00), _fd(-1), _device_address(p_device_address) {
  // Setup I2C support
  if ((_fd = ::libhal_i2c_setup(_bus_id, _device_address)) == -1) {
    throw std::runtime_error("i2c::i2c: ::libhal_i2c_setup failure");
  }
} // End of constructor

i2c::i2c(const uint8_t p_bus_id, const uint8_t p_device_address) : _bus_id(p_bus_id), _fd(-1), _device_address(p_device_address) {
  // Setup I2C support
  if ((_fd = ::libhal_i2c_setup(_bus_id, _device_address)) == -1) {
    throw std::runtime_error("i2c::i2c: ::libhal_i2c_setup failure");
  }
} // End of constructor

i2c::~i2c() {
  if (_fd != -1) {
    ::libhal_i2c_close(_fd);
    _fd = -1;
  }
} // End of dtor

int32_t i2c::read_register(const uint8_t p_register_id, uint8_t * p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  int32_t result = ::libhal_i2c_read_register8(_fd, p_register_id);
  if (result == -1) {
    return -1;
  }
  *p_value = static_cast<uint8_t>(result);
  return 0;
}

int32_t i2c::read_register(const uint8_t p_register_id, uint16_t * p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  int32_t result = ::libhal_i2c_read_register16(_fd, p_register_id);
  if (result == -1) {
    return -1;
  }
  *p_value = static_cast<uint16_t>(result);
  return 0;
}

int32_t i2c::read_register(const uint16_t p_register_id, uint8_t * p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  int32_t result = ::libhal_i2c_ext_read_register8(_fd, _device_address, p_register_id);
  if (result == -1) {
    return -1;
  }
  *p_value = static_cast<uint8_t>(result);
  return 0;
}

int32_t i2c::read_register(const uint16_t p_register_id, uint16_t * p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  int32_t result = ::libhal_i2c_ext_read_register8(_fd, _device_address, p_register_id);
  if (result == -1) {
    return -1;
  }
  *p_value = static_cast<uint16_t>(result);
  return 0;
}

int32_t i2c::write_register(const uint8_t p_register_id, const uint8_t p_value) {
  return -1;
}

int32_t i2c::write_register(const uint8_t p_register_id, const uint16_t p_value) {
  return -1;
}

int32_t i2c::write_register(const uint16_t p_register_id, const uint8_t p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  if (::libhal_i2c_ext_write_register8(_fd, _device_address, p_register_id, p_value) == -1) {
    return -1;
  }
  
  return 0;
}

int32_t i2c::write_register(const uint16_t p_register_id, const uint16_t p_value) {
  // Sanity check
  if (_fd == -1) {
    return -1;
  }
  
  if (::libhal_i2c_ext_write_register16(_fd, _device_address, p_register_id, p_value) == -1) {
    return -1;
  }
  
  return 0;
}
