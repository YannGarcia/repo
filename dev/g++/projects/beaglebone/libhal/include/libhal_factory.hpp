/**
 * @file    libhal_factory.hpp
 * @brief   Header file for for lihal object factory.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include "libhal.h"

#include "libhal_spi.hpp"
#include "libhal_i2c.hpp"
#include "digital_out.hpp"
#include "analog_in.hpp"

/**
 * @class libhal_factory
 * @brief lihal object factory
 * @remark Refer to signeton pattern
 */
class libhal_factory {
  /** Unique instance of this class
   */
  static std::unique_ptr<libhal_factory> g_instance;

public:
  /**
   * @brief Destructor
   */
  virtual ~libhal_factory() { ::libhal_shutdown_sys(); };
  
  /**
   * @brief Create a new instance of a digital_out class
   * @param[in] p_gpio_name The gpio to connect
   * @param[in] p_state The gpio state. Default: digital_state_t::digital_state_low
   * @return The new instance of a digital_out class
   */
  inline std::unique_ptr<digital_out> create_digital_out_instance(const pin_name p_gpio_name, const digital_state_t p_state = digital_state_t::digital_state_low) const;
  /**
   * @brief Create a new instance of an analog_in class
   * @param[in] p_adc The analog input pin identifier
   * @return The new instance of a analoh_in class
   */
  inline std::unique_ptr<analog_in> create_analog_in_instance(const pin_name p_adc) const;
  /**
   * @brief Create a new instance of an i2c class
   * @param[in] p_bus_id The I2C bus identifier (/dev/i2c-<p_bus_id>)
   * @param[in] p_device_address The I2C device address on 7 bits (e.g. 1 0 1 0 A3 A2 A1 R/W for 24LCxx)
   * @return The new instance of a i2c class
   */
  inline std::unique_ptr<i2c> create_i2c_instance(const uint8_t p_bus_id, const uint8_t p_device_address) const;
  /**
   * @brief Create a new instance of an spi class
   * @param [in] p_spi_channel_id The SPI channel identifier
   * @param [in] p_cs The chip select pin name. Default: NC
   * @return The new instance of a spi class
   */
  inline std::unique_ptr<spi> create_spi_instance(const uint8_t p_spi_channel_id, const pin_name p_cs = pin_name::NC) const;
  
  /**
   * @brief Get unique reference of this class
   * return The unique reference of this class
   */
  static inline const libhal_factory & get_instance() { return *g_instance; };

private:
  /**
   * @brief Private constructor. 
   * Refer to singleton design pattern
   */
  libhal_factory() { ::libhal_setup_sys(); };
  
}; // End of class libhal_factory

std::unique_ptr<libhal_factory> libhal_factory::g_instance(new libhal_factory());

std::unique_ptr<digital_out> libhal_factory::create_digital_out_instance(const pin_name p_gpio_name, const digital_state_t p_state) const {
  std::unique_ptr<digital_out> do_(new digital_out(p_gpio_name, p_state));
  return do_;
};

std::unique_ptr<analog_in> libhal_factory::create_analog_in_instance(const pin_name p_adc) const {
  std::unique_ptr<analog_in> ai_(new analog_in(p_adc));
  return ai_;
};

std::unique_ptr<i2c> libhal_factory::create_i2c_instance(const uint8_t p_bus_id, const uint8_t p_device_address) const {
  std::unique_ptr<i2c> i2c_(new i2c(p_bus_id, p_device_address));
  return i2c_;
};

std::unique_ptr<spi> libhal_factory::create_spi_instance(const uint8_t p_spi_channel_id, const pin_name p_cs) const {
  std::unique_ptr<spi> spi_(new spi(p_spi_channel_id, p_cs));
  return spi_;
};
