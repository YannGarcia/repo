/**
 * \file      libhal_pin_names.ti_msp_exp432p401r.h
 * \brief     Header file for the Hardware Abstract Layer pins mapping.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 * \see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * \see       EK-TM4C129EXL Firmware Development Package User's Guide
 * \see       EK-TM4C1294XL Firmware Development Package User's Guide
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "driverlib/driverlib.h"

/** \enum pin_names_t
 * \brief Describes the pin name/pin kernel number
 *        Analog conversions only done on PORTB and PORTE pins (ADC0 module)
 *        PWM and analog comparators use ADC1 module
 * \see SLAS826 –MARCH 2015 Table Table 6-19. Port Mapping, Mnemonics, and Functions
 * \see MSP432P401R LaunchPad™ Development Kit (MSP‑EXP432P401R) Clause 2.7 BoosterPack Plug-in Module Pinout
 */
typedef enum {
  p1 = GPIO_PORT_P10 << 16 | GPIO_PIN1,
  p2 = GPIO_PORT_P10 << 16 | GPIO_PIN2,
  p3 = GPIO_PORT_P10 << 16 | GPIO_PIN3,
  p4 = GPIO_PORT_P1 << 16 | GPIO_PIN0,   /* On board LED1 */
  p5 = GPIO_PORT_P1 << 16 | GPIO_PIN1,   /* On board Switch 1 */
  p6 = GPIO_PORT_P1 << 16 | GPIO_PIN2,
  p7 = GPIO_PORT_P1 << 16 | GPIO_PIN3,
  p8 = GPIO_PORT_P1 << 16 | GPIO_PIN4,   /* On board Switch 2 */
  p9 = GPIO_PORT_P1 << 16 | GPIO_PIN5,   /* SPI CLK */
  p10 = GPIO_PORT_P1 << 16 | GPIO_PIN6,  /* SPI MOSI */
  p11 = GPIO_PORT_P1 << 16 | GPIO_PIN7,  /* SPI MISO */
  p16 = GPIO_PORT_P2 << 16 | GPIO_PIN0,  /* On board RGB led Red */
  p17 = GPIO_PORT_P2 << 16 | GPIO_PIN1,  /* On board RGB led Green */
  p18 = GPIO_PORT_P2 << 16 | GPIO_PIN2,  /* On board RGB led Blue */
  p19 = GPIO_PORT_P2 << 16 | GPIO_PIN3,
  p20 = GPIO_PORT_P2 << 16 | GPIO_PIN4,
  p21 = GPIO_PORT_P2 << 16 | GPIO_PIN5,  /* PWM */
  p22 = GPIO_PORT_P2 << 16 | GPIO_PIN6,
  p23 = GPIO_PORT_P2 << 16 | GPIO_PIN7,
  p24 = GPIO_PORT_P10 << 16 | GPIO_PIN4,
  p25 = GPIO_PORT_P10 << 16 | GPIO_PIN5,
  p26 = GPIO_PORT_P7 << 16 | GPIO_PIN4,
  p27 = GPIO_PORT_P7 << 16 | GPIO_PIN5,
  p28 = GPIO_PORT_P7 << 16 | GPIO_PIN6,
  p29 = GPIO_PORT_P7 << 16 | GPIO_PIN7,
  p30 = GPIO_PORT_P8 << 16 | GPIO_PIN1,
  p31 = GPIO_PORT_P8 << 16 | GPIO_PIN0,
  p32 = GPIO_PORT_P3 << 16 | GPIO_PIN0,
  p33 = GPIO_PORT_P3 << 16 | GPIO_PIN1,
  p34 = GPIO_PORT_P3 << 16 | GPIO_PIN2,
  p35 = GPIO_PORT_P3 << 16 | GPIO_PIN3,
  p36 = GPIO_PORT_P3 << 16 | GPIO_PIN4,
  p37 = GPIO_PORT_P3 << 16 | GPIO_PIN5,
  p38 = GPIO_PORT_P3 << 16 | GPIO_PIN6,
  p39 = GPIO_PORT_P3 << 16 | GPIO_PIN7,
  p41 = GPIO_PORT_PJ << 16 | GPIO_PIN0,
  p42 = GPIO_PORT_PJ << 16 | GPIO_PIN1,
  p46 = GPIO_PORT_P8 << 16 | GPIO_PIN2,
  p47 = GPIO_PORT_P8 << 16 | GPIO_PIN3,
  p48 = GPIO_PORT_P8 << 16 | GPIO_PIN4,
  p49 = GPIO_PORT_P8 << 16 | GPIO_PIN5,
  p50 = GPIO_PORT_P8 << 16 | GPIO_PIN6,
  p51 = GPIO_PORT_P8 << 16 | GPIO_PIN7,
  p52 = GPIO_PORT_P9 << 16 | GPIO_PIN0,
  p53 = GPIO_PORT_P9 << 16 | GPIO_PIN1,
  p54 = GPIO_PORT_P6 << 16 | GPIO_PIN0,
  p55 = GPIO_PORT_P6 << 16 | GPIO_PIN1,
  p56 = GPIO_PORT_P4 << 16 | GPIO_PIN0,
  p57 = GPIO_PORT_P4 << 16 | GPIO_PIN1,
  p58 = GPIO_PORT_P4 << 16 | GPIO_PIN2,
  p59 = GPIO_PORT_P4 << 16 | GPIO_PIN3,
  p60 = GPIO_PORT_P4 << 16 | GPIO_PIN4,
  p61 = GPIO_PORT_P4 << 16 | GPIO_PIN5,
  p62 = GPIO_PORT_P4 << 16 | GPIO_PIN6,
  p63 = GPIO_PORT_P4 << 16 | GPIO_PIN7,
  p64 = GPIO_PORT_P5 << 16 | GPIO_PIN0,
  p65 = GPIO_PORT_P5 << 16 | GPIO_PIN1,
  p66 = GPIO_PORT_P5 << 16 | GPIO_PIN2,
  p67 = GPIO_PORT_P5 << 16 | GPIO_PIN3,
  p68 = GPIO_PORT_P5 << 16 | GPIO_PIN4,
  p69 = GPIO_PORT_P5 << 16 | GPIO_PIN5,
  p70 = GPIO_PORT_P5 << 16 | GPIO_PIN6,
  p71 = GPIO_PORT_P5 << 16 | GPIO_PIN7,
  p74 = GPIO_PORT_P9 << 16 | GPIO_PIN2,
  p75 = GPIO_PORT_P9 << 16 | GPIO_PIN3,

  A0 = GPIO_PORT_P5 << 16 | GPIO_PIN5,      /* MCU pin #69 */
  A1 = GPIO_PORT_P5 << 16 | GPIO_PIN4,      /* MCU pin #68 */
  A2 = GPIO_PORT_P5 << 16 | GPIO_PIN3,      /* MCU pin #67 */
  A3 = GPIO_PORT_P5 << 16 | GPIO_PIN2,      /* MCU pin #66 */
  A4 = GPIO_PORT_P5 << 16 | GPIO_PIN1,      /* MCU pin #65 */
  A5 = GPIO_PORT_P5 << 16 | GPIO_PIN0,      /* MCU pin #64 */
  A6 = GPIO_PORT_P4 << 16 | GPIO_PIN7,      /* MCU pin #63 */
  A7 = GPIO_PORT_P4 << 16 | GPIO_PIN6,      /* MCU pin #62 */

  A9 = GPIO_PORT_P4 << 16 | GPIO_PIN4,      /* MCU pin #60 */
  A10 = GPIO_PORT_P4 << 16 | GPIO_PIN3,     /* MCU pin #59 */
  A11 = GPIO_PORT_P4 << 16 | GPIO_PIN2,     /* MCU pin #58 */
  A12 = GPIO_PORT_P4 << 16 | GPIO_PIN1,     /* MCU pin #57 */
  A13 = GPIO_PORT_P4 << 16 | GPIO_PIN0,     /* MCU pin #56 */
  A14 = GPIO_PORT_P6 << 16 | GPIO_PIN1,     /* MCU pin #55 */
  A15 = GPIO_PORT_P6 << 16 | GPIO_PIN0,     /* MCU pin #54 */

  pwm0 = GPIO_PORT_P2 << 16 | GPIO_PIN5,    /*p21 */

  NC = 0xffffffff
} pin_names_t;

/**
 */
typedef pin_names_t pin_name;

#ifdef __cplusplus
}
#endif

