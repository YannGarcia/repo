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
#define PBLOCK p81     /** Power command for the photoresistors measurements - BoosterPack#1: C1.7 - Green wire */
#define BUZZER p45     /** BUZZER command - BoosterPack#1: C1.3 - White wire */
#define LMOTOR p43     /** Left MOTOR command - BoosterPack#1: C1.1 - Salmon wire */
#define RMOTOR p44     /** Right MOTOR command - BoosterPack#1: C1.2 - Salmon wire */
#define LPHOTO p82     /** Left photoresistor measurement - BoosterPack#1: C1.8 - Brown wire */
#define RPHOTO p83     /** Right photoresistor measurement - BoosterPack#1: C1.9 - Blue wire */
#define MICINP A3      /** Microphon measurement - BoosterPack#1: B1.3 - Yellow wire */
/** @} */

/**
 * @fn void set_debug_state(const uint8_t p_state)
 * @brief Display the current debug state using the four on-board LEDs
 * @param[in] p_state The state value to display (between 0b0000 to 0b1111)
 */
void set_debug_state(const uint8_t p_state);
/**
 * @fn void snooze(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t duration, const uint8_t p_repeat, const uint32_t p_wait)
 * @brief Generate a tone based on a PWM signal
 * @param[in] p_frequency  The frequency of the PWM signal
 * @param[in] p_duty_cycle The duty cycle of the PWM signal
 * @param[in] p_duration   The delay between two tone repeatition
 * @param[in] p_repeat     Indicates how many time to repeat the tone
 * @param[in] p_wait       The delay between two repeatition
 */
void snooze(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat, const uint32_t p_wait);
/**
 * @fn void siren(const float p_frequency_low, const float p_frequency_high, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat)
 * @brief Make the "siren" noise by alternating high frequency (e.g. 1200Hz) and low frequency (e.g. 800Hz) tones
 * @param[in] p_frequency_low  The frequency of the PWM signal
 * @param[in] p_frequency_high The frequency of the PWM signal
 * @param[in] p_duty_cycle     The duty cycle of the PWM signal
 * @param[in] p_duration       The delay between two tone repeatition
 * @param[in] p_repeat         Indicates how many time to repeat the tone
 */
void siren(const float p_frequency_low, const float p_frequency_high, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat);
/**
 * @fn void pyramid(const float p_frequency_low, const float p_frequency_high, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat)
 * @brief Make the "pyramid" noise by alternating high frequency (e.g. 1200Hz) and low frequency (e.g. 800Hz) tones
 * @param[in] p_frequency_low  The frequency of the PWM signal
 * @param[in] p_frequency_high The frequency of the PWM signal
 * @param[in] p_duty_cycle     The duty cycle of the PWM signal
 * @param[in] p_duration       The delay between two tone repeatition
 * @param[in] p_repeat         Indicates how many time to repeat the tone
 */
void pyramid(const float p_frequency_low, const float p_frequency_high, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat);
/**
 * @fn void left_motor_on(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t duration, const uint8_t p_repeat)
 * @brief Generate a PWM signal on left motor
 * @param[in] p_frequency  The frequency of the PWM signal
 * @param[in] p_duty_cycle The duty cycle of the PWM signal
 * @param[in] p_duration   The delay between two tone repeatition
 * @param[in] p_repeat     Indicates how many time to repeat the motor activation
 */
void left_motor_on(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat);
/**
 * @fn void right_motor_on(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t duration, const uint8_t p_repeat)
 * @brief Generate a PWM signal on left motor
 * @param[in] p_frequency  The frequency of the PWM signal
 * @param[in] p_duty_cycle The duty cycle of the PWM signal
 * @param[in] p_duration   The delay between two tone repeatition
 * @param[in] p_repeat     Indicates how many time to repeat the motor activation
 */
void right_motor_on(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat);

/**
 * @brief Main function of the bouncing robot software application
 * @return Shall never returned
 */
