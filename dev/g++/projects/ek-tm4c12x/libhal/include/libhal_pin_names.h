/**
 * @file      libhal_pin_names.h
 * @brief     Header file for the Hardware Abstract Layer pins mapping.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 * @see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * @see       EK-TM4C129EXL Firmware Development Package User's Guide
 * @see       EK-TM4C1294XL Firmware Development Package User's Guide
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(TM4C1294NCPDT_BOARD) || defined(TM4C129ENCPDT_BOARD)

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

/** @enum pin_names_t
 * @brief Describes the pin name/pin kernel number
 *        Analog conversions only done on PORTB and PORTE pins (ADC0 module)
 *        PWM and analog comparators use ADC1 module
 * @see Tiva™ TM4C129DNCPDT Microcontroller Table 10-2. GPIO Pins and Alternate Functions
 * @see SPMU365C Table 2-1. BoosterPack 1 GPIO and Signal Muxing, MCI Pin column
 */
typedef enum {
  p1 = GPIO_PORTD_BASE | GPIO_PIN_0,    /* SSI2: SPI MISO */
  p2 = GPIO_PORTD_BASE | GPIO_PIN_1,    /* SSI2: SPI MOSI */
  p3 = GPIO_PORTD_BASE | GPIO_PIN_2,
  p4 = GPIO_PORTD_BASE | GPIO_PIN_3,    /* SSI2: SPI CLK */
  p22 = GPIO_PORTC_BASE | GPIO_PIN_7,
  p23 = GPIO_PORTC_BASE | GPIO_PIN_6,
  p24 = GPIO_PORTC_BASE | GPIO_PIN_5,
  p25 = GPIO_PORTC_BASE | GPIO_PIN_4,
  p29 = GPIO_PORTH_BASE | GPIO_PIN_0,
  p30 = GPIO_PORTH_BASE | GPIO_PIN_1,
  p31 = GPIO_PORTH_BASE | GPIO_PIN_2,
  p32 = GPIO_PORTH_BASE | GPIO_PIN_3,
  p33 = GPIO_PORTA_BASE | GPIO_PIN_0,
  p34 = GPIO_PORTA_BASE | GPIO_PIN_1,
  p35 = GPIO_PORTA_BASE | GPIO_PIN_2,
  p36 = GPIO_PORTA_BASE | GPIO_PIN_3,
  p37 = GPIO_PORTA_BASE | GPIO_PIN_4,
  p38 = GPIO_PORTA_BASE | GPIO_PIN_5,
  p40 = GPIO_PORTA_BASE | GPIO_PIN_6,
  p41 = GPIO_PORTA_BASE | GPIO_PIN_7,
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
  p75 = GPIO_PORTM_BASE | GPIO_PIN_3,   /* PWM */
  p81 = GPIO_PORTL_BASE | GPIO_PIN_0,
  p82 = GPIO_PORTL_BASE | GPIO_PIN_1,
  p83 = GPIO_PORTL_BASE | GPIO_PIN_2,
  p84 = GPIO_PORTL_BASE | GPIO_PIN_3,
  p91 = GPIO_PORTB_BASE | GPIO_PIN_2,   /* I2C SDL */
  p92 = GPIO_PORTB_BASE | GPIO_PIN_3,   /* I2C SDA */
  p107 = GPIO_PORTN_BASE | GPIO_PIN_0,  /* On board LED2 */
  p108 = GPIO_PORTN_BASE | GPIO_PIN_1,  /* On board LED1 */
  p109 = GPIO_PORTN_BASE | GPIO_PIN_2,
  p116 = GPIO_PORTJ_BASE | GPIO_PIN_0,  /* On board Switch1 */
  p117 = GPIO_PORTJ_BASE | GPIO_PIN_1,  /* On board Switch2 */

  A0 = GPIO_PORTE_BASE | GPIO_PIN_3, /* MCU pin #12 */
  A1 = GPIO_PORTE_BASE | GPIO_PIN_2, /* MCU pin #13 */
  A2 = GPIO_PORTE_BASE | GPIO_PIN_1, /* MCU pin #14 */
  A3 = GPIO_PORTE_BASE | GPIO_PIN_0, /* MCU pin #15 */

  A8 = GPIO_PORTE_BASE | GPIO_PIN_5, /* MCU pin #124 */
  A9 = GPIO_PORTE_BASE | GPIO_PIN_4, /* MCU pin #123 */

  SDA0 = GPIO_PORTB_BASE | GPIO_PIN_3,
  SCL0 = GPIO_PORTB_BASE | GPIO_PIN_2,

  NC = 0xffffffff
} pin_names_t;

#elif defined(TM4C123_BOARD)

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

/** @enum pin_names_t
 * @brief Describes the pin name/pin kernel number
 * @see Tiva™ TM4C1233H6PM Microcontroller Table 10-2. GPIO Pins and Alternate Functions
 * @see SPMU296–April 2013 Table 2-1. BoosterPack 1 GPIO and Signal Muxing, MCI Pin column
 */
typedef enum {
  p1 = GPIO_PORTB_BASE | GPIO_PIN_6,
  p4 = GPIO_PORTB_BASE | GPIO_PIN_7,
  p5 = GPIO_PORTF_BASE | GPIO_PIN_4, /* On board Switch 1 */

  p28 = GPIO_PORTF_BASE | GPIO_PIN_0, /* On board Switch 2 */
  p29 = GPIO_PORTF_BASE | GPIO_PIN_1, /* On board RGB led Red */
  p30 = GPIO_PORTF_BASE | GPIO_PIN_2, /* On board RGB led Blue */
  p31 = GPIO_PORTF_BASE | GPIO_PIN_3, /* On board RGB led Green */
  
  NC = 0xffffffff
} pin_names_t;

#elif defined(MSP432P4XX_BOARD)

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

#else /* TM4C1294NCPDT_BOARD, TM4C123_BOARD, MSP432P4XX_BOARD */

#error "No board flag was not defined (e.g. TM4C1294NCPDT_BOARD, TM4C123_BOARD or MSP432P4XX_BOARD)"

#endif /* TM4C1294NCPDT_BOARD, TM4C123_BOARD, MSP432P4XX_BOARD */

/**
 */
typedef pin_names_t pin_name;

#ifdef __cplusplus
}
#endif

