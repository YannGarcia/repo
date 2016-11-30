/**
 * @file    main.c
 * @brief   Bouncing robot software application for BerkeleyX: EE40LX Electronic Interfaces
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 1.0
 * @see http://www.ti.com/tool/ek-tm4c1294xl
 * @see https://courses.edx.org/asset-v1:BerkeleyX+EE40LX+2T2015+type@asset+block/code_flowchart.pdf
 */

#include "libhal.h" /* Hardware Abstraction Level library (Beagle Bone, Raspbery MBED & TM4C1294 */
#include "libhal_shift.h"

/**
 * @defgroup Booststarter #1 pin assignment
 *
 * @{
 */
#define LED3     p108    /** On board LED1, used for debug - MSB 4 bytes state machine status */
#define LED2     p107    /** On board LED2, used for debug */
#define LED1     p46     /** On board LED3, used for debug */
#define LED0     p42     /** On board LED4, used for debug - LSB 4 bytes state machine status */
#define DATA     p81     /** Data  - BoosterPack#1: C1.3 - xxx wire */
#define W_CLK    p45     /** Clock - BoosterPack#1: C1.3 - xxx wire */
#define FQ_UD    p43     /** FQ_UD - BoosterPack#1: C1.1 - xxx wire */
#define RESET    p44     /** Reset - BoosterPack#1: C1.2 - xxx wire */
#define SIGNAL   A3      /** Sine wave #1 - BoosterPack#1: B1.3 - Yellow wire */
#define INC_FREQ p116    /** PUSH1 */
#define DEC_FREQ p117    /** PUSH2 */
/** @} */

/**
 * @fn void set_debug_state(const uint8_t p_state)
 * @brief Display the current debug state using the four on-board LEDs
 * @param[in] p_state The state value to display (between 0b0000 to 0b1111)
 */
void set_debug_state(const uint8_t p_state);
/**
 * @fn void adjust_frequency(const uint32_t p_frequency)
 * @brief Serial mode programming of the AD9851
 * @param[in] p_frequency The desired frequency in Hz
 * @see AD9851 Rev.D datashhet, page 12
 */
void adjust_frequency(const uint32_t p_frequency);

/**
 * @brief Main function of the bouncing robot software application
 * @return Shall never returned
 */
int32_t main(void) {
  float adc_micro = 0.0;
  float frequency_factor = 4294967296.0 / 180.0e6;
  float desired_frequency = 1e6;
  uint32_t frequency_tuning_word; // desired_frequency * 4294967296.0 / 180.0e6;
  bool frequency_changed = false;
  
  /* Initialise the HAL */
  libhal_setup();

  /* State 0: display 0b1111 then 0x0000 */
  set_debug_state(0);

  /* Setup output ports */
  pin_mode(DATA, gpio_modes_digital_output);
  pin_mode(W_CLK, gpio_modes_digital_output);
  pin_mode(FQ_UD, gpio_modes_digital_output);
  pin_mode(RESET, gpio_modes_digital_output);

  /* Setup analog ports */
  pin_mode(SIGNAL, gpio_modes_adc_input);

  /* Setup GPIO input ports */
  pin_mode(INC_FREQ, gpio_modes_digital_input);
  pull_up_dn_control(INC_FREQ, pud_up);         /* Configuration of the pull-up is required for USR_SW1/2 */
  pin_mode(DEC_FREQ, gpio_modes_digital_input);
  pull_up_dn_control(DEC_FREQ, pud_up);         /* Configuration of the pull-up is required for USR_SW1/2 */

  // Reset AD9851 (AD9851 Rev.D datasheet Figure 14. Master Reset Timing Sequence)
  digital_write(RESET, digital_state_high);
  digital_write(RESET, digital_state_low);

  // Set serial load enable (AD9851 Rev.D datasheet Figure 17. Serial Load Enable Sequence)
  digital_write(W_CLK, digital_state_high);
  digital_write(W_CLK, digital_state_low);
  digital_write(FQ_UD, digital_state_high);
  digital_write(FQ_UD, digital_state_low);
  
  /* State 1: initialisation done */
  set_debug_state(1);

  // Adjust frequency
  set_debug_state(2);
  frequency_tuning_word = (uint32_t)(desired_frequency * frequency_factor);
  adjust_frequency(frequency_tuning_word);
  
  while (true) {
    // Check buttons
    set_debug_state(3);

    // Process user action
    set_debug_state(4);

    // Adjust frequency
    if (frequency_changed) {
      set_debug_state(4);
      frequency_tuning_word = (uint32_t)(desired_frequency * frequency_factor);
      adjust_frequency(frequency_tuning_word);
      frequency_changed = false;
    }
    
  } /* End of 'while' statement */

  return 0;
} /* End of main function*/

void adjust_frequency(const uint32_t p_frequency) {
  // Send the tuned frequency word
  uint8_t frequency;
  for (uint8_t i = 0; i < 4; i++) {
    frequency = (uint8_t)((p_frequency >> (i * 8)) & 0xff);
    libhal_shift_out(DATA, W_CLK, shift_modes_lsb_first, frequency);
  }
  // Send the phase modulation word,
  libhal_shift_out(DATA, W_CLK, shift_modes_lsb_first, 0x01);
  // Flush the data
  digital_write(FQ_UD, digital_state_high);
  digital_write(FQ_UD, digital_state_low);
}

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
