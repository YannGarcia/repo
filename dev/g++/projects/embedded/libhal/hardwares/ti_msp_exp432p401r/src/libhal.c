/*!
 * \file      libhal.c
 * \brief     Main implementation file for the Hardware Abstract Layer library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2016-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 * \see       MSP432® Peripheral Driver Library - USER’S GUIDE
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc14.h"

#include "libhal.h"

#ifndef TRUE
#define TRUE    (1==1)
#define FALSE   (1==2)
#endif

/* Number of pins */
#define MAX_GPIO_ID 128                                 /*!< Maximal GPIO index value */

/* Maximal simultaneous analog GPIOs */
#define MAX_ANALOG_GPIO 8                               /*< Maximal simultaneous analog GPIOs */

/** @enum gpioMode
 *  @brief GPIO access mode
 */
typedef enum {
  gpio_access_shared,                                    /*!< GPIO access is shared */
  gpio_access_greedy,                                    /*!< GPIO access is not shared */
  gpio_access_weak                                       /*!< ??? */
} gpio_access_t;

/** @enum gpio_types_t
 *  @brief GPIO type
 */
typedef enum {
  gpio_types_digital,                                  /*!< GPIO digital type */
  gpio_types_pwm,                                      /*!< GPIO PWM type */
  gpio_types_adc                                       /*!< GPIO analog/digital type */
} gpio_types_t;

typedef struct {
  int32_t (* callback_fn)(void *);                      /*!< Callback to be called on ISR. NOTE: Execution code shall be short as possible */
  void *callback_arg;
  int32_t ready;                                        /*!< Contains the pin value SysFS file descriptor, fd */
} gpio_callback_t;

/** @struct digital GPIO user interface
 *  @brief This structure describes the digital GPIO user interface
 */
typedef struct {
  gpio_callback_t callback;                             /*!< ISR callback */
  pud_t pud;                                            /*!< Pull-Up configuration */
} digital_context_t;

/** @struct ADC GPIO user interface
 *  @brief This structure describes the ADC GPIO user interface
 */
typedef struct {
  gpio_callback_t callback;                            /*!< ISR callback */
} adc_context_t;

/** @struct PWM user interface
 *  @brief This structure describes the PWM user interface
 */
typedef struct {
  float frequency;                                      /*!< The PWM signal frequency */
//  uint32_t divisor;                                     /*!< Divisor to apply for the PWM base frequency */
  float duty_cycle;                                     /*!< Duty cycle of the PWM signal (in ns) */
} pwm_context_t;

/** @struct GPIOs configuration
 *  @brief This structure describes the GPIOs configuration
 */
typedef struct {
  pin_name gpio;                                        /*!< Gpio port number */
  int32_t shared;                                       /*!< Gpio shared counter. Default: 0 (not shared) */
  gpio_types_t type;                                    /*!< Gpio type (digital, analog...) */
  union {
    digital_context_t digital;                          /*!< The digital context */
    pwm_context_t pwm;                                  /*!< The PWM context */
    adc_context_t adc;                                  /*!< The ADC context */
  } types;
} gpio_context;

typedef struct { // FIXME analog_gpios & num_configured_analog_gpios seems to be useless for analog_read
  uint32_t gpio_idx[MAX_ANALOG_GPIO];  /*!< The analog GPIOs list */
  uint8_t num_configured_analog_gpios;  /*!< The number of analog GPIOs used */
  bool initialised;                     /*!< Set to true if the ADC is initialised */
  uint16_t samples[MAX_ANALOG_GPIO];    // Sequence 3 has a FIFO depth of 1
                                        // 8: maximal number of channels on  SS0
} adc_configuration_;

static uint8_t is_initialised = FALSE;                  /*!< Indicate if the library was initialized by the process */
static uint32_t sys_clock = 0;                          /*!< Indicate the configured system clock frequency in Hz */
static uint32_t pwm_frequency = 0;                      /*!< Indicate the configured PWM clock frequency in Hz */
static gpio_context context_handles[MAX_GPIO_ID];       /*!< context_handles: Map a file descriptor from the /sys/class/xxx */
static adc_configuration_ adc_configuration;            /*!< adc configuration */
//static uint64_t time_ms, time_us;                     /*!< Time for easy calculations */

/**
 * @fn void initialise_time(void)
 * @brief Initialise internal time variables
 */
static void initialise_time(void);
/**
 * @fn int32_t create_new_context(const pin_name p_gpio, const gpio_access_t p_gpio_access, const gpio_types_t p_gpio_type)
 * @brief Create and initialise a new GPIO context
 */
static int32_t create_new_context(const pin_name p_gpio, const gpio_access_t p_gpio_access, const gpio_types_t p_gpio_type);
/**
 * @fn int32_t free_context(const pin_name p_gpio)
 * @brief Free a new GPIO context
 */
static int32_t free_context(const pin_name p_gpio);
//static void pwm_update(gpio_context *p_gpio);
static uint8_t gpio_to_index(const pin_name p_gpio);
static void enable_adc_periph(const pin_name p_gpio);
static void enable_adcs_periph(const pin_name * p_gpio, const uint32_t p_len);
//static uint32_t gpio_to_pwm_output(const pin_name p_gpio); // TODO Check if this function is really useful
//static uint32_t gpio_to_pwm_pinmap(const pin_name p_gpio); // TODO Check if this function is really useful
//static uint32_t gpio_to_pwm_module(const pin_name p_gpio); // TODO Check if this function is really useful
//static uint32_t gpio_to_pwm_enable(const pin_name p_gpio); // TODO Check if this function is really useful
static void insert_sort(uint32_t *p_gpio, const uint8_t p_len, const uint32_t p_value);

/**
 * @fn void initialise_gpios(const bool p_ethernet_mode, const bool p_usb_mode)
 * @brief Configure the device pins for the standard usages on the launchPad MSP432P401R
 * @see MSP432P4xx SimpleLink™ Microcontrollers (Rev. F) Clause 10
 */
static void initialise_gpios(const bool p_ethernet_mode, const bool p_usb_mode);

int32_t libhal_setup(void)
{
  return libhal_setup_sys();
}

