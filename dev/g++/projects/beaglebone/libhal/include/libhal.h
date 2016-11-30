/**
 * @file    libhal.h
 * @brief   Main header file for the Hardware Abstract Layer library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once
#include <stdint.h>

#include "libhal_pin_names.h"

/** @def Handy defines
 */
#define    HAL_MODE_PINS           0
#define    HAL_MODE_GPIO           1
#define    HAL_MODE_GPIO_SYS       2
#define    HAL_MODE_PHYS           3
#define    HAL_MODE_PIFACE         4
#define    HAL_MODE_UNINITIALISED -1

/** @enum GPIO modes
 */
typedef enum {
  gpio_modes_digital_input,
  gpio_modes_digital_output,
  gpio_modes_adc_input,
  gpio_modes_pwm_output,
  gpio_modes_clock
} gpio_modes_t;

/** @enum Digital GPIO state
 */
typedef enum {
  digital_state_low,                /*!< Low digital GPIO level state (Vss) */
  digital_state_high,               /*!< High digital GPIO level state (Vcc) */
  digital_state_error
} digital_state_t;

/** @enum Pull-up/pull-down mode
 */
typedef enum {
  pud_off,                          /*!< Pull-up/pull-down unset */
  pud_down,                         /*!< Pull-down set */
  pud_up                            /*!< Pull-up set */
} pud_t;

// PWM

#define    HAL_PWM_MODE_MS          0
#define    HAL_PWM_MODE_BAL         1

/** @enum Interrupt levels
 */
typedef enum {
  isr_levels_setup,
  isr_levels_edge_falling,
  isr_levels_edge_rising,
  isr_levels_edge_both
} isr_levels_t;
// Threads

#define    HAL_THREAD(X)    void *X (void *p_void)

// Failure modes

#define    HAL_FATAL           (1==1)
#define    HAL_ALMOST          (1==2)

