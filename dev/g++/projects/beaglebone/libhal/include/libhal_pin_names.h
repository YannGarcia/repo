/**
 * @file    pin_names.h
 * @brief   Main header file for the Hardware Abstract Layer pins mapping.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#if !defined(__BEAGLEBONEBLACK_PIN_NAMES_H__)
#define __BEAGLEBONEBLACK_PIN_NAMES_H__

/** @def Flagto define BeagleBoneBlack board
 */
#define BEAGLEBONE_BLACK_BOARD 1

#if defined(BEAGLEBONE_BLACK_BOARD)

/** @enum pin_names_t
 * @brief Describes the pin name/pin kernel number 
 */
typedef enum {
  P9_22 = 2,
  P9_21 = 3,
  P9_42 = 7,
  P8_19 = 22,
  P8_13 = 23,
  P8_05 = 34,
  P8_06 = 35,
  P8_03 = 38,
  P8_04 = 39,
  P9_23 = 49,
  P9_14 = 50,
  P9_16 = 51,
  P8_07 = 66,
  P8_08 = 67,
  P8_10 = 68,
  P8_09 = 69,
  P9_28 = 113,
  P9_27 = 115,
  P9_25 = 117,
  P9_33 = 243,
  P9_35 = 245,
  P9_36 = 246,
  P9_37 = 247,
  P9_38 = 248,
  P9_39 = 249,
  P9_40 = 250,
  
  p2 = P9_22,
  p3 = P9_21,
  p7 = P9_42,
  p22 = P8_19,
  p23 = P8_13,
  p34 = P8_05,
  p38 = P8_03,
  p39 = P8_04,
  p49 = P9_23,
  p50 = P9_14,
  p51 = P9_16,
  p66 = P8_07,
  p67 = P8_08,
  p68 = P8_10,
  p69 = P8_09,
  p113 = P9_28,
  p115 = P9_27,
  p117 = P9_25,

  A0 = P9_39,

  NC = 0xffffffff
} pin_names_t;

/**
 */
typedef pin_names_t pin_name;

#else // BEAGLEBONE_BLACK_BOARD
#error "BeagleBoneBlack board flag was not defined"
#endif // BEAGLEBONE_BLACK_BOARD

#endif // __BEAGLEBONEBLACK_PIN_NAMES_H__