int32_t pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode)
{
  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if (gpio_idx > MAX_GPIO_ID) {
    return -1;
  }

  //  if (context_handles[gpio_idx] == NULL) {
  if (context_handles[gpio_idx].gpio == NC) {
    switch (p_mode) {
    case gpio_modes_digital_input:
      /* No break; */
    case gpio_modes_digital_output:
      /* Allocation GPIO */
      if (create_new_context(p_gpio, gpio_access_weak, gpio_types_digital) < 0) {
        return -1;
      }
      break;
    case gpio_modes_pwm_output:
      /* Allocation GPIO */
      if (create_new_context(p_gpio, gpio_access_weak, gpio_types_pwm) < 0) {
        return -1;
      }
      break;
    case gpio_modes_adc_input:
      /* Allocation GPIO */
      if (create_new_context(p_gpio, gpio_access_weak, gpio_types_adc) < 0) {
        return -1;
      }
      break;
    case gpio_modes_clock:
      /* TODO To be implemented */
      break;
    } /* End of 'switch' statement */
  }

  /* Set the direction */
  switch (p_mode) {
  case gpio_modes_digital_input:
    /* Set gpio_modes_digital_input mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                                                   (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                                                   (uint_fast8_t)(p_gpio & 0xffff),                         /* Port number */
                                                   GPIO_PRIMARY_MODULE_FUNCTION
                                                  );
    MAP_GPIO_setAsInputPin(
                           (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                           (uint_fast8_t)(p_gpio & 0xffff)                          /* Port number */
                          );
    break;
  case gpio_modes_digital_output:
    /* Set gpio_modes_digital_outut mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                                                   (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16),  /* Port register */
                                                   (uint_fast8_t)(p_gpio & 0xffff),                          /* Port number */
                                                   GPIO_PRIMARY_MODULE_FUNCTION
                                                  );
    MAP_GPIO_setAsOutputPin(
                            (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16),  /* Port register */
                            (uint_fast8_t)(p_gpio & 0xffff)                           /* Port number */
                            );
    break;
  case gpio_modes_adc_input:
    enable_adc_periph(p_gpio);
    break;
  case gpio_modes_pwm_output:
    /* Configure the GPIO for PWM */
      MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(
                                                     (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16),  /* Port register */
                                                     (uint_fast8_t)(p_gpio & 0xffff),                          /* Port number */
                                                     GPIO_PRIMARY_MODULE_FUNCTION
                                                    );
      MAP_GPIO_setAsOutputPin(
                              (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16),  /* Port register */
                              (uint_fast8_t)(p_gpio & 0xffff)                           /* Port number */
                              );
    break;
  case gpio_modes_clock:
    /* TODO To be implemented */
    break;
  } /* End of 'switch' statement */

  return 0;
} /* End of function pin_mode */

int32_t pins_mode(const pin_name * p_gpios, const uint8_t p_len, const gpio_modes_t p_mode) {
  /* Sanity checks */
  if (p_len == 1) {
    return -1;
  }

  uint8_t gpio;
  for (gpio = 0; gpio < p_len; gpio++) {
    uint8_t gpio_idx = gpio_to_index(*(p_gpios + gpio));
    if (gpio_idx > MAX_GPIO_ID) {
      return -1;
    }

    if (context_handles[gpio_idx].gpio == NC) {
      switch (p_mode) {
      case gpio_modes_digital_input:
        /* No break; */
      case gpio_modes_digital_output:
        /* Allocation GPIO */
        if (create_new_context(*(p_gpios + gpio), gpio_access_weak, gpio_types_digital) < 0) {
          context_handles[gpio_idx].gpio = NC;
          return -1;
        }
        break;
      case gpio_modes_pwm_output:
        /* Allocation GPIO */
        if (create_new_context(*(p_gpios + gpio), gpio_access_weak, gpio_types_pwm) < 0) {
          context_handles[gpio_idx].gpio = NC;
          return -1;
        }
        break;
      case gpio_modes_adc_input:
        /* Allocation GPIO */
        if (create_new_context(*(p_gpios + gpio), gpio_access_weak, gpio_types_adc) < 0) {
          context_handles[gpio_idx].gpio = NC;
          return -1;
        }
        break;
      case gpio_modes_clock:
        /* TODO To be implemented */
        break;
      } /* End of 'switch' statement */
    }

    /* Set the direction */
    switch (p_mode) {
      case gpio_modes_digital_input:
        /* Set gpio_modes_digital_input mode */
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                                                       (uint_fast8_t)(((uint32_t)*(p_gpios + gpio) & 0xffffff00) >> 16), /* Port register */
                                                       (uint_fast16_t)((uint8_t)*(p_gpios + gpio) & 0xff),               /* Port number */
                                                       GPIO_PRIMARY_MODULE_FUNCTION
                                                      );
        MAP_GPIO_setAsInputPin(
                               (uint_fast8_t)(((uint32_t)*(p_gpios + gpio) & 0xffffff00) >> 16), /* Port register */
                               (uint_fast16_t)((uint8_t)*(p_gpios + gpio) & 0xff)                /* Port number */
                              );
        break;
      case gpio_modes_digital_output:
        /* Set gpio_modes_digital_outut mode */
        MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(
                                                        (uint_fast8_t)(((uint32_t)*(p_gpios + gpio) & 0xffffff00) >> 16), /* Port register */
                                                        (uint_fast16_t)((uint8_t)*(p_gpios + gpio) & 0xff),               /* Port number */
                                                        GPIO_PRIMARY_MODULE_FUNCTION
                                                       );
        MAP_GPIO_setAsOutputPin(
                                (uint_fast8_t)(((uint32_t)*(p_gpios + gpio) & 0xffffff00) >> 16), /* Port register */
                                (uint_fast16_t)((uint8_t)*(p_gpios + gpio) & 0xff)                /* Port number */
                               );
        break;
      case gpio_modes_adc_input:
        // Processed outside
        break;
      case gpio_modes_pwm_output:
        // Configure the GPIO for PWM
//        GPIOPinConfigure(gpio_to_pwm_pinmap(*(p_gpios + gpio)));
//        GPIOPinTypePWM(
//                       *(p_gpios + gpio) & 0xffffff00, /* Port register */
//                       *(p_gpios + gpio) & 0xff      /* Port number */
//                       );
//        // Configure the PWM generator for count down mode with immediate updates to the parameters
//        PWMGenConfigure(PWM0_BASE, gpio_to_pwm_module(*(p_gpios + gpio)), PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
//        // Disable the PWM dead band output
//        PWMDeadBandDisable(PWM0_BASE, gpio_to_pwm_module(*(p_gpios + gpio)));
//        // Setup PWM module
//        pwm_update(&context_handles[gpio_idx]);
        break;
      case gpio_modes_clock:
        /* TODO To be implemented */
        break;
    } /* End of 'switch' statement */
  } /* End of 'for' statement */
  if (p_mode == gpio_modes_adc_input) {
    /* Sanity check */
    if (p_len >= MAX_ANALOG_GPIO) {
      return -1;
    }
    enable_adcs_periph(p_gpios, p_len);
  }

  return 0;
} /* End of function pins_mode */