int32_t main(void) {
  float adc_micro = 0.0;

  /* Initialise the HAL */
  libhal_setup();

  /* State 0: display 0b1111 then 0x0000 */
  set_debug_state(0);

  /* Setup PWM ports */
  pin_mode(BUZZER, gpio_modes_pwm_output);
  pin_mode(LMOTOR, gpio_modes_pwm_output);
  pin_mode(RMOTOR, gpio_modes_pwm_output);

  /* Setup analog ports */
  pin_mode(MICINP, gpio_modes_adc_input);

  /* Setup GPIO output ports */
  pin_mode(PBLOCK, gpio_modes_digital_output);

  /* Setup GPIO input ports */
  pin_mode(LPHOTO, gpio_modes_digital_input);
  pull_up_dn_control(LPHOTO, pud_down);         /* Configuration of the pull-up is required for USR_SW1/2 */
  pin_mode(RPHOTO, gpio_modes_digital_input);
  pull_up_dn_control(RPHOTO, pud_down);         /* Configuration of the pull-up is required for USR_SW1/2 */

  /* State 1: initialisation done */
  set_debug_state(1);

  while (true) {
    /* 1. Beep four times: PWM 900Hz, 50% during 50ms, 4 times */
    set_debug_state(2);
    snooze(1000, 60, 100, 4, 500);

    /* 2. Check micro */
    set_debug_state(3);
    adc_micro = analog_read(MICINP);
    if (adc_micro < 1.65) {
        // Turn on PWM to L.R. Motor
        set_debug_state(4);
        siren(800, 12000, 50, 10, 5);
        left_motor_on(20000, 25, 200, 1);
        right_motor_on(20000, 25, 200, 1);
        pyramid(100, 300, 50, 10, 1);
    }

    /* 2. Check photoresistor */
    set_debug_state(5);
    digital_write(PBLOCK, digital_state_high); // Power up
    wait_ms(25); // Wait a little beat
    set_debug_state(6);
    // Read left photoresistor
    if (digital_read(LPHOTO) == digital_state_high) {
      // Turn on PWM to L. Motor
      set_debug_state(7);
      left_motor_on(20000, 60, 1000, 1);
    }
    // Read Right photoresistor
    if (digital_read(RPHOTO) == digital_state_high) {
      // Turn on PWM to R. Motor
      set_debug_state(8);
      right_motor_on(20000, 60, 1000, 1);
    }
    digital_write(PBLOCK, digital_state_low); // Power down

  } /* End of 'while' statement */

  return 0;
} /* End of main function*/

void snooze(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat, const uint32_t p_wait) {
  /* Sanity check */
  if ((p_frequency == 0) || (p_repeat == 0)) {
    /* Nothing to do */
    return;
  }
  uint8_t repeat = 0x00;
  pwm_set_clock(BUZZER, 32);
  pwm_set_range(BUZZER, p_frequency);
  for ( ; repeat < p_repeat - 1; repeat++) {
    pwm_write(BUZZER, p_duty_cycle);
    wait_ms(p_duration);
    pwm_write(BUZZER, 0);
    wait_ms(p_wait);
  } /* End of 'for' statement */
  pwm_write(BUZZER, p_duty_cycle);
  wait_ms(p_duration);
  pwm_write(BUZZER, 0);

} /* End of function snooze */

void siren(const float p_frequency_low, const float p_frequency_high, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat) {
  uint8_t repeat = 0;
  for ( ; repeat < p_repeat; repeat++) {
    snooze(p_frequency_high, p_duty_cycle, 100, 1, 0);
    snooze(p_frequency_low, p_duty_cycle, 100, 1, 0);
  } /* End of 'for' statement */
} /* End of function siren */

void pyramid(const float p_frequency_low, const float p_frequency_high, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat) {
  uint8_t repeat = 0;
  for ( ; repeat < p_repeat; repeat++) {
    float frequency = p_frequency_low;
    for ( ; frequency < p_frequency_high; frequency++) {
      snooze(frequency, p_duty_cycle, p_duration, 1, 0);
    } /* End of 'for' statement */
    for (frequency = p_frequency_high; frequency < p_frequency_low; frequency--) {
      snooze(frequency, p_duty_cycle, p_duration, 1, 0);
    } /* End of 'for' statement */
  } /* End of 'for' statement */
} /* End of function siren */

void left_motor_on(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat) {
  uint8_t repeat = 0x00;
  pwm_set_clock(LMOTOR, 32);
  pwm_set_range(LMOTOR, p_frequency);
  for ( ; repeat < p_repeat; repeat++) {
    pwm_write(LMOTOR, p_duty_cycle);
    wait_ms(p_duration);
    pwm_write(LMOTOR, 0);
  } /* End of 'for' statement */
} /* End of function left_motor_on */

void right_motor_on(const float p_frequency, const uint32_t p_duty_cycle, const uint32_t p_duration, const uint8_t p_repeat) {
  uint8_t repeat = 0x00;
  pwm_set_clock(RMOTOR, 32);
  pwm_set_range(RMOTOR, p_frequency);
  for ( ; repeat < p_repeat; repeat++) {
    pwm_write(RMOTOR, p_duty_cycle);
    wait_ms(p_duration);
    pwm_write(RMOTOR, 0);
  } /* End of 'for' statement */
} /* End of function right_motor_on */

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
