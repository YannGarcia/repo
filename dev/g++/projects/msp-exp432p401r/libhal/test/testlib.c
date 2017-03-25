/**
 * @file      main.c
 * @brief     Application to validate libhal libary
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2016-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   1.0
 * @see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * @see       EK-TM4C129EXL Firmware Development Package User's Guide
 * @see       EK-TM4C1294XL Firmware Development Package User's Guide
 */

#include "libhal.h" /* Hardware Abstraction Level library (Beagle Bone, Raspbery MBED & TI launchpads */

/*!<
 * @defgroup Booststarter #1 pin assignment
 * @{
 */
#define LED1       p4  /*!< On board LED */
#define SW1        p5  /*!< On board Switch1 */
#define SW2        p8  /*!< On board Switch2 */

#define L_RED      p16
#define L_GREEN    p17
#define L_BLUE     p18
/*!< @} */

/*!<
 * @brief Main function of the bouncing robot software application
 * @return Shall never returned
 */
int32_t main(void) {
  uint32_t ii;

  /* Initialise the HAL */
  libhal_setup();

  digital_write(LED1, digital_state_high);

  /* Setup GPIO input ports */
  pin_mode(SW1, gpio_modes_digital_input);
  pull_up_dn_control(SW1, pud_up);         /* Configuration of the pull-up is required for USR_SW1 */
  pin_mode(SW2, gpio_modes_digital_input);
  pull_up_dn_control(SW2, pud_up);         /* Configuration of the pull-up is required for USR_SW2 */

  while (true) {
    /* Wait event on Switch1 - PullUp ==> pushed = digital_state_low */
    while (digital_read(SW1) == digital_state_high);
    /* wait for 170 ms for button debouncing */
    for(ii=0;ii<1700;ii++);

    digital_write(L_BLUE, digital_state_high);
    for(ii=0;ii<5000;ii++);

    digital_write(L_RED, digital_state_high);
    for(ii=0;ii<5000;ii++);

    /* Wait event on Switch2 */
    while (digital_read(SW2) == digital_state_high);
    /* wait for 170 ms for button debouncing */
    for(ii=0;ii<1700;ii++);

    digital_write(L_BLUE, digital_state_low);
    for(ii=0;ii<5000;ii++);

    digital_write(L_RED, digital_state_low);
    for(ii=0;ii<5000;ii++);
  } /* End of 'while' statement */

  return 0;
} /* End of main function*/

