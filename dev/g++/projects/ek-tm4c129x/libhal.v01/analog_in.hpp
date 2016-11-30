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
  std::unique_ptr<AnalogIn> _adc;

public:
  /**
   * @brief Constructor
   * @param[in] p_adc The analog input pin identifier
   */
  analog_in(const pin_name p_adc) : _adc(new AnalogIn(p_adc)) { };
  /**
   * @brief Destructor
   */
  ~analog_in() { _adc.reset(); };

  /**
   * @brief Read the input voltage, represented as a float in the range
   * @return The voltage value
   */
  inline const float read() const { return _adc->read(); };
  /**
   * @brief Read the input voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
   * @return The voltage value
   */
  inline const uint16_t read_u16() const { return static_cast<uint16_t>(_adc->read_u16()); };
  /**
   * @brief Read the input voltage, represented as a float in the range
   * @return The voltage value
   */
  inline operator const float () const { return _adc->read(); };
}; // End of class analog_in