int32_t pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud) {
  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if ((gpio_idx > MAX_GPIO_ID) || (context_handles[gpio_idx].gpio == NC)) {
    return -1;
  }

  switch (p_pud) {
  case pud_up:
    context_handles[gpio_idx].types.digital.pud = pud_up;
    MAP_GPIO_setAsInputPinWithPullUpResistor(
                                             (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                                             (uint_fast16_t)((uint8_t)p_gpio & 0xffff)                /* Port number */
                                            );

    break;
  case pud_down:
    context_handles[gpio_idx].types.digital.pud = pud_down;
    MAP_GPIO_setAsInputPinWithPullDownResistor(
                                               (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                                               (uint_fast16_t)((uint8_t)p_gpio & 0xffff)                /* Port number */
                                              );
    break;
  }

  return 0;
}

digital_state_t digital_read(const pin_name p_gpio) {
  int32_t c;

  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if (gpio_idx > MAX_GPIO_ID) {
    return digital_state_error;
  }

  /* On-Board Pin */
  if (context_handles[gpio_idx].gpio == NC) {
    /* Allocation GPIO */
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_digital) < 0) {
      return digital_state_low;
    }
    /* Set direction in */
    pin_mode(p_gpio, gpio_modes_digital_input);
  }
  /* Read value */
  c = MAP_GPIO_getInputPinValue(
                                (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                                (uint_fast16_t)((uint8_t)p_gpio & 0xffff)                /* Port number */
                               );
  return (c == 1) ? digital_state_high : digital_state_low; /* Pull-Up/Down interpretation is up to the application */
}

void digital_write(const pin_name p_gpio, const digital_state_t p_value) {
  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if (gpio_idx > MAX_GPIO_ID) {
    fprintf(stderr, "digitalWrite: Wrong parameter\n");
    return;
  }

  if (context_handles[gpio_idx].gpio == NC) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_digital) < 0) {
      return;
    }
    // Set direction out
    pin_mode(p_gpio, gpio_modes_digital_output);
  }

  // Write value
  if (p_value == digital_state_low) {
    MAP_GPIO_setOutputLowOnPin(
                               (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                               (uint_fast16_t)((uint8_t)p_gpio & 0xffff)                /* Port number */
                              );
  } else {
    MAP_GPIO_setOutputHighOnPin(
                                (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                                (uint_fast16_t)((uint8_t)p_gpio & 0xffff)                /* Port number */
                               );
  }

  return;
}

void digital_toggle(const pin_name p_gpio) {
  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if (gpio_idx > MAX_GPIO_ID) {
    fprintf(stderr, "digital_toggle: Wrong parameter\n");
    return;
  }

  //  if (context_handles[gpio_idx] == NULL) {
  if (context_handles[gpio_idx].gpio == NC) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_digital) < 0) {
      return;
    }
    // Set direction out
    pin_mode(p_gpio, gpio_modes_digital_output);
  }

  // Toggle low -> high -> low
  MAP_GPIO_toggleOutputOnPin(
                             (uint_fast8_t)(((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                             (uint_fast16_t)((uint8_t)p_gpio & 0xffff)                /* Port number */
                            );
  return;
}

int32_t pwm_start(const pin_name p_gpio, const float p_frequency, const float p_duty_cycle) {
  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if (gpio_idx > MAX_GPIO_ID) {
    fprintf(stderr, "digital_toggle: Wrong parameter\n");
    return -1;
  }

  //  if (context_handles[gpio_idx] == NULL) {
  if (context_handles[gpio_idx].gpio == NC) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_pwm) < 0) {
    return -1;
    }
    // Set direction out
    pin_mode(p_gpio, gpio_modes_pwm_output);
  }
  /* Configure Timer_A PWM Configuration Parameter */
  volatile uint_fast16_t divider = 1;
  volatile uint32_t ticks = (uint32_t)((float)pwm_frequency / p_frequency);
  while (ticks > 65536) {
      divider *= 2;
      ticks = (uint32_t)((float)pwm_frequency / p_frequency / divider);
  } // End of 'while' statement
  if (divider > 16) { // TODO To be refined
    context_handles[gpio_idx].gpio = NC;
    return -1;
  }
  context_handles[gpio_idx].types.pwm.frequency = p_frequency;
  context_handles[gpio_idx].types.pwm.duty_cycle = p_duty_cycle;
  volatile uint_fast16_t duty_cycle = (uint_fast16_t)((float)ticks * p_duty_cycle / 100.0);
  volatile const Timer_A_PWMConfig pwm_config = {
                                                TIMER_A_CLOCKSOURCE_ACLK,
                                                divider,
                                                (uint_fast16_t)ticks,
                                                TIMER_A_CAPTURECOMPARE_REGISTER_1,
                                                TIMER_A_OUTPUTMODE_RESET_SET,
                                                duty_cycle
  };
  MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_config);
  return 0;
}

int32_t pwm_stop(const pin_name p_gpio) {
  return -1;
}
//int32_t pwm_write(const pin_name p_gpio, const uint32_t p_value) {
//  /* Sanity check */
//  uint8_t gpio_idx = gpio_to_index(p_gpio);
//  if (gpio_idx > MAX_GPIO_ID) {
//    // FIXME Add default behavior
//    return -1;
//  }
//
//  //  if (context_handles[gpio_idx] == NULL) {
//  if (context_handles[gpio_idx].gpio == NC) {
//    // Allocation GPIO
//    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_pwm) < 0) {
//      return -1;
//    }
//    // Setup PWM module
//    pin_mode(p_gpio, gpio_modes_pwm_output);
//  }
//
//  // Update duty cycle
//  if (p_value == 0) { // Stop PWM
//    context_handles[gpio_idx].types.pwm.duty_cycle = 0.0;
//    PWMOutputState(PWM0_BASE, gpio_to_pwm_enable(p_gpio), false);
//  } else {
//    context_handles[gpio_idx].types.pwm.duty_cycle = (float)p_value / 100.0;
//    pwm_update(&context_handles[gpio_idx]);
//    // Start the timers
//    PWMGenEnable(PWM0_BASE, gpio_to_pwm_module(p_gpio));
//    // Enable the outputs
//    PWMOutputState(PWM0_BASE, gpio_to_pwm_enable(p_gpio), true);
//  }
//
//  return 0;
//} // End of function pwm_write
//
//void pwm_set_mode(const uint8_t p_mode) {
  /* Nothing to do */
