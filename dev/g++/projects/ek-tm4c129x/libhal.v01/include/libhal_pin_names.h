/**
 * @file    libhal_pin_names.h
 * @brief   Main header file for the Hardware Abstract Layer pins mapping.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#if !defined(__TM4C129_PIN_NAMES_H__)
#define __TM4C129_PIN_NAMES_H__

/** @def Flag to define TM4C129 board
 */
#define TM4C129_BOARD 1

#if defined(TM4C129_BOARD)

#include "driverlib/gpio.h"

/** @enum pin_names_t
 * @brief Describes the pin name/pin kernel number 
 */
typedef enum {
  p0 = GPIO_PIN_0,
  p1 = GPIO_PIN_1,
  p2 = GPIO_PIN_2,
  p3 = GPIO_PIN_3,
  p4 = GPIO_PIN_4,
  p5 = GPIO_PIN_5,
  p6 = GPIO_PIN_6,
  p7 = GPIO_PIN_7,
  
  A0 = P9_39,

  NC = 0xffffffff
} pin_names_t;

/**
 */
typedef pin_names_t pin_name;

#else // TM4C129_BOARD
#error "TM4C129 board flag was not defined"
#endif // TM4C129_BOARD

#endif // __TM4C129_PIN_NAMES_H__

