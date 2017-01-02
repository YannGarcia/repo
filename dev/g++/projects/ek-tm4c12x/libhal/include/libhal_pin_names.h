/**
 * @file      libhal_pin_names.h
 * @brief     Main header file for the Hardware Abstract Layer pins mapping.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 * @see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * @see       EK-TM4C129EXL Firmware Development Package User's Guide
 * @see       EK-TM4C1294XL Firmware Development Package User's Guide
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(TM4C1294NCPDT_BOARD) || defined(TM4C129ENCPDT_BOARD)

/** @enum pin_names_t
 * @brief Describes the pin name/pin kernel number
 * @see Tiva™ TM4C129DNCPDT Microcontroller Table 10-2. GPIO Pins and Alternate Functions
 * @see SPMU365C Table 2-1. BoosterPack 1 GPIO and Signal Muxing, MCI Pin column
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
  p116 = GPIO_PORTJ_BASE | GPIO_PIN_0,  /* On board Switch1 */
  p117 = GPIO_PORTJ_BASE | GPIO_PIN_1,  /* On board Switch2 */

  A0 = GPIO_PORTE_BASE | GPIO_PIN_3, /* MCU pin #12 */
  A1 = GPIO_PORTE_BASE | GPIO_PIN_2, /* MCU pin #13 */
  A2 = GPIO_PORTE_BASE | GPIO_PIN_1, /* MCU pin #14 */
  A3 = GPIO_PORTE_BASE | GPIO_PIN_0, /* MCU pin #15 */

  A9 = GPIO_PORTE_BASE | GPIO_PIN_4, /* MCU pin #123 */

  SDA0 = GPIO_PORTB_BASE | GPIO_PIN_3,
  SCL0 = GPIO_PORTB_BASE | GPIO_PIN_2,

  NC = 0xffffffff
} pin_names_t;

#elif defined(TM4C123_BOARD)

/** @enum pin_names_t
 * @brief Describes the pin name/pin kernel number
 * @see Tiva™ TM4C1233H6PM Microcontroller Table 10-2. GPIO Pins and Alternate Functions
 * @see SPMU296–April 2013 Table 2-1. BoosterPack 1 GPIO and Signal Muxing, MCI Pin column
 */
typedef enum {
  p1 = GPIO_PORTB_BASE | GPIO_PIN_6,
  p4 = GPIO_PORTB_BASE | GPIO_PIN_7,
  p5 = GPIO_PORTF_BASE | GPIO_PIN_4, /* On board Swith 1 */

  p28 = GPIO_PORTF_BASE | GPIO_PIN_0, /* On board Swith 2 */
  p29 = GPIO_PORTF_BASE | GPIO_PIN_1, /* On board RGB led Red */
  p30 = GPIO_PORTF_BASE | GPIO_PIN_2, /* On board RGB led Blue */
  p31 = GPIO_PORTF_BASE | GPIO_PIN_3, /* On board RGB led Green */
  
  NC = 0xffffffff
} pin_names_t;

#else /* TM4C1294NCPDT_BOARD, TM4C123_BOARD */

#error "TM4C129x board flag was not defined"

#endif /* TM4C1294NCPDT_BOARD, TM4C123_BOARD */

/**
 */
typedef pin_names_t pin_name;

#ifdef __cplusplus
}
#endif