//} // End of function pwm_set_mode
//
//int32_t pwm_set_range(const pin_name p_gpio, const float p_frequency) {
//  /* Sanity check */
//  uint8_t gpio_idx = gpio_to_index(p_gpio);
//  if ((gpio_idx > MAX_GPIO_ID) || (context_handles[gpio_idx].gpio == NC)) {
//  // FIXME Add default behavior
//  return -1;
//  }
//
//  context_handles[gpio_idx].types.pwm.frequency = p_frequency;
//  pwm_update(&context_handles[gpio_idx]);
//
//  return 0;
//} // End of function pwm_set_range
//
//int32_t pwm_set_clock(const pin_name p_gpio, const uint32_t p_divisor) {
//  /* Sanity check */
//  uint8_t gpio_idx = gpio_to_index(p_gpio);
//  if ((gpio_idx > MAX_GPIO_ID) || (context_handles[gpio_idx].gpio == NC)) {
//  // FIXME Add default behavior
//  return -1;
//  }
//
//  context_handles[gpio_idx].types.pwm.divisor = p_divisor;
//  pwm_update(&context_handles[gpio_idx]);
//
//  return 0;
//} // End of function pwm_set_clock

float analog_read(const pin_name p_gpio) {
  /* Sanity checks */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if (gpio_idx > MAX_GPIO_ID) {
    fprintf(stderr, "analog_read: Wrong parameter\n");
    return (float)LONG_MAX;
  }

  if (context_handles[gpio_idx].gpio == NC) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_adc) < 0) {
      return (float)LONG_MAX;
    }
    // Set GPIO pin mode
    pin_mode(p_gpio, gpio_modes_adc_input);
  }
  /* Configure analog input */
  uint32_t memory_select = 0, interrupt_flag = 0, channel = 0;
  if ((((uint32_t)p_gpio & 0xffffffff00) >> 16) == GPIO_PORT_P4) {
    switch (p_gpio & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
      case GPIO_PIN0:
        memory_select = ADC_MEM13;
        interrupt_flag = ADC_INT13;
        channel = ADC_INPUT_A13;
        break;
      case GPIO_PIN1:
        memory_select = ADC_MEM12;
        interrupt_flag = ADC_INT12;
        channel = ADC_INPUT_A12;
        break;
      case GPIO_PIN2:
        memory_select = ADC_MEM11;
        interrupt_flag = ADC_INT11;
        channel = ADC_INPUT_A11;
        break;
      case GPIO_PIN3:
        memory_select = ADC_MEM10;
        interrupt_flag = ADC_INT10;
        channel = ADC_INPUT_A10;
        break;
      case GPIO_PIN4:
        memory_select = ADC_MEM9;
        interrupt_flag = ADC_INT9;
        channel = ADC_INPUT_A9;
        break;
      case GPIO_PIN5:
        memory_select = ADC_MEM8;
        interrupt_flag = ADC_INT8;
        channel = ADC_INPUT_A8;
        break;
      case GPIO_PIN6:
        memory_select = ADC_MEM7;
        interrupt_flag = ADC_INT7;
        channel = ADC_INPUT_A7;
        break;
      case GPIO_PIN7:
        memory_select = ADC_MEM6;
        interrupt_flag = ADC_INT6;
        channel = ADC_INPUT_A6;
        break;
    } /* End of 'switch' statement */
  } else if ((((uint32_t)p_gpio & 0xffffffff00) >> 16) == GPIO_PORT_P5) {
    switch (p_gpio & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
      case GPIO_PIN0:
        memory_select = ADC_MEM5;
        interrupt_flag = ADC_INT5;
        channel = ADC_INPUT_A5;
        break;
      case GPIO_PIN1:
        memory_select = ADC_MEM4;
        interrupt_flag = ADC_INT4;
        channel = ADC_INPUT_A4;
        break;
      case GPIO_PIN2:
        memory_select = ADC_MEM3;
        interrupt_flag = ADC_INT3;
        channel = ADC_INPUT_A3;
        break;
      case GPIO_PIN3:
        memory_select = ADC_MEM2;
        interrupt_flag = ADC_INT2;
        channel = ADC_INPUT_A2;
        break;
      case GPIO_PIN4:
        memory_select = ADC_MEM1;
        interrupt_flag = ADC_INT1;
        channel = ADC_INPUT_A1;
        break;
      case GPIO_PIN5:
        memory_select = ADC_MEM0;
        interrupt_flag = ADC_INT0;
        channel = ADC_INPUT_A0;
        break;
    } /* End of 'switch' statement */
  } else if ((((uint32_t)p_gpio & 0xffffffff00) >> 16) == GPIO_PORT_P6) {
    switch (p_gpio & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
      case GPIO_PIN0:
        memory_select = ADC_MEM15;
        interrupt_flag = ADC_INT15;
        channel = ADC_INPUT_A15;
        break;
      case GPIO_PIN1:
        memory_select = ADC_MEM14;
        interrupt_flag = ADC_INT14;
        channel = ADC_INPUT_A14;
        break;
    } /* End of 'switch' statement */
  }
  MAP_ADC14_configureSingleSampleMode(memory_select, false);
  MAP_ADC14_configureConversionMemory(memory_select, ADC_VREFPOS_AVCC_VREFNEG_VSS, channel, false);
  /* Configuring Sample Timer */
  MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
  /* Enabling/Toggling Conversion */
  MAP_ADC14_enableConversion();
  MAP_ADC14_toggleConversionTrigger();
  /* Enable interrupts */
  MAP_ADC14_enableInterrupt(interrupt_flag);
  // Wait result
  while(!(interrupt_flag & MAP_ADC14_getEnabledInterruptStatus()));
  /* Read ADC Value. */
  adc_configuration.samples[0] = MAP_ADC14_getResult(memory_select);
  /* Disable conversion */
  MAP_ADC14_disableConversion();
  /* Disable interrupts */
  MAP_ADC14_disableInterrupt(interrupt_flag);
  /* Clear interrupts */
  MAP_ADC14_clearInterruptFlag(MAP_ADC14_getEnabledInterruptStatus());
  /* Disable Sample Timer */
  MAP_ADC14_disableSampleTimer();
  /* return value; */
  float value = (float)(adc_configuration.samples[0] & 0xffff) * 3.3f / 16384.0f; // See MSP432P4xx SimpleLink™ Microcontrollers Clause 20.2.1 14-Bit ADC Core
  return (float)value;
} // End of function analogRead

