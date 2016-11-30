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
 * @see https://projects.drogon.net/raspberry-pi/wiringpi/pins/
 */
typedef enum {
  P1_3 = 8,
  P1_5 = 9,
  P1_7 = 7,
  P1_8 = 15,
  P1_10 = 16,
  P1_11 = 0,
  P1_12 = 1,
  P1_13 = 2,
  P1_15 = 3,
  P1_16 = 4,
  P1_18 = 5,
  P1_19 = 12,
  P1_21 = 13,
  P1_22 = 6,
  P1_23 = 14,
  P1_24 = 10,
  P1_26 = 11,
  P5_3 = 17,
  P5_4 = 18,
  P5_5 = 19,
  P5_6 = 20,

  p3 = P1_3,
  p5 = P1_5,
  p7 = P1_7,
  p8 = P1_8,
  p10 = P1_10,
  p11 = P1_11,
  p12 = P1_12,
  p13 = P1_13,
  p15 = P1_15,
  p16 = P1_16,
  p18 = P1_18,
  p19 = P1_19,
  p21 = P1_21,
  p22 = P1_22,
  p23 = P1_23,
  p24 = P1_24,
  p27 = P5_3,
  p28 = P5_4,
  p29 = P5_5,
  p30 = P5_6,

  NC = 0xffffffff
} pin_names_t;

/**
 */
typedef pin_names_t pin_name;

#else // BEAGLEBONE_BLACK_BOARD
#error "BeagleBoneBlack board flag was not defined"
#endif // BEAGLEBONE_BLACK_BOARD

#endif // __BEAGLEBONEBLACK_PIN_NAMES_H__
