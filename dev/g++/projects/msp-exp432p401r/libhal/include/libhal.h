/**
 * \file      libhal.h
 * \brief     Main header file for the Hardware Abstract Layer library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2016-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 * \see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * \see       EK-TM4C129EXL Firmware Development Package User's Guide
 * \see       MSP432® Peripheral Driver Library - USER’S GUIDE
 */
#pragma once

#include "libhal_pin_names.h"

/** \def Handy defines
 */
#define    HAL_MODE_PINS           0
#define    HAL_MODE_GPIO           1
#define    HAL_MODE_GPIO_SYS       2
#define    HAL_MODE_PHYS           3
#define    HAL_MODE_PIFACE         4
#define    HAL_MODE_UNINITIALISED -1

#ifdef __cplusplus
extern "C" {
#endif

  /** \enum GPIO modes
   */
  typedef enum {
    gpio_modes_digital_input,
    gpio_modes_digital_output,
    gpio_modes_adc_input,
    gpio_modes_pwm_output,
    gpio_modes_clock
  } gpio_modes_t;

  /** \enum Digital GPIO state
   */
  typedef enum {
    digital_state_low,                /*!< Low digital GPIO level state (Vss) */
    digital_state_high,               /*!< High digital GPIO level state (Vcc) */
    digital_state_error
  } digital_state_t;

  /** \enum Pull-up/pull-down mode
   */
  typedef enum {
    pud_off,                          /*!< Pull-up/pull-down unset */
    pud_down,                         /*!< Pull-down set */
    pud_up,                           /*!< Pull-up set */
    pud_push_pull                     /*!< Push-pull set */
  } pud_t;

  /* PWM */

#define    HAL_PWM_MODE_MS          0
#define    HAL_PWM_MODE_BAL         1

  /** \enum Interrupt levels
   */
  typedef enum {
    isr_levels_setup,
    isr_levels_edge_falling,
    isr_levels_edge_rising,
    isr_levels_edge_both
  } isr_levels_t;

  /* Function prototypes */

  /**
   * \fn int32_t libhal_failure(const int32_t p_code, const char *p_message, ...)
   * \brief Display fatal error message, including error code
   * \param[in] p_code      Error code
   * \param[in] p_message   Error message to be displayed
   * \param[in] ...         Message argument (compliant with printf)
   * \return 0 on success, -1 otherwise
   */
  extern int32_t libhal_failure(const uint8_t p_code, const char *p_message, ...);
  /**
   * \fn int32_t  libhal_setup(void)
   * \brief Initialises the system into libhal Pin mode and uses the memory mapped hardware directly
   * \return 0 on success, -1 otherwise
   * \remark This function must be called once at the start of your program execution
   */
  extern int32_t libhal_setup(void);
  /**
   * \fn int32_t  libhal_setup_sys(void)
   * \brief Initialises the system into libhal Pin mode and uses the memory mapped hardware directly
   * \return 0 on success, -1 if the library was already initialised
   * \remark This function must be called once at the start of your program execution
   */
  extern int32_t libhal_setup_sys(void);
  /**
   * \fn int32_t  libhal_shutdown_sys(void)
   * \brief Uninitialize the Hardware Abstract Layer library
   * \remark This function shall be called once in the process
   * \return 0 on success, -1  if the library was already initialised
   */
  extern void libhal_shutdown_sys(void);
  /**
   * \fn int32_t pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode)
   * \brief Sets the mode of the specifed pin
   * \remark This function shall be called once in the process
   * \param[in] p_gpio      The GPIO identifier to be changed
   * \param[in] p_mode      The mode to be set
   * \return 0 on success, -1 otherwise
   */
  extern int32_t pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode);
  /**
   * \fn int32_t pins_mode(const pin_name * p_gpios, const uint32_t p_len, const gpio_modes_t p_mode)
   * \brief Sets the mode of the specifed pin
   * \remark This function shall be called once in the process
   * \param[in] p_gpio      The GPIO identifier to be changed
   * \param[in] p_mode      The mode to be set
   * \return 0 on success, -1 otherwise
   */
  extern int32_t pins_mode(const pin_name * p_gpios, const uint8_t p_len, const gpio_modes_t p_mode);
  /**
   * \fn void pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud)
   * \brief Control the internal pull-up/down resistors on a GPIO pin
   * \param[in] p_gpio      The GPIO identifier to be changed
   * \param[in] p_pud       The mode to be set
   * \return 0 on success, -1 otherwise
   * @todo To be implemented
   */
  extern int32_t pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud);
  /**
   * \fn void digital_read(const pin_name p_gpio)
   * \brief Read the value of a GPIO pin
   * \param[in] p_gpio      The GPIO identifier to be changed
   * \return The digital GPIO state on success, digital_state_error otherwise
   * @code
   *   pin_mode(SW1, gpio_modes_digital_input);
   *   pull_up_dn_control(SW1, pud_up);         // Configuration of the pull-up is required for USR_SW1
   *   while (digital_read(SW1) == digital_state_low);
   * @endcode
   */
  extern digital_state_t digital_read(const pin_name p_gpio);
  /**
   * \fn void digital_read(const pin_name p_gpio)
   * \brief Write the value of a GPIO pin
   * \param[in] p_gpio      The GPIO identifier to be changed
   * \param[in] p_value     The value to write
   */
  extern void digital_write(const pin_name p_gpio, const digital_state_t p_value);
  /**
   * \fn void digital_toggle(const pin_name p_gpio)
   * \brief Toggle the value of a GPIO pin (low -> high -> low)
   * \param[in] p_gpio      The GPIO identifier to be toggled
   */
  extern void digital_toggle(const pin_name p_gpio);
  /**
   * \fn float analog_read(const pin_name p_gpio)
   * \brief Read the analog value of the specified GPIO
   *        14-bit analog to digital converter, 0.73mV (between 0 and 3.3 volts)
   *        It uses the sequencer number SS3, see Tiva™ TM4C1294NCPDT Microcontroller Clause 15.3.1 Sample Sequencer
   * \param[in] p_gpio The analog input GPIO
   * \return The ADC value on success, LONG_MAX on error
   * \remark The function pin_mode() shall be called before analog_read()
   */
  extern float analog_read(const pin_name p_gpio);
  /**
   * \fn void analog_multiple_read(const pin_name * p_gpios, const uint8_t p_len, float * p_values)
   * \brief Read multiple analog channels at the same time
   *        14-bit analog to digital converter, 0.73mV (between 0 and 3.3 volts)
   *        It uses the sequencer number SS0, SS1 or SS2, see Tiva™ TM4C1294NCPDT Microcontroller Clause 15.3.1 Sample Sequencer
   * \param[in] p_gpios      The analog inputs
   * \param[in] p_len        The number of analog inputs (and values to return)
   * \param[out] p_values    The read values on success, LONG_MAX on error
   * \return 0 on success, -1 otherwise
   * \remark The function pins_mode() shall be called before analog_multiple_read()
   */
  extern int32_t  analog_multiple_read(const pin_name * p_gpios, const uint8_t p_len, float * p_values);
  /*  extern void analog_write(const pin_name p_gpio, int32_t p_value);*/
  /**
   * \fn void board_revision(void)
   * \brief Retrieve the number representing the hardware revision of the board
   * \return The revision board on success, -1 otherwise
   */
  extern int32_t board_revision(void);