int32_t  analog_multiple_read(const pin_name * p_gpios, const uint8_t p_len, float *p_values) {
  /* Sanity check */
  if (p_len != adc_configuration.num_configured_analog_gpios) {
    return -1;
  }
  uint8_t gpio = 0;
  for ( ; gpio < adc_configuration.num_configured_analog_gpios; gpio++) {
    if ((((uint32_t)*(p_gpios + gpio) & 0xffffffff00) >> 16) == GPIO_PORT_P5) { // TODO Common code with enable_adc_periph, to be optimised
      switch (*(p_gpios + gpio) & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
        case GPIO_PIN0:
          MAP_ADC14_configureConversionMemory(ADC_MEM5, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A5, false);
          break;
        case GPIO_PIN1:
          MAP_ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A4, false);
          break;
        case GPIO_PIN2:
          MAP_ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A3, false);
          break;
        case GPIO_PIN3:
          MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A2, false);
          break;
        case GPIO_PIN4:
          MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1, false);
          break;
        case GPIO_PIN5:
          MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);
          break;
      } /* End of 'switch' statement */
    } else if ((((uint32_t)*(p_gpios + gpio) & 0xffffffff00) >> 16) == GPIO_PORT_P4) {
      switch (*(p_gpios + gpio) & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
        case GPIO_PIN0:
          MAP_ADC14_configureConversionMemory(ADC_MEM13, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A13, false);
          break;
        case GPIO_PIN1:
          MAP_ADC14_configureConversionMemory(ADC_MEM12, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A12, false);
        break;
        case GPIO_PIN2:
          MAP_ADC14_configureConversionMemory(ADC_MEM11, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, false);
          break;
        case GPIO_PIN3:
          MAP_ADC14_configureConversionMemory(ADC_MEM10, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A10, false);
          break;
        case GPIO_PIN4:
          MAP_ADC14_configureConversionMemory(ADC_MEM9, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, false);
          break;
        case GPIO_PIN6:
          MAP_ADC14_configureConversionMemory(ADC_MEM7, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7, false);
          break;
        case GPIO_PIN7:
          MAP_ADC14_configureConversionMemory(ADC_MEM6, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A6, false);
          break;
      } /* End of 'switch' statement */
    } else if ((((uint32_t)*(p_gpios + gpio) & 0xffffffff00) >> 16) == GPIO_PORT_P6) {
      switch (*(p_gpios + gpio) & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
        case GPIO_PIN0:
          MAP_ADC14_configureConversionMemory(ADC_MEM15, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, false);
          break;
        case GPIO_PIN1:
          MAP_ADC14_configureConversionMemory(ADC_MEM14, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A14, false);
          break;
      } /* End of 'switch' statement */
    }
  } /* End of 'for' statement */
  /* Configuring multiple ADCs */
  MAP_ADC14_configureMultiSequenceMode(adc_configuration.gpio_idx[0], adc_configuration.gpio_idx[adc_configuration.num_configured_analog_gpios - 1], false);
  /* Configuring Sample Timer */
  MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
  /* Enable interrupts */
  MAP_ADC14_enableInterrupt(adc_configuration.gpio_idx[adc_configuration.num_configured_analog_gpios - 1]);
  /* Enabling/Toggling Conversion */
  MAP_ADC14_enableConversion();
  MAP_ADC14_toggleConversionTrigger();
  /* Wait result */
  while((adc_configuration.gpio_idx[adc_configuration.num_configured_analog_gpios - 1] & MAP_ADC14_getEnabledInterruptStatus()) == 0x00);
  /* Read ADC Value. */
  MAP_ADC14_getMultiSequenceResult(adc_configuration.samples);
  /* Disable conversion */
  MAP_ADC14_disableConversion();
  /* Disable interrupts */
  MAP_ADC14_disableInterrupt(adc_configuration.gpio_idx[adc_configuration.num_configured_analog_gpios - 1]);
  /* Clear interrupts */
  MAP_ADC14_clearInterruptFlag(MAP_ADC14_getEnabledInterruptStatus());
  /* Disable Sample Timer */
  MAP_ADC14_disableSampleTimer();
  /* convert values */
  uint8_t i;
  for (i = 0; i < adc_configuration.num_configured_analog_gpios; i++) {
    if (0x8000 & adc_configuration.samples[i]) {
      *(p_values + i) = (float)((((adc_configuration.samples[i] >> 2) | 0xFFFFC000) * 3.3f) / 16384.0f);
    } else {
      *(p_values + i) = (float)((adc_configuration.samples[i] >> 2) * 3.3f) / 16384.0f;
    }
  } /* End of 'for' statement */

  return 0;
} // End of function analogRead

void wait_ms(const uint32_t p_delay_ms)
{
  uint32_t ii;
  for(ii=0;ii<p_delay_ms * 10000;ii++);
//  SysCtlDelay(p_delay_ms * (sys_clock / 3000));
}

void wait_us(const uint32_t p_delay_us) {
//  SysCtlDelay(p_delay_us * ((sys_clock / 3000) / 1000));
}

void reset_time(void)
{
  //  struct timeval tv;
  //
  //  gettimeofday(&tv, NULL);
  //  time_ms = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  //  time_us = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec);
}

uint32_t elapse_ms(void) {
  //  struct timeval tv ;
  //  uint64_t now ;
  //
  //  gettimeofday (&tv, NULL);
  //  now = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  //
  //  return (uint32_t)(now - time_ms);
  return 0;
}

uint32_t elapse_us(void) {
  //  struct timeval tv;
  //  uint64_t now;
  //
  //  gettimeofday (&tv, NULL);
  //  now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec ;
  //
  //  return (uint32_t)(now - time_us);
  return 0;
}

