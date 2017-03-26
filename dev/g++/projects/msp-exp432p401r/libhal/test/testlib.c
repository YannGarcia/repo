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
#include "libhal_serial.h"

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

#define J_SW       p23 /*!< Select button - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */
#define J_X        A7  /*!< Horizontal X-axis - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */
#define J_Y        A0  /*!< Horizontal Y-axis - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */

#define A_X        A3  /*!< Accelerometer, X-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */
#define A_Y        A2  /*!< Accelerometer, Y-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */
#define A_Z        A1  /*!< Accelerometer, Z-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */

#define TFT_CS     p109 /*!< TFT /CS - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_MOSI   p2   /*!< TFT SPI MOSI - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_CLK    p4   /*!< TFT SPI clock - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_RST    p32  /*!< TFT reset - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_RS     p84  /*!< TFT command indicator - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_WIDTH  128  /*!< TFT screen width in pixels - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_HEIGHT 128  /*!< TFT screen heigh in pixels - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
/*!<
 * @defgroup TFT screen color descriptions
 * @{
 */
#define TFT_BLACK   0x0000
#define TFT_BLUE    0x001F
#define TFT_RED     0xF800
#define TFT_GREEN   0x07E0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0
#define TFT_WHITE   0xFFFF
/*!< @} */

/*!< @} */

/*!<
 * @brief Main function of the bouncing robot software application
 * @return Shall never returned
 */
int32_t main(void) {
  uint32_t ii;
  int32_t uart0;           /*!< UART handle */
  uint32_t counter = 0;    /*!< Counter */
  int8_t float2str[8];

  /* Initialise the HAL */
  libhal_setup();

  digital_write(LED1, digital_state_high);

  /* Setup GPIO input ports */
  pin_mode(SW1, gpio_modes_digital_input);
  pull_up_dn_control(SW1, pud_up);         /* Configuration of the pull-up is required for USR_SW1 */
  pin_mode(SW2, gpio_modes_digital_input);
  pull_up_dn_control(SW2, pud_up);         /* Configuration of the pull-up is required for USR_SW2 */

  // Open serial console
  uart0 = serial_open("/dev/tty0", 115200);

  // Setup Joystick
  pin_mode(J_SW, gpio_modes_digital_input);
  pin_mode(J_X, gpio_modes_adc_input);
  pin_mode(J_Y, gpio_modes_adc_input);
  float j_x = analog_read(J_X);
  float j_y = analog_read(J_Y);

  while (true) {
    serial_printf(uart0, "Please push User button #1 (%d)\r\n", counter++);

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

