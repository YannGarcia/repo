/**
 * @file    pin_names.h
 * @brief   Main header file for the Hardware Abstract Layer pins mapping.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#if !defined(__TM4C129x_PIN_NAMES_H__)
#define __TM4C129x_PIN_NAMES_H__

/** @def Flag to define TM4C1294NCPDT board
 */
#define TM4C1294NCPDT_BOARD 1

#if defined(TM4C1294NCPDT_BOARD)

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"


/** @enum pin_names_t
 * @brief Describes the pin name/pin kernel number
 * @see SPMU365B Table 2-1. BoosterPack 1 GPIO and Signal Muxing
 */
typedef enum {
  p1 = GPIO_PORTD_BASE | GPIO_PIN_0,
  p2 = GPIO_PORTD_BASE | GPIO_PIN_1,
  p3 = GPIO_PORTD_BASE | GPIO_PIN_2,
  p4 = GPIO_PORTD_BASE | GPIO_PIN_3,
  p22 = GPIO_PORTC_BASE | GPIO_PIN_7,
  p23 = GPIO_PORTC_BASE | GPIO_PIN_6,
  p24 = GPIO_PORTC_BASE | GPIO_PIN_5,
  p25 = GPIO_PORTC_BASE | GPIO_PIN_4,
  p29 = GPIO_PORTH_BASE | GPIO_PIN_0,
  p30 = GPIO_PORTH_BASE | GPIO_PIN_1,
  p33 = GPIO_PORTA_BASE | GPIO_PIN_0,
  p42 = GPIO_PORTF_BASE | GPIO_PIN_0,   /* On board LED4 */
  p43 = GPIO_PORTF_BASE | GPIO_PIN_1,
  p44 = GPIO_PORTF_BASE | GPIO_PIN_2,
  p45 = GPIO_PORTF_BASE | GPIO_PIN_3,
  p46 = GPIO_PORTF_BASE | GPIO_PIN_4,   /* On board LED3 */
  p49 = GPIO_PORTG_BASE | GPIO_PIN_0,
  p50 = GPIO_PORTG_BASE | GPIO_PIN_1,
  p61 = GPIO_PORTK_BASE | GPIO_PIN_6,
  p62 = GPIO_PORTK_BASE | GPIO_PIN_5,
  p63 = GPIO_PORTK_BASE | GPIO_PIN_4,
  p81 = GPIO_PORTL_BASE | GPIO_PIN_0,
  p82 = GPIO_PORTL_BASE | GPIO_PIN_1,
  p83 = GPIO_PORTL_BASE | GPIO_PIN_2,
  p84 = GPIO_PORTL_BASE | GPIO_PIN_3,
  p107 = GPIO_PORTN_BASE | GPIO_PIN_0,  /* On board LED2 */
  p108 = GPIO_PORTN_BASE | GPIO_PIN_1,  /* On board LED1 */
  p116 = GPIO_PORTJ_BASE | GPIO_PIN_0,  /* On board PUSH1 */
  p117 = GPIO_PORTJ_BASE | GPIO_PIN_1,  /* On board PUSH2 */

  A0 = GPIO_PORTE_BASE | GPIO_PIN_3, /* MCU pin #12 */
  A1 = GPIO_PORTE_BASE | GPIO_PIN_2, /* MCU pin #13 */
  A2 = GPIO_PORTE_BASE | GPIO_PIN_1, /* MCU pin #14 */
  A3 = GPIO_PORTE_BASE | GPIO_PIN_0, /* MCU pin #15 */

  A9 = GPIO_PORTE_BASE | GPIO_PIN_4, /* MCU pin #123 */

  NC = 0xffffffff
} pin_names_t;

/**
 */
typedef pin_names_t pin_name;

#else /* TM4C1294NCPDT_BOARD */
#error "TM4C129x board flag was not defined"
#endif /* TM4C1294NCPDT_BOARD */

#endif /* __TM4C129x_PIN_NAMES_H__ */