uint32_t get_sys_clock_freq(void) {
  return sys_clock;
}

int64_t map(int64_t p_value, int64_t p_in_min, int64_t p_in_max, int64_t p_out_min, int64_t p_out_max) {
  return (p_value - p_in_min) * (p_out_max - p_out_min) / (p_in_max - p_in_min) + p_out_min;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal C functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @fn int32_t board_revision(void)
 * @brief Retrieve the hardware revision of the board
 *
 * @param p_code      Error code
 * @param p_message   Error message to be displayed
 * @param ...         Message argument (compliant with printf)
 * @return            The hardware revision of the board
 */
int32_t board_revision(void) {
  int32_t board_revision = 1;

  return board_revision;
}

int32_t libhal_setup_sys(void) {
  int32_t revision;

  /* Sanity checks */
  if (is_initialised == TRUE) {
    return -1;
  }

  /* Enable processor interrupts */
  MAP_Interrupt_disableMaster();

  /* Halting WDT and disabling master interrupts */
  MAP_WDT_A_holdTimer();
  /* Set the core voltage level to VCORE1 */
  MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
  /* Set 2 flash wait states for Flash bank 0 and 1*/
  MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
  MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
  /* Initialises Clock System */
  MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
  MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);   /* Master clock set to internal 48MHz */
  MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); /* Subsystem master clock set to internal 48MHz */
  MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);  /* Low-speed subsystem master clock set to internal 48MHz */
  MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);  /* Auxiliary clock A set to internal 128kHz, used by PWM */
  MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);  /* Auxiliary clock B set to internal 32.768kHz */

  sys_clock = CS_48MHZ;
  pwm_frequency = 128e3;

  /* Enabling the FPU for floating point operation */
  MAP_FPU_enableModule();
  MAP_FPU_enableLazyStacking();

  /* Setting reference voltage to 2.5  and enabling reference */
  MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
  MAP_REF_A_enableReferenceVoltage();

  // Configure table according to board version
  revision = board_revision();
  if (revision == 1) {
  } // else, handle errors

  initialise_time();

  // Initialise context_handles table
  memset((void *)&context_handles, 0xff, sizeof(gpio_context) * MAX_GPIO_ID);

  // Initialise adc_configuration table
  memset((void *)&adc_configuration, 0x00, sizeof(adc_configuration));

  // Initialise interrupt handler
  /* memset((void *)&isr_fds, 0x00, sizeof(isr_fds)); */
  /* isr_fds_num = 0; */

  initialise_gpios(false, false);

  is_initialised = TRUE;

  /* Enabling the FPU for floating point operation */
  MAP_FPU_enableModule();
  MAP_FPU_enableLazyStacking();

  MAP_Interrupt_enableMaster();
  MAP_Interrupt_enableSleepOnIsrExit();

  return 0;
}

void libhal_shutdown_sys(void) {
  int32_t gpio_idx = 0;

  for(gpio_idx = 0; gpio_idx < MAX_GPIO_ID; gpio_idx++) {
    //  if (context_handles[gpio_idx] != NULL) {
    if (context_handles[gpio_idx].gpio == NC) {
      //      free_context(context_handles[gpio_idx]->gpio);
      free_context(context_handles[gpio_idx].gpio);
    }
  } /* End of 'for' statement */

  return;
}

int32_t libhal_failure(const uint8_t p_code, const char *p_message, ...) {
  va_list argp ;
  char * buffer = (char *)malloc(1024);

  va_start (argp, p_message);
  vsnprintf (buffer, 1023, p_message, argp);
  va_end (argp);
  fprintf (stderr, "%s", buffer);
  free(buffer);

  exit (EXIT_FAILURE);
  return 0 ;
}

int32_t create_new_context(const pin_name p_gpio, const gpio_access_t p_gpio_access, const gpio_types_t p_gpio_type) {
  gpio_context *gpio = NULL;

  /* Sanity checks */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if (gpio_idx > MAX_GPIO_ID) {
    return -1;
  }

  //  if (context_handles[gpio_idx] == NULL) {
  if (context_handles[gpio_idx].gpio == NC) {
    // Allocate value
    //    gpio = (gpio_context *)malloc(sizeof(gpio_context));
    gpio = &context_handles[gpio_idx];
    gpio->gpio = p_gpio;
    gpio->shared = 0;
    // Export it
    switch (p_gpio_type) {
    case gpio_types_digital:
      gpio->type = gpio_types_digital;
      gpio->types.digital.pud = pud_off;
      gpio->types.digital.callback.callback_fn = NULL;
      gpio->types.digital.callback.callback_arg = NULL;
      gpio->types.digital.callback.ready = 0xff;
      break;
    case gpio_types_pwm:
      gpio->type = gpio_types_pwm;
//      gpio->types.pwm.divisor = 64;        /* PWM frequency base = System frequency / 64 */
      gpio->types.pwm.duty_cycle = 0.5;    /* 50% */
      gpio->types.pwm.frequency = 25000.0; /* 25KHz */
      break;
    case gpio_types_adc:
      gpio->type = gpio_types_adc;
      break;
    } // End of 'switch' mode
    //    context_handles[gpio_idx] = gpio;
  } else if (p_gpio_access == gpio_access_weak) {
    free(gpio);
    return -1; // TODO Error
  } else {
    //    gpio = context_handles[gpio_idx];
    gpio = &context_handles[gpio_idx];
    if (p_gpio_access == gpio_access_shared) {
      gpio->shared += 1;
    }
  }

  return 0;
}

int32_t free_context(const pin_name p_gpio) {
  //  gpio_context * gpio;

  /* Sanity checks */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if ((gpio_idx > MAX_GPIO_ID) || (context_handles[gpio_idx].gpio == NC/*context_handles[gpio_idx] == NULL*/)) {
    return -1;
  }
  context_handles[gpio_idx].gpio = NC;
  //  gpio = context_handles[gpio_idx];
  //  if (gpio->shared == 0) { // Free it
  //    switch (gpio->type) {
  //    case gpio_types_digital:
  //      if (gpio->types.digital.callback.ready != 0xff) {
  //  // TOTO
  //  gpio->types.digital.callback.ready = 0xff;
  //      }
  //      break;
  //    case gpio_types_pwm:
  //      // TODO
  //      break;
  //    case gpio_types_adc:
  //      // TODO
  //      break;
  //      // FIXME TO be done
  //      break;
  //    } /* End of 'switch' statement */
  //    // Free the GPIO descriptor
  //    free((void *)gpio);
  //    context_handles[gpio_idx] = NULL;
  //  } else {
  //    gpio->shared -= 1;
  //  }

  return 0;
}

