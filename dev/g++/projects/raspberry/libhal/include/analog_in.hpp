/**
 * @file    analog_in.hpp
 * @brief   Header file for digital to numeric convertion.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include "libhal.h"

/**
 * @class analog_in
 * @brief digital output gpio state
 * @see ::digital_write
 */
class analog_in {
protected:
  pin_name _adc;

public:
  /**
   * @brief Constructor
   * @param[in] p_adc The analog input pin identifier
   */
  analog_in(const pin_name p_adc) : _adc(p_adc) { };
  /**
   * @brief Destructor
   */
  ~analog_in() { };

  /**
   * @brief Read the input voltage, represented as a float in the range
   * @return The voltage value
   */
  inline const float read() const { return ::analog_read(_adc); };
  /**
   * @brief Read the input voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
   * @return The voltage value
   */
  inline const uint16_t read_u16() const { return -1; }; // TODO
  /**
   * @brief Read the input voltage, represented as a float in the range
   * @return The voltage value
   */
  inline operator const float () const { return ::analog_read(_adc); };
}; // End of class analog_in