//TODO  extern void digitalWriteByte(const uint8_t p_value);

  /**
   * \fn int32t pwm_start(const pin_name p_gpio, const float p_frequency, const float p_duty_cycle)
   * \brief Start a PWM signal on the specified GPIO identifier
   * \param[in] p_gpio          The GPIO pin identifier
   * \param[in] p_frequency     The frequency in Hertz of the PWM signal
   * \param[in] p_duty_cycle    The duty cycle in percent of the PWM signal, 0%: no PWM, 100% Full time PWM signal
   * \return 0 on success, -1 otherwise (such as no PWM gpio available)
   * \remark A duty cycle of 0% is not equivalent to a call to pwm_stop()
   */
  extern int32_t pwm_start(const pin_name p_gpio, const float p_frequency, const float p_duty_cycle);
  /**
   * \fn int32_t pwm_stop(const pin_name p_gpio);
   * \brief Stop PWM signal generation on the specify GPIO and free the GPIO resource
   * \param[in] p_gpio      The GPIO pwm pin identifier
   * \param[in] p_frequency
   * \return 0 on success, -1 otherwise
   */
  extern int32_t pwm_stop(const pin_name p_gpio);
  /**
   * \fn int32_t wait_for_interrupt(const pin_name p_gpio, uint32_t p_timeout)
   * \brief create an interrupt handler that will do a callback to the user supplied function
   * \param[in] p_gpio
   * \param[in] p_timeout
   */
  extern int32_t wait_for_interrupt(const pin_name p_gpio, uint32_t p_timeout);
  /**
   * \fn int32_t libhal_isr(const pin_name p_gpio, int32_t p_mode, int32_t (*p_function)(void *))
   * \brief create an interrupt handler that will do a callback to the user supplied function
   * \param[in] p_gpio
   * \param[in] p_mode
   * \param[in] p_function
   */
  extern int32_t libhal_isr(const pin_name p_gpio, const isr_levels_t p_levels, int32_t (*p_function)(void *));
  /**
   * \fn void wait_ms(const uint32_t p_delay_ms)
   * \brief Wait for the specified delay
   * \param[in] p_delay The delay to wait in milliseconds
   */
  extern void wait_ms(const uint32_t p_delay_ms);
  /**
   * \fn void wait_us(const uint32_t p_delay_us)
   * \brief Wait for the specified delay in microseconds
   * \param[in] p_delay_us The delay to wait in microseconds
   * @todo To be verified with BBB
   */
  extern void wait_us(const uint32_t p_delay_us);
  /**
   * \fn void reset_time(void)
   * \brief Reset internal timer
   */
  extern void reset_time(void);
  /**
   * \fn void elapse_ms(void)
   * \brief Retrieve the number of milliseconds since library initialisation or from the last call to \see reset_time()
   * \return The number of milliseconds since library initialisatino or from the last call to \see reset_time()
   */
  extern uint32_t elapse_ms(void);
  /**
   * \fn void elapse_us(void)
   * \brief Retrieve the number of microseconds since library initialisation or from the last call to \see reset_time()
   * \return The number of microseconds since library initialisation or from the last call to \see reset_time()
   */
  extern uint32_t elapse_us(void);

  /**
   * \fn uint32_t get_sys_clock_freq(void)
   * \brief Retrieve the current system clock frequency in Hz
   * \return The current system clock frequency in Hz
   */
  extern uint32_t get_sys_clock_freq(void);

  /**
   * \fn int64_t map(int64_t p_value, int64_t p_in_min, int64_t p_in_max, int64_t p_out_min, int64_t p_out_max)
   * \brief Re-maps a number from one range to another
   * \return The re-mapped value
   */
  extern int64_t map(int64_t p_value, int64_t p_in_min, int64_t p_in_max, int64_t p_out_min, int64_t p_out_max);

#ifdef __cplusplus
}
#endif