//void pwm_update(gpio_context *p_gpio) {
//  uint32_t pwm_clock = (uint32_t)(sys_clock / p_gpio->types.pwm.divisor);
//  uint32_t pwm_period = (uint32_t)(pwm_clock / p_gpio->types.pwm.frequency) - 1;
//  uint32_t pwm_duty = (uint32_t)(pwm_period * p_gpio->types.pwm.duty_cycle);
//  uint32_t divisor = PWM_SYSCLK_DIV_1;
//  switch (p_gpio->types.pwm.divisor) {
//  case 2:
//    divisor = PWM_SYSCLK_DIV_2;
//    break;
//  case 4:
//    divisor = PWM_SYSCLK_DIV_4;
//    break;
//  case 8:
//    divisor = PWM_SYSCLK_DIV_8;
//    break;
//  case 16:
//    divisor = PWM_SYSCLK_DIV_16;
//    break;
//  case 32:
//    divisor = PWM_SYSCLK_DIV_32;
//    break;
//  case 64:
//    divisor = PWM_SYSCLK_DIV_64;
//    break;
//  } /* End of 'switch' statement */
//  PWMClockSet(PWM0_BASE, divisor);
//  // Set the period
//  PWMGenPeriodSet(PWM0_BASE, gpio_to_pwm_module(p_gpio->gpio), pwm_period);
//  // Set the pulse width of PWM0 for a 25% duty cycle.
//  PWMPulseWidthSet(PWM0_BASE, gpio_to_pwm_output(p_gpio->gpio), pwm_duty);
//} // End of function pwm_update

static void initialise_time(void)
{
  //  struct timeval tv;
  //
  //  gettimeofday(&tv, NULL);
  //  time_ms = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  //  time_us = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec);
}

uint8_t gpio_to_index(const pin_name p_gpio)
{
  uint8_t index_base = (uint8_t)(
                                 ((((uint32_t)p_gpio & 0xffffffff00) >> 16) - GPIO_PORT_P1)
                                );

  uint8_t port = p_gpio & 0xffff; // FIXME To be checked
  switch (port) {
  case GPIO_PIN0:
    port = index_base;
    break;
  case GPIO_PIN1:
    port = index_base + 1;
    break;
  case GPIO_PIN2:
    port = index_base + 2;
    break;
  case GPIO_PIN3:
    port = index_base + 3;
    break;
  case GPIO_PIN4:
    port = index_base + 4;
    break;
  case GPIO_PIN5:
    port = index_base + 5;
    break;
  case GPIO_PIN6:
    port = index_base + 6;
    break;
  case GPIO_PIN7:
    port = index_base + 7;
    break;
  case GPIO_PIN10:
    port = index_base + 10;
    break;
  case GPIO_PIN11:
    port = index_base + 11;
    break;
  case GPIO_PIN12:
    port = index_base + 12;
    break;
  case GPIO_PIN13:
    port = index_base + 13;
    break;
  case GPIO_PIN14:
    port = index_base + 14;
    break;
  case GPIO_PIN15:
    port = index_base + 15;
    break;
  } /* End of 'switch' statement */

  return port;
}

void initialise_gpios(const bool p_ethernet_mode, const bool p_usb_mode) {

  /* P1.1 and P1.4 are on board Switch 1 and 2 */
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
  MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4); // SLAU597B–March 2015–Revised July 2016 - Figure 44. Schematics (2 of 6)

  /* P1.0, P2.0, P2.1 and P2.2 are on board led and RGB leds */
  MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
  MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
  /* Default the LEDs to OFF. */
  MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
  MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
}

void enable_adc_periph(const pin_name p_gpio) {
  /* Sanity checks not required here */

  switch ((((uint32_t)p_gpio & 0xffffffff00) >> 16)) {
    case GPIO_PORT_P4:
    case GPIO_PORT_P5:
    case GPIO_PORT_P6:
      if (!adc_configuration.initialised) {
        adc_configuration.initialised = true;
        MAP_ADC14_enableModule();                                                       /* Enable ADC peripheral */
        MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0); /* Initializing ADC module with MCLK / 1 / 1 */
        MAP_ADC14_setResolution(ADC_14BIT);
      }
      break;
  } /* End of 'switch' statement */
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                                                 (((uint32_t)p_gpio & 0xffffffff00) >> 16), /* Port register */
                                                 p_gpio & 0xffff,                           /* Port number */
                                                 GPIO_TERTIARY_MODULE_FUNCTION
                                                );
  /* Clear the interrupt status flag.  This is done to make sure the interrupt flag is cleared before we sample. */
  MAP_ADC14_clearInterruptFlag(MAP_ADC14_getEnabledInterruptStatus());
}

