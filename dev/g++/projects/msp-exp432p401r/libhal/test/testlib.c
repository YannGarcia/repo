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
#include <limits.h>

#include "libhal.h" /* Hardware Abstraction Level library (Beagle Bone, Raspbery MBED & TI launchpads */
#include "libhal_serial.h"

/*!<
 * @defgroup Booststarter #1 pin assignment
 * @{
 */
#define LED1       p4  /*!< On board LED */
#define SW1        p5  /*!< On board Switch1 */
#define SW2        p8  /*!< On board Switch2 */

#define L_RED      p22
#define L_GREEN    p20
#define L_BLUE     p70

#define LIGHT_INT  p22 /*!< TI OPT3001 Light Sensor on BOOSTXL-EDUMKII Educational BoosterPack™ Mark II Plug-in Module See SLAU599–August 2015 Clause 2.1.2 TI OPT3001 Light Sensor */

#define BUT1       p65 /*!< MK-II button 1 */
#define BUT2       p37 /*!< MK-II button 1 */

#define J_SW       p57 /*!< Select button - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */
#define J_X        A9  /*!< Horizontal X-axis - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */
#define J_Y        A15 /*!< Horizontal Y-axis - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */

#define A_X        A14 /*!< Accelerometer, X-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */
#define A_Y        A13 /*!< Accelerometer, Y-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */
#define A_Z        A11 /*!< Accelerometer, Z-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */

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
  int32_t uart0;           /*!< UART handle */
  int32_t i2c0;            /*!< I2C bus 0 handle */
  uint32_t counter = 0;    /*!< Counter */
  uint8_t spi_cmd_rsp;     /*!< 8bit SPI command/response buffer */
  float j_x, j_y;          /*!< Joystick positions */
  float values[3] = {0};   /*!< X,Y,Z accelerator positions */

  int8_t float2str[8];

  /* Initialise the HAL */
  libhal_setup();

  digital_write(LED1, digital_state_high);

  /* Setup GPIO input ports */
  pin_mode(SW1, gpio_modes_digital_input);
  pull_up_dn_control(SW1, pud_up);         /* Configuration of the pull-up is required for USR_SW1 */
  pin_mode(SW2, gpio_modes_digital_input);
  pull_up_dn_control(SW2, pud_up);         /* Configuration of the pull-up is required for USR_SW2 */

  /* Setup GPIO input ports for MKII buttons, see MKII Circuit Diagram */
  pin_mode(BUT1, gpio_modes_digital_input);
  pin_mode(BUT2, gpio_modes_digital_input);

  // Open serial console
  uart0 = serial_open("/dev/tty0", 115200);

  // Setup Joystick
  pin_mode(J_SW, gpio_modes_digital_input);
  pin_mode(J_X, gpio_modes_adc_input);
  pin_mode(J_Y, gpio_modes_adc_input);

  // Setup accelerator
  const pin_name acc_pins[3] = {A_X, A_Y, A_Z};
  pins_mode(acc_pins, 3, gpio_modes_adc_input);
  analog_multiple_read(acc_pins, 3, values);

  // Setup I2C bus 0 to access TI OPT3001 Light Sensor

  // Setup TFT screen

  while (true) {
    serial_printf(uart0, "Please push User button #1 (%d)\r\n", counter++);

    /* Wait event on MK-II/BUT1
     * MK-II-Circuit diagram (SLAU599–August 2015) indicates a PullUp ==> pushed = digital_state_low
     */
    while (digital_read(BUT1) == digital_state_high);
    /* wait for 170 ms for button debouncing */
    wait_ms(170);

    // Joystick position
    if ((j_x = analog_read(J_X)) == (float)LONG_MAX) {
      serial_printf(uart0, "Joystick X: analog_read failure\r\n");
    } else if ((j_y = analog_read(J_Y)) == (float)LONG_MAX) {
      serial_printf(uart0, "Joystick Y: analog_read failure\r\n");
    } else {
      ftoa(j_x, float2str, 5);
      serial_printf(uart0, "Joystick position: (%s, ", float2str);
      ftoa(j_y, float2str, 5);
      serial_printf(uart0, "%s)\r\n", float2str, 5);
    }
    // Accelerator values
//    analog_multiple_read(acc_pins, 3, values);
//    ftoa(values[0], float2str, 5);
//    serial_printf(uart0, "Accelerator (X, Y, Z): (%s, ", float2str, 5);
//    ftoa(values[1], float2str, 5);
//    serial_printf(uart0, "%s, ", float2str, 5);
//    ftoa(values[2], float2str, 5);
//    serial_printf(uart0, "%s)\r\n", float2str, 5);
  } /* End of 'while' statement */

  return 0;
} /* End of main function*/

