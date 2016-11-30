/**
 * @file    digital_out.hpp
 * @brief   Header file for digital out gpio state.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include "libhal.h"

/**
 * @class digital_out
 * @brief digital output gpio state
 * @see ::digital_write
 */
class digital_out {
protected:
  /** The gpio idenifier */
  pin_name _gpio;
  /** The gpio state */
  digital_state_t _state;

public:
  /**
   * @brief Constructor
   * State is set to low
   * @param[in] The gpio to connect
   */
  digital_out(const pin_name p_gpio_name) : _gpio(p_gpio_name), _state(digital_state_t::digital_state_low) { ::digital_write(_gpio, _state); };
  /**
   * @brief Constructor
   * @param[in] The gpio to connect
   * @param[in] p_state The gpio state
   */
  digital_out(const pin_name p_gpio_name, const digital_state_t p_state) : _gpio(p_gpio_name), _state(p_state) { ::digital_write(_gpio, _state); };
  /**
   * @brief Destructor
   * The gpio state is set to low before to destroy this class reference
   */
  virtual ~digital_out() { ::digital_write(_gpio, digital_state_t::digital_state_low); };

  /**
   * @brief Set the gpio state
   * @param[in] The new gpio state
   */
  inline void write(const digital_state_t p_state) { _state = p_state; ::digital_write(_gpio, _state); };
  /**
   * @brief Indicates the gpio state
   * @return The gpio state
   * @see digital_state_t
   */
  inline digital_state_t read() const { return _state; };

  inline digital_out & operator = (const digital_state_t p_state) { write(p_state); return *this; };
  inline digital_out & operator = (const digital_out & p_gpio) { write(p_gpio.read()); return *this; };
  inline operator const digital_state_t() const { return read(); };
}; // End of class digital_out
