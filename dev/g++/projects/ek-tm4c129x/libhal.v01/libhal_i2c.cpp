/**
 * @file    i2c.cpp
 * @brief   Implementation file for I2C/SMBUS.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdexcept>

#include "libhal_i2c.hpp"

i2c::i2c(const uint8_t p_device_address) : _i2c(), _device_address(p_device_address)
{
  // Default I2C module: p9/p10
  _i2c.reset(new I2C(p9, p10));
} // End of constructor

i2c::i2c(const uint8_t p_bus_id, const uint8_t p_device_address) : _i2c(), _device_address(p_device_address)
{
  if (p_bus_id == 0) { // Default I2C module 0: p9/p10
    _i2c.reset(new I2C(p9, p10));
  } else { // I2C module 1: p28/p27
    _i2c.reset(new I2C(p28, p27));
  }
} // End of constructor

i2c::~i2c()
{
  if (_i2c.get() != NULL) {
    _i2c.reset();
  }
} // End of destructor

int32_t i2c::read_register(const uint8_t p_register_id, uint8_t * p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  uint8_t buffer[1];
  buffer[0] = p_register_id;
  // 2. Send I2C start + memory address
  if (_i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 1, true) == 0) {
    // 2. Read data + I2C stop
    int32_t result = _i2c->read(_device_address, reinterpret_cast<char *>(&buffer[0]), 1);
    *p_value = static_cast<uint8_t>(buffer[0]);

    return result;
  }

  return -1;
}

int32_t i2c::read_register(const uint8_t p_register_id, uint16_t * p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  int8_t buffer[2];
  buffer[0] = p_register_id;
  // 2. Send I2C start + memory address
  if (_i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 1, true) == 0) {
    // 2. Read data + I2C stop
    int32_t result = _i2c->read(_device_address, reinterpret_cast<char *>(&buffer[0]), 2);
    *p_value = static_cast<uint16_t>((buffer[0] << 8) | buffer[1]);

    return result;
  }

  return -1;
}

int32_t i2c::read_register(const uint16_t p_register_id, uint8_t * p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  int8_t buffer[2];
  buffer[0] = static_cast<uint8_t>(p_register_id >> 8);
  buffer[1] = static_cast<uint8_t>(p_register_id & 0xff);
  // 2. Send I2C start + memory address
  if (_i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 1, true) == 0) {
    // 2. Read data + I2C stop
    int32_t result = _i2c->read(_device_address, reinterpret_cast<char *>(&buffer[0]), 1);
    *p_value = static_cast<uint8_t>(buffer[0]);

    return result;
  }

  return -1;
}

int32_t i2c::read_register(const uint16_t p_register_id, uint16_t * p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  int8_t buffer[2];
  buffer[0] = static_cast<uint8_t>(p_register_id >> 8);
  buffer[1] = static_cast<uint8_t>(p_register_id & 0xff);
  // 2. Send I2C start + memory address
  if (_i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 2, true) == 0) {
    // 2. Read data + I2C stop
    int32_t result = _i2c->read(_device_address, reinterpret_cast<char *>(&buffer[0]), 2);
    *p_value = static_cast<uint16_t>((buffer[0] << 8) | buffer[1]);

    return result;
  }

  return -1;
}

int32_t i2c::write_register(const uint8_t p_register_id, const uint8_t p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  uint8_t buffer[2]; // Memory address + one byte of data
  // 1.1. Memory address
  buffer[0] = static_cast<uint8_t>(p_register_id);
  // 1.2. Datas
  buffer[1] = p_value;

  // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
  int32_t result = _i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 2);

  return result;
}

int32_t i2c::write_register(const uint8_t p_register_id, const uint16_t p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  uint8_t buffer[3]; // Memory address + two bytes of data
  // 1.1. Memory address
  buffer[0] = static_cast<uint8_t>(p_register_id);
  // 1.2. Datas
  buffer[1] = static_cast<uint8_t>(p_value >> 8);
  buffer[2] = static_cast<uint8_t>(p_value & 0xff);

  // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
  int32_t result = _i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 3);

  return result;
}

int32_t i2c::write_register(const uint16_t p_register_id, const uint8_t p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  uint8_t buffer[3]; // Memory address + one byte of data
  // 1.1. Memory address
  buffer[0] = static_cast<uint8_t>(p_register_id >> 8);
  buffer[1] = static_cast<uint8_t>(p_register_id & 0xff);
  // 1.2. Datas
  buffer[2] = p_value;

  // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
  int32_t result = _i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 3);

  return result;
}

int32_t i2c::write_register(const uint16_t p_register_id, const uint16_t p_value)
{
  // Sanity check
  if (_i2c.get() == NULL) {
    return -1;
  }

  // 1.Prepare buffer
  uint8_t buffer[4]; // Memory address + two bytes of data
  // 1.1. Memory address
  buffer[0] = static_cast<uint8_t>(p_register_id >> 8);
  buffer[1] = static_cast<uint8_t>(p_register_id & 0xff);
  // 1.2. Datas
  buffer[2] = static_cast<uint8_t>(p_value >> 8);
  buffer[3] = static_cast<uint8_t>(p_value & 0xff);

  // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
  int32_t result = _i2c->write(_device_address, reinterpret_cast<const char *>(&buffer[0]), 4);

  return result;
}
