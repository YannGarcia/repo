/**
 * @file    main.c
 * @brief   Application to validate libhal libary
 * @author  garciay.yann@gmail.com
 * @license This project is released under the MIT License
 * @version 1.0
 * @see http://www.ti.com/tool/ek-tm4c1294xl
 */

#include "libhal.h" /* Hardware Abstraction Level library (Beagle Bone, Raspbery MBED & TI launchpads */
#include "libhal_serial.h"

/**
 * @defgroup Booststarter #1 pin assignment
 *
 * @{
 */
#define LED3 p108      /** On board LED1, used for debug - MSB 4 bytes state machine status */
#define LED2 p107      /** On board LED2, used for debug */
#define LED1 p46       /** On board LED3, used for debug */
#define LED0 p42       /** On board LED4, used for debug - LSB 4 bytes state machine status */
#define SW1  p116      /** On board Switch1 */
#define SW2  p117      /** On board Switch2 */
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
  uint8_t debug_state = 0;

  /* Initialise the HAL */
  libhal_setup();

  /* State 0: display 0b1111 then 0x0000 */
  set_debug_state(debug_state);

  /* Setup GPIO input ports */
  pin_mode(SW1, gpio_modes_digital_input);
  pull_up_dn_control(SW1, pud_up);         /* Configuration of the pull-up is required for USR_SW1 */
  pin_mode(SW2, gpio_modes_digital_input);
  pull_up_dn_control(SW2, pud_up);         /* Configuration of the pull-up is required for USR_SW2 */

  while (true) {
    /* Wait event on Switch1 */
    while (digital_read(SW2) == digital_state_low);

    /* Increment debug_state */
    debug_state = (debug_state + 1) % 8;

    /* Change debug state */
    set_debug_state(debug_state);

    /* wait for 500 ms */
    wait_ms(500);
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