// Function prototypes
#ifdef __cplusplus
extern "C" {
#endif
  
  /**
   * @fn int32_t libhal_failure(const int32_t p_code, const char *p_message, ...)
   * @brief Display fatal error message, including error code
   * @param[in] p_code      Error code
   * @param[in] p_message   Error message to be displayed
   * @param[in] ...         Message argument (compliant with printf)
   * @return            0 on success, -1 otherwise
   */
  int32_t libhal_failure(const uint8_t p_code, const char *p_message, ...);
  /**
   * @fn int32_t  libhal_setup(void)
   * @brief Initialises the system into libhal Pin mode and uses the memory mapped hardware directly
   * @return 0 on success, -1 otherwise
   * @remark This function must be called once at the start of your program execution
   */
  extern int32_t libhal_setup(void);
  /**
   * @fn int32_t  libhal_setup_sys(void)
   * @brief Initialises the system into libhal Pin mode and uses the memory mapped hardware directly
   * @return 0 on success, -1 if the library was already initialised
   * @remark This function must be called once at the start of your program execution
   */
  extern int32_t libhal_setup_sys(void);
  /**
   * @fn int32_t  libhal_shutdown_sys(void)
   * @brief Uninitialize the Hardware Abstract Layer library
   * @remark This function shall be called once in the process
   * @return 0 on success, -1  if the library was already initialised
   */
  extern void libhal_shutdown_sys(void);  
  /**
   * @fn void pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode)
   * @brief Sets the mode of the specifed pin
   * @remark This function shall be called once in the process
   * @param[in] p_gpio      The GPIO identifier to be changed
   * @param[in] p_mode      The mode to be set
   * @return 0 on success, -1 otherwise
   */
  extern void pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode);
  /**
   * @fn void pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud)
   * @brief Control the internal pull-up/down resistors on a GPIO pin
   * @param[in] p_gpio      The GPIO identifier to be changed
   * @param[in] p_pud       The mode to be set
   * @return 0 on success, -1 otherwise
   * @todo To be implemented
   */
  extern int32_t pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud);
  /**
   * @fn void digital_read(const pin_name p_gpio)
   * @brief Read the value of a GPIO pin
   * @param[in] p_gpio      The GPIO identifier to be changed
   * @return The digital GPIO state on success, digital_state_error otherwise
   */
  extern digital_state_t digital_read(const pin_name p_gpio);
  /**
   * @fn void digital_read(const pin_name p_gpio)
   * @brief Write the value of a GPIO pin
   * @param[in] p_gpio      The GPIO identifier to be changed
   * @param[in] p_value     The value to write
   * @return The digital GPIO state on success, digital_state_error otherwise
   */
  extern void digital_write(const pin_name p_gpio, const digital_state_t p_value);
  /**
   * @fn int32_t pwm_write(const pin_name p_gpio, const uint32_t p_value)
   * @brief Set up a GPIO pin as output PWM
   * @param[in] p_gpio      The GPIO identifier to be changed
   * @param[in] p_value     The PWM durty cycle in %
   * @return 0 on success, -1 otherwise
   */
  extern int32_t pwm_write(const pin_name p_gpio, const uint32_t p_value);
  /**
   * @fn int32_t analog_read(const pin_name p_gpio)
   * @brief Read the analog value of the specified GPIO
   * @param p_gpio The analog input GPIO
   * @return The ADC value
   */
  extern float analog_read(const pin_name p_gpio);
  //  extern void analog_write(const pin_name p_gpio, int32_t p_value);
  /**
   * @fn void board_revision(void)
   * @brief Retrieve the number representing the hardware revision of the board
   * @return The revision board on success, -1 otherwise
   */
  extern int32_t  board_revision(void);
  
  extern void digital_write_byte(const uint8_t p_value);
  
  /**
   * @fn void pwm_set_mode(const uint8_t p_mode)
   * @brief  
   * @param[in] p_mode
   * @todo To be implemented
   */
  extern void pwm_set_mode(const uint8_t p_mode);
  /**
   * @fn int32_t pwm_set_range(const pin_name p_gpio, const uint32_t p_range)
   * @brief 
   * @param[in] p_gpio      The GPIO pwm pin identifier
   * @param[in] p_range     The range value to apply
   * @return 0 on success, -1 otherwise
   * @todo To be implemented
   */
  extern int32_t pwm_set_range(const pin_name p_gpio, const uint32_t p_range);
  /**
   * @fn int32_t pwm_set_clock(const pin_name p_gpio, const uint32_t p_divisor)
   * @brief Change the PWM clock
   * @param[in] p_gpio      The GPIO pwm pin identifier
   * @param[in] p_divisor   The time divisor value to apply
   * @return 0 on success, -1 otherwise
   * @todo To be implemented
   */
  extern int32_t pwm_set_clock(const pin_name p_gpio, const uint32_t p_divisor);
  /**
   * @fn int32_t wait_for_interrupt(const pin_name p_gpio, uint32_t p_timeout)
   * @brief create an interrupt handler that will do a callback to the user supplied function
   * @param[in] p_gpio
   * @param[in] p_timeout
   */
  extern int32_t wait_for_interrupt(const pin_name p_gpio, uint32_t p_timeout);
  /**
   * @fn int32_t libhal_isr(const pin_name p_gpio, int32_t p_mode, int32_t (*p_function)(void *))
   * @brief create an interrupt handler that will do a callback to the user supplied function
   * @param[in] p_gpio
   * @param[in] p_mode
   * @param[in] p_function
   */
  extern int32_t libhal_isr(const pin_name p_gpio, const isr_levels_t p_levels, int32_t (*p_function)(void *));
  /**
   * @fn void wait_ms(const uint32_t p_delay_ms)
   * @brief Wait for the specified delay
   * @param[in] p_delay The delay to wait in milliseconds
   */
  extern void wait_ms(const uint32_t p_delay_ms);
  /**
   * @fn void wait_us(const uint32_t p_delay_us)
   * @brief Wait for the specified delay in microseconds
   * @param[in] p_delay_us The delay to wait in microseconds
   * @todo To be verified with BBB
   */
  extern void wait_us(const uint32_t p_delay_us);
  /**
   * @fn void reset_time(void)
   * @brief Reset internal timer
   */
  extern void reset_time(void);
  /**
   * @fn void elapse_ms(void)
   * @brief Retrieve the number of milliseconds since library initialisation or from the last call to @see reset_time()
   * @return The number of milliseconds since library initialisatino or from the last call to @see reset_time()
   */
  extern uint32_t elapse_ms(void);
  /**
   * @fn void elapse_us(void)
   * @brief Retrieve the number of microseconds since library initialisation or from the last call to @see reset_time()
   * @return The number of microseconds since library initialisation or from the last call to @see reset_time()
   */
  extern uint32_t elapse_us(void);

#ifdef __cplusplus
}
#endif