void enable_adcs_periph(const pin_name * p_gpios, const uint32_t p_len) {
  /* Sanity checks not required here */

  // Setup ADC peripheral
  if (!adc_configuration.initialised) {
    adc_configuration.initialised = true;
    MAP_ADC14_enableModule();                                                       /* Enable ADC peripheral */
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, 0); /* Initializing ADC module with MCLK / 1 / 4 */
    MAP_ADC14_setResolution(ADC_14BIT);
  }
  // Reset array
  uint8_t gpio;
  for(gpio = 0; gpio < MAX_ANALOG_GPIO; gpio++) adc_configuration.gpio_idx[gpio] = 0xFFFFFFFF;
  // Setup the channels
  adc_configuration.num_configured_analog_gpios = p_len;
  for (gpio = 0; gpio < adc_configuration.num_configured_analog_gpios; gpio++) {
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                                                   (((uint32_t)*(p_gpios + gpio) & 0xffffffff00) >> 16), /* Port register */
                                                   *(p_gpios + gpio) & 0xffff,                                      /* Port number */
                                                   GPIO_TERTIARY_MODULE_FUNCTION
                                                  );
    if ((((uint32_t)*(p_gpios + gpio) & 0xffffffff00) >> 16) == GPIO_PORT_P5) { // TODO Common code with enable_adc_periph, to be optimised
      switch (*(p_gpios + gpio) & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
        case GPIO_PIN0:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT5);
          MAP_ADC14_configureConversionMemory(ADC_MEM5, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A5, false);
          break;
        case GPIO_PIN1:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT4);
          MAP_ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A4, false);
          break;
        case GPIO_PIN2:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT3);
          MAP_ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A3, false);
          break;
        case GPIO_PIN3:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT2);
          MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A2, false);
          break;
        case GPIO_PIN4:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT1);
          MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1, false);
          break;
        case GPIO_PIN5:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT0);
          MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);
          break;
      } /* End of 'switch' statement */
    } else if ((((uint32_t)*(p_gpios + gpio) & 0xffffffff00) >> 16) == GPIO_PORT_P4) {
      switch (*(p_gpios + gpio) & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
        case GPIO_PIN0:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT13);
          MAP_ADC14_configureConversionMemory(ADC_MEM13, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A13, false);
          break;
        case GPIO_PIN1:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT12);
          MAP_ADC14_configureConversionMemory(ADC_MEM12, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A12, false);
        break;
        case GPIO_PIN2:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT11);
          MAP_ADC14_configureConversionMemory(ADC_MEM11, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, false);
          break;
        case GPIO_PIN3:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT10);
          MAP_ADC14_configureConversionMemory(ADC_MEM10, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A10, false);
          break;
        case GPIO_PIN4:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT9);
          MAP_ADC14_configureConversionMemory(ADC_MEM9, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, false);
          break;
        case GPIO_PIN6:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT6);
          MAP_ADC14_configureConversionMemory(ADC_MEM7, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7, false);
          break;
        case GPIO_PIN7:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT6);
          MAP_ADC14_configureConversionMemory(ADC_MEM6, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A6, false);
          break;
      } /* End of 'switch' statement */
    } else if ((((uint32_t)*(p_gpios + gpio) & 0xffffffff00) >> 16) == GPIO_PORT_P6) {
      switch (*(p_gpios + gpio) & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
        case GPIO_PIN0:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT15);
          MAP_ADC14_configureConversionMemory(ADC_MEM15, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, false);
          break;
        case GPIO_PIN1:
          insert_sort(adc_configuration.gpio_idx, MAX_ANALOG_GPIO, ADC_INT14);
          MAP_ADC14_configureConversionMemory(ADC_MEM14, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A14, false);
          break;
      } /* End of 'switch' statement */
    }
  } /* End of 'for' statement */
//  /* Configuring multiple ADCs */
//  MAP_ADC14_configureMultiSequenceMode(adc_configuration.gpio_idx[0], adc_configuration.gpio_idx[adc_configuration.num_configured_analog_gpios - 1], false);
  /* Clear the interrupt status flag.  This is done to make sure the interrupt flag is cleared before we sample. */
  MAP_ADC14_clearInterruptFlag(MAP_ADC14_getEnabledInterruptStatus());
}

//uint32_t gpio_to_pwm_output(const pin_name p_gpio) {
//  return 0;
//  uint32_t pwm = PWM_OUT_0; // p42
//  switch (p_gpio) {
//  case p43:
//    pwm = PWM_OUT_1;
//    break;
//  case p44:
//    pwm = PWM_OUT_2;
//    break;
//  case p45:
//    pwm = PWM_OUT_3;
//    break;
//  case p49:
//    pwm = PWM_OUT_4;
//    break;
//  case p50:
//    pwm = PWM_OUT_5;
//    break;
//  case p62:
//    pwm = PWM_OUT_7;
//    break;
//  case p63:
//    pwm = PWM_OUT_6;
//    break;
//  } /* End of 'switch' statement */
//
//  return pwm;
//}

//uint32_t gpio_to_pwm_pinmap(const pin_name p_gpio) {
//  return 0;
//  uint32_t pwm = GPIO_PF0_M0PWM0; // p42
//  switch (p_gpio) {
//  case p43:
//    pwm = GPIO_PF1_M0PWM1;
//    break;
//  case p44:
//    pwm = GPIO_PF2_M0PWM2;
//    break;
//  case p45:
//    pwm = GPIO_PF3_M0PWM3;
//    break;
//  case p49:
//    pwm = GPIO_PG0_M0PWM4;
//    break;
//  case p50:
//    pwm = GPIO_PG1_M0PWM5;
//    break;
//  case p62:
//    pwm = GPIO_PK5_M0PWM7;
//    break;
//  case p63:
//    pwm = GPIO_PK4_M0PWM6;
//    break;
//  } /* End of 'switch' statement */
//
//  return pwm;
//}

//uint32_t gpio_to_pwm_module(const pin_name p_gpio) {
//  return 0;
//  uint32_t pwm = PWM_GEN_0;
//  switch (p_gpio) {
//  case p42:
//    // No break;
//  case p43:
//    pwm = PWM_GEN_0;
//    break;
//  case p44:
//    // No break;
//  case p45:
//    pwm = PWM_GEN_1;
//    break;
//  case p49:
//    // No break;
//  case p50:
//    pwm = PWM_GEN_2;
//    break;
//  case p62:
//    // No break;
//  case p63:
//    pwm = PWM_GEN_3;
//    break;
//  } /* End of 'switch' statement */
//
//  return pwm;
//}

//uint32_t gpio_to_pwm_enable(const pin_name p_gpio) {
//  return 0;
//  uint32_t pwm = PWM_OUT_0_BIT; // p42
//  switch (p_gpio) {
//  case p43:
//    pwm = PWM_OUT_1_BIT;
//    break;
//  case p44:
//    pwm = PWM_OUT_2_BIT;
//    break;
//  case p45:
//    pwm = PWM_OUT_3_BIT;
//    break;
//  case p49:
//    pwm = PWM_OUT_4_BIT;
//    break;
//  case p50:
//    pwm = PWM_OUT_5_BIT;
//    break;
//  case p62:
//    pwm = PWM_OUT_7_BIT;
//    break;
//  case p63:
//    pwm = PWM_OUT_6_BIT;
//    break;
//  } /* End of 'switch' statement */
//
//  return pwm;
//}

void insert_sort(uint32_t *p_gpio, const uint8_t p_len, const uint32_t p_value) {
  volatile uint8_t i = p_len - 1;
  while ((i > 0) && (*(p_gpio + i - 1) > p_value)) {
    *(p_gpio + i) = *(p_gpio + i - 1);
    i -= 1;
  } // End of 'while' statement
  *(p_gpio + i) = p_value;
}
