/**
 * @file      main.c
 * @brief     Application to validate libhal libary
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2016 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   1.0
 * @see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * @see       EK-TM4C129EXL Firmware Development Package User's Guide
 * @see       EK-TM4C1294XL Firmware Development Package User's Guide
 */

#include "libhal.h" /* Hardware Abstraction Level library (Beagle Bone, Raspbery MBED & TI launchpads */
#include "libhal_serial.h"
#include "libhal_i2c.h"

/**
 * @defgroup Booststarter #1 pin assignment
 *
 * @{
 */
#define LED3       p108      /** On board LED1, used for debug - MSB 4 bytes state machine status */
#define LED2       p107      /** On board LED2, used for debug */
#define LED1       p46       /** On board LED3, used for debug */
#define LED0       p42       /** On board LED4, used for debug - LSB 4 bytes state machine status */
#define SW1        p116      /** On board Switch1 */
#define SW2        p117      /** On board Switch2 */

#define L_RED      p44
#define L_GREEN    p45
#define L_BLUE     p49
#define LIGHT_INT  p22 /** TI OPT3001 Light Sensor on BOOSTXL-EDUMKII Educational BoosterPack™ Mark II Plug-in Module See SLAU599–August 2015 Clause 2.1.2 TI OPT3001 Light Sensor */
/** @} */

/**
 * @fn void set_debug_state(const uint8_t p_state)
 * @brief Display the current debug state using the four on-board LEDs
 * @param[in] p_state The state value to display (between 0b0000 to 0b1111)
 */
void set_debug_state(const uint8_t p_state);

/**
 * @brief Main function of the bouncing robot software application
 * @return Shall never returned
 */
int32_t main(void) {
  uint8_t debug_state = 0; /** Debug state status */
  int32_t uart0;           /** UART handle */
  int32_t i2c0;            /** I2C bus 0 handle */
  uint32_t counter = 0;    /** Counter */

  /* Initialise the HAL */
  libhal_setup();

  /* State 0: display 0b1111 then 0x0000 */
  set_debug_state(debug_state);

  /* Setup GPIO input ports */
  pin_mode(SW1, gpio_modes_digital_input);
  pull_up_dn_control(SW1, pud_up);         /* Configuration of the pull-up is required for USR_SW1 */
  pin_mode(SW2, gpio_modes_digital_input);
  pull_up_dn_control(SW2, pud_up);         /* Configuration of the pull-up is required for USR_SW2 */

  // Open serial console
  uart0 = serial_open("/dev/tty0", 115200);
  debug_state += 1;
  set_debug_state(debug_state);

  // Setup I2C bus 0 to access TI OPT3001 Light Sensor
  i2c0 = libhal_i2c_setup(0/*"/dev/i2c0"*/, 0x44); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.3.4.1 Serial Bus Address
  digital_toggle(L_BLUE); // Marker between two I2C transactions
  // Read Manufacturer ID
  uint16_t data = libhal_i2c_read_register16(i2c0, 0x7E); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  // Read Device ID
  data = libhal_i2c_read_register16(i2c0, 0x7F); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  wait_us(1);
  // Read config
  data = libhal_i2c_read_register16(i2c0, 0x01); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  wait_us(1);
  // Read Low Limit
  data = libhal_i2c_read_register16(i2c0, 0x02); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  wait_us(1);
  // Read High Limit
  data = libhal_i2c_read_register16(i2c0, 0x03); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  debug_state += 1;
  set_debug_state(debug_state);

  while (true) {
    serial_printf(uart0, "Please push User button #2 (%d)\r\n", counter++);

    /* Wait event on Switch1 */
    while (digital_read(SW2) == digital_state_low);

    /* Increment debug_state */
    debug_state = (debug_state + 1) % 8;

    /* Change debug state */
    set_debug_state(debug_state);

//    digital_toggle(L_BLUE); // Marker between two I2C transactions
    // Read Result register
    data = libhal_i2c_read_register8(i2c0, 0x00);
//    digital_toggle(L_BLUE); // Marker between two I2C transactions
    serial_printf(uart0, "Light sensor value: %d\r\n", data);

    /* wait for 50 ms for button debouncing */
    wait_ms(50);
  } /* End of 'while' statement */

  return 0;
} /* End of main function*/

void set_debug_state(const uint8_t p_state) {
  switch (p_state) {
  case 0x00:
    digital_write(LED3, digital_state_high);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
    wait_ms(50);
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_low);
    break;
  case 0x01:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_high);
    break;
  case 0x02:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_low);
    break;
  case 0x03:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
    break;
  case 0x04:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_low);
    break;
  case 0x05:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_high);
    break;
  case 0x06:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_low);
    break;
  case 0x07:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
    break;
  case 0x08:
    digital_write(LED3, digital_state_high);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_low);
    break;
  default:
    digital_write(LED3, digital_state_high);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
  } /* End of 'switch' statement */

} /* End of function set_debug_state */
