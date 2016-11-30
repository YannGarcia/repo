/**
 * @file    libhal_shift.h
 * @brief   Main header file for the Hardware Abstract Layer library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#if !defined(__BEAGLEBONEBLACK_SHIFT_H__)
#define __BEAGLEBONEBLACK_SHIFT_H__

#include "libhal.h"

/** @enum shifting modes
 */
typedef enum {
  shift_modes_lsb_first, /*!< Shift bit0 first */
  shift_modes_msb_first  /*!< Shift bit7 first */
} shift_modes_t;

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @fn uint8_t shift_in(const pin_name p_data_pin, const pin_name p_clock_pin, const shift_modes_t order)
   * @brief Shift data in from a clocked source
   * @param[in] p_data_pin  The data GPIO identifier
   * @param[in] p_clock_pin The clock GPIO identifier
   * @param[in] p_order     The bit order for shifting
   * @return            The shifted value
   */
  extern uint8_t libhal_shift_in(const pin_name p_data_pin, const pin_name p_clock_pin, const shift_modes_t p_order);
  /**
   * @fn uint8_t shift_in(const pin_name p_data_pin, const pin_name p_clock_pin, const shift_modes_t order)
   * @brief Shift data out to a clocked source
   * @param[in] p_data_pin  The data GPIO identifier
   * @param[in] p_clock_pin The clock GPIO identifier
   * @param[in] p_order     The bit order for shifting
   * @param[in] p_register  The value to shift
   */
  extern void libhal_shift_out(const pin_name p_data_pin, const pin_name p_clock_pin, const shift_modes_t p_order, const uint8_t p_register);

#ifdef __cplusplus
}
#endif

#endif // __BEAGLEBONEBLACK_SHIFT_H__
