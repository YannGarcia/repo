/**
 * @file    libhal_shift.c
 * @brief   Main implementation file for the Hardware Abstract Layer library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include "libhal_shift.h"

uint8_t libhal_shift_in(const pin_name p_data_pin, const pin_name p_clock_pin, const shift_modes_t p_order) {
  uint8_t value = 0 ;
  int8_t i;
 
  if (p_order == shift_modes_msb_first) {
    for (i = 7 ; i >= 0 ; --i) {
      digital_write (p_clock_pin, digital_state_high);
      value |= digital_read (p_data_pin) << i ;
      digital_write (p_clock_pin, digital_state_low);
    } // End of 'for' statement
  } else {
    for (i = 0 ; i < 8 ; ++i) {
      digital_write (p_clock_pin, digital_state_high);
      value |= digital_read (p_data_pin) << i ;
      digital_write (p_clock_pin, digital_state_low);
    } // End of 'for' statement
  }

  return value;
} // End of function libhal_shift_in

void libhal_shift_out(const pin_name p_data_pin, const pin_name p_clock_pin, const shift_modes_t p_order, const uint8_t p_register) {
  int8_t i;

  if (p_order == shift_modes_msb_first) {
    for (i = 7 ; i >= 0 ; --i) {
      digital_write(p_data_pin, p_register & (1 << i));
      digital_write(p_clock_pin, digital_state_high);
      digital_write(p_clock_pin, digital_state_low);
    } // End of 'for' statement
  } else {
    for (i = 0 ; i < 8 ; ++i) {
      digital_write(p_data_pin, p_register & (1 << i));
      digital_write(p_clock_pin, digital_state_high);
      digital_write(p_clock_pin, digital_state_low);
    } // End of 'for' statement
  }
  
  // Reset GPIOs state
  digital_write(p_data_pin, digital_state_low);
} // End of function libhal_shift_out
