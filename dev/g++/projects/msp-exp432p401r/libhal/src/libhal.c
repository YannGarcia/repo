/**
 * @file      libhal.c
 * @brief     Main implementation file for the Hardware Abstract Layer library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2016-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 * @see       MSP432® Peripheral Driver Library - USER’S GUIDE
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

#define PWM_FIXED_FREQUENCY ((float)CS_48MHZ)            /*!< Default PWM frequency with no divisor */

#define PWM_CONTROL 0
#define PWM_STATUS  1
#define PWM0_RANGE  4
#define PWM0_DATA   5
#define PWM1_RANGE  8
#define PWM1_DATA   9

#ifndef TRUE
#define TRUE    (1==1)
#define FALSE   (1==2)
#endif

/* Number of pins */
#define MAX_GPIO_ID 255                                 /*!< Maximal GPIO index value */

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
  uint32_t divisor;                                     /*!< Divisor to apply for the PWM base frequency */
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
    digital_context_t digital;
    pwm_context_t pwm;                                  /*!< The PWM chip set number */
    adc_context_t adc;                                  /*!< The ADC chip set number */
  } types;
} gpio_context;

typedef struct { // FIXME analog_gpios & num_configured_analog_gpios seems to be useless for analog_read
  gpio_context *analog_gpios[MAX_ANALOG_GPIO];          /*!< The analog GPIOs list */
  uint8_t num_configured_analog_gpios;                  /*!< The number of analog GPIOs used */
  uint32_t base_address;                                /*!< The ADC base address */
  bool initialised;                                     /*!< Set to true if the ADC is initialised */
  uint8_t sequencer;                                    /*!< The sequencer to be used */
  uint32_t samples[8]; // Sequence 3 has a FIFO depth of 1
                              // 8: maximal number of channels on  SS0
} adc_configuration_;
static uint32_t gpio_idx[8]; // Reused several times in the function
                             // 8: maximal number of channels on  SS0

static uint8_t is_initialised = FALSE;                  /*!< Indicate if the library was initialized by the process */
static uint32_t sys_clock = 0;                          /*!< Indicate the configured system clock frequency in Hz */
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
static void pwm_update(gpio_context *p_gpio);
static uint8_t gpio_to_index(const pin_name p_gpio);
static void enable_adc_periph(const pin_name p_gpio);
static void enable_adcs_periph(const pin_name * p_gpio, const uint32_t p_len);
static uint32_t gpio_to_pwm_output(const pin_name p_gpio); // TODO Check if this function is really useful
static uint32_t gpio_to_pwm_pinmap(const pin_name p_gpio); // TODO Check if this function is really useful
static uint32_t gpio_to_pwm_module(const pin_name p_gpio); // TODO Check if this function is really useful
static uint32_t gpio_to_pwm_enable(const pin_name p_gpio); // TODO Check if this function is really useful

/**
 * @fn void initialise_gpios(const bool p_ethernet_mode, const bool p_usb_mode)
 * @brief Configure the device pins for the standard usages on the EK-TM4C129x
 * @see Tiva™ TM4C129ENCPDT Microcontroller Clause 10
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
    // Configure the GPIO for PWM
//    GPIOPinConfigure(gpio_to_pwm_pinmap(p_gpio));
//    GPIOPinTypePWM(
//                   p_gpio & 0xffffffff00, /* Port register */
//                   p_gpio & 0xffff        /* Port number */
//                  );
//    // Configure the PWM generator for count down mode with immediate updates to the parameters
//    PWMGenConfigure(PWM0_BASE, gpio_to_pwm_module(p_gpio), PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
//    // Disable the PWM dead band output
//    PWMDeadBandDisable(PWM0_BASE, gpio_to_pwm_module(p_gpio));
//    // Setup PWM module
//    pwm_update(&context_handles[gpio_idx]);
    break;
  case gpio_modes_clock:
    /* TODO To be implemented */
    break;
  } /* End of 'switch' statement */

  return 0;
} /* End of function pin_mode */

int32_t pins_mode(const pin_name * p_gpios, const uint8_t p_len, const gpio_modes_t p_mode)
{
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
          return -1;
        }
        break;
      case gpio_modes_pwm_output:
        /* Allocation GPIO */
        if (create_new_context(*(p_gpios + gpio), gpio_access_weak, gpio_types_pwm) < 0) {
          return -1;
        }
        break;
      case gpio_modes_adc_input:
        /* Allocation GPIO */
        if (create_new_context(*(p_gpios + gpio), gpio_access_weak, gpio_types_adc) < 0) {
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
  } // End of 'for' statement
  if (p_mode == gpio_modes_adc_input) {
    enable_adcs_periph(p_gpios, p_len);
  }

  return 0;
} /* End of function pin_mode */

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

int32_t pwm_write(const pin_name p_gpio, const uint32_t p_value) {
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

  return 0;
} // End of function pwm_write

void pwm_set_mode(const uint8_t p_mode) {
  /* Nothing to do */
} // End of function pwm_set_mode

int32_t pwm_set_range(const pin_name p_gpio, const float p_frequency) {
  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if ((gpio_idx > MAX_GPIO_ID) || (context_handles[gpio_idx].gpio == NC)) {
  // FIXME Add default behavior
  return -1;
  }

  context_handles[gpio_idx].types.pwm.frequency = p_frequency;
  pwm_update(&context_handles[gpio_idx]);

  return 0;
} // End of function pwm_set_range

int32_t pwm_set_clock(const pin_name p_gpio, const uint32_t p_divisor) {
  /* Sanity check */
  uint8_t gpio_idx = gpio_to_index(p_gpio);
  if ((gpio_idx > MAX_GPIO_ID) || (context_handles[gpio_idx].gpio == NC)) {
  // FIXME Add default behavior
  return -1;
  }

  context_handles[gpio_idx].types.pwm.divisor = p_divisor;
  pwm_update(&context_handles[gpio_idx]);

  return 0;
} // End of function pwm_set_clock

float analog_read(const pin_name p_gpio) {
    return (float)LONG_MAX;
//  /* Sanity checks */
//  uint8_t channel= 0;
//  uint8_t gpio_idx = gpio_to_index(p_gpio);
//  if (gpio_idx > MAX_GPIO_ID) {
//    fprintf(stderr, "analog_read: Wrong parameter\n");
//    return (float)LONG_MAX;
//  }
//
//  if (context_handles[gpio_idx].gpio == NC) {
//    // Allocation GPIO
//    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_adc) < 0) {
//      return (float)LONG_MAX;
//    }
//    // Set GPIO pin mode
//    pin_mode(p_gpio, gpio_modes_adc_input);
//  }
//
//  // Setup the correct channel
//  if ((p_gpio & 0xffffffff00) == GPIO_PORTE_BASE) {
//    switch (p_gpio & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
//      case GPIO_PIN0:
//        channel = ADC_CTL_CH3;
//        break;
//      case GPIO_PIN1:
//        channel = ADC_CTL_CH2;
//        break;
//      case GPIO_PIN2:
//        channel = ADC_CTL_CH1;
//        break;
//      case GPIO_PIN3:
//        channel = ADC_CTL_CH0;
//        break;
//      case GPIO_PIN4:
//        channel = ADC_CTL_CH9;
//        break;
//      case GPIO_PIN5:
//        channel = ADC_CTL_CH8;
//        break;
//    } // End of 'switch' statement
//  } else if ((p_gpio & 0xffffffff00) == GPIO_PORTB_BASE) {
//    switch (p_gpio & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
//      case GPIO_PIN4:
//        channel = ADC_CTL_CH10;
//        break;
//      case GPIO_PIN5:
//        channel = ADC_CTL_CH11;
//        break;
//    } // End of 'switch' statement
//  }
//  ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, channel | ADC_CTL_IE | ADC_CTL_END);
//
//  // Trigger the ADC conversion
//  ADCProcessorTrigger(adc_configuration.base_address, 3);
//  // Wait result
//  while(!ADCIntStatus(adc_configuration.base_address, 3, false));
//  // Clear the ADC interrupt flag.
//  ADCIntClear(adc_configuration.base_address, 3);
//  // Read ADC Value.
//  if (ADCSequenceDataGet(adc_configuration.base_address, 3, adc_configuration.samples) != 1) {
//    return (float)LONG_MAX;
//  }
//
//  /* return value; */
//  float value = (float)(adc_configuration.samples[0] & 0x0fff) * 3.3 / 4096.0; // See Tiva� TM4C1294NCPDT Microcontroller DATA SHEET Clause 15.3.4.1 Voltage Reference
//  return (float)value;
} // End of function analogRead

void analog_multiple_read(const pin_name * p_gpios, const uint8_t p_len, float *p_values) {
    return;
//  /* Sanity checks */
//  uint8_t gpio;
//  bool reconfigure = false;
//  for (gpio = 0; gpio < p_len; gpio++) {
//    *(p_values + gpio) = (float)LONG_MAX;
//    gpio_idx[gpio] = gpio_to_index(*(p_gpios + gpio));
//    if (gpio_idx[gpio] > MAX_GPIO_ID) {
//      fprintf(stderr, "analog_read: Wrong parameter\n");
//      return;
//    }
//    if (context_handles[gpio_idx[gpio]].gpio == NC) {
//      reconfigure = true; // Shall call pins_mode
//      // Allocation GPIO
//      if (create_new_context(*(p_gpios + gpio), gpio_access_weak, gpio_types_adc) < 0) {
//        return;
//      }
//    }
//  } // End of 'for' statement
//  if (reconfigure) {// Set GPIO pin mode
//    pins_mode(p_gpios, p_len, gpio_modes_adc_input);
//  }
//  // Setup the correct channel
//  for (gpio = 0; gpio < p_len; gpio++) {
//      if ((*(p_gpios + gpio) & 0xffffff00) == GPIO_PORTE_BASE) {
//        switch (*(p_gpios + gpio) & 0xff) { // GPIO_PORTE_BASE TODO Add support for other port
//          case GPIO_PIN0:
//            gpio_idx[gpio] = ADC_CTL_CH3;
//            break;
//          case GPIO_PIN1:
//            gpio_idx[gpio] = ADC_CTL_CH2;
//            break;
//          case GPIO_PIN2:
//            gpio_idx[gpio] = ADC_CTL_CH1;
//            break;
//          case GPIO_PIN3:
//            gpio_idx[gpio] = ADC_CTL_CH0;
//            break;
//          case GPIO_PIN4:
//            gpio_idx[gpio] = ADC_CTL_CH9;
//            break;
//          case GPIO_PIN5:
//            gpio_idx[gpio] = ADC_CTL_CH8;
//            break;
//        } // End of 'switch' statement
//      } else if ((*(p_gpios + gpio) & 0xffffff00) == GPIO_PORTB_BASE) {
//        switch (*(p_gpios + gpio) & 0xff) { // GPIO_PORTE_BASE TODO Add support for other port
//          case GPIO_PIN4:
//            gpio_idx[gpio] = ADC_CTL_CH10;
//            break;
//          case GPIO_PIN5:
//            gpio_idx[gpio] = ADC_CTL_CH11;
//            break;
//        } // End of 'switch' statement
//      }
//      ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, gpio_idx[gpio] | ADC_CTL_IE);
//  } // End of 'for' statement
//  // The last channel shall contain the flag ADC_CTL_END
//  ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, p_len - 1, gpio_idx[p_len - 1] | ADC_CTL_IE | ADC_CTL_END);
//
//  // Trigger the ADC conversion
//  ADCProcessorTrigger(adc_configuration.base_address, adc_configuration.sequencer);
//  // Wait result
//  while(!ADCIntStatus(adc_configuration.base_address, adc_configuration.sequencer, false));
//  // Clear the ADC interrupt flag.
//  ADCIntClear(adc_configuration.base_address, adc_configuration.sequencer);
//  // Read ADC Value.
//  if (ADCSequenceDataGet(adc_configuration.base_address, adc_configuration.sequencer, adc_configuration.samples) != 1) {
//    return;
//  }
//
//  /* convert values */
//  for (gpio = 0; gpio < p_len; gpio++) {
//    *(p_values + gpio) = (float)(adc_configuration.samples[gpio] & 0x0fff) * 3.3 / 4096.0; // See Tiva� TM4C1294NCPDT Microcontroller DATA SHEET Clause 15.3.4.1 Voltage Reference
//  } // End of 'for' statement
//
//  return;
} // End of function analogRead

void wait_ms(const uint32_t p_delay_ms)
{
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

  // Enable processor interrupts
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
  MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
  MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
  MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
  MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

  sys_clock = CS_48MHZ;

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
  } // End of 'for' statement

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
      gpio->types.pwm.divisor = 64;        /* PWM frequency base = System frequency / 64 */
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
  //    } // End of 'switch' statement
  //    // Free the GPIO descriptor
  //    free((void *)gpio);
  //    context_handles[gpio_idx] = NULL;
  //  } else {
  //    gpio->shared -= 1;
  //  }

  return 0;
}

void pwm_update(gpio_context *p_gpio) {
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
//  } // End of 'switch' statement
//  PWMClockSet(PWM0_BASE, divisor);
//  // Set the period
//  PWMGenPeriodSet(PWM0_BASE, gpio_to_pwm_module(p_gpio->gpio), pwm_period);
//  // Set the pulse width of PWM0 for a 25% duty cycle.
//  PWMPulseWidthSet(PWM0_BASE, gpio_to_pwm_output(p_gpio->gpio), pwm_duty);

} // End of function pwm_update

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
  } // End of 'switch' statement

  return port;
}

void initialise_gpios(const bool p_ethernet_mode, const bool p_usb_mode) {

  // P1.1 and P1.4 are on board Switch 1 and 2
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);
  MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4); // SLAU597B–March 2015–Revised July 2016 - Figure 44. Schematics (2 of 6)

  // P1.0, P2.0, P2.1 and P2.2 are on board led and RGB leds
  MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
  MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
  // Default the LEDs to OFF.
  MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
  MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
}

void enable_adc_periph(const pin_name p_gpio) {
//  uint8_t gpio_idx = gpio_to_index(p_gpio);
//
//  switch (p_gpio & 0xffffffff00) {
//    case GPIO_PORTE_BASE:
//    case GPIO_PORTB_BASE:
//      if (!adc_configuration.initialised) {
//        adc_configuration.initialised = true;
//        adc_configuration.base_address = ADC0_BASE; // TODO Check to use use ADC1_BASE or keep it for PWM?
//        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // Enable ADC0 peripheral
//        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));
//        // Configure the ADC to use PLL at 480 MHz divided by 24 to get an ADC clock of 20 MHz
//        ADCClockConfigSet(adc_configuration.base_address, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 24);
//        // Configures the trigger source and priority of a sample sequence
//        ADCSequenceConfigure(adc_configuration.base_address, 3, ADC_TRIGGER_PROCESSOR, 2/*Priotity between 0 & 3*/);
//        ADCReferenceSet(adc_configuration.base_address, ADC_REF_INT);
//      }
//      break;
//  } // End of 'switch' statement
//  GPIOPinTypeADC(
//                 p_gpio & 0xffffffff00, /* Port register */
//                 p_gpio & 0xffff        /* Port number */
//                 );
//  // Setup the correct channel
//  if ((p_gpio & 0xffffffff00) == GPIO_PORTE_BASE) {
//    switch (p_gpio & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
//      case GPIO_PIN0:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH3); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      case GPIO_PIN1:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH2); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      case GPIO_PIN2:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH1); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      case GPIO_PIN3:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH0); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      case GPIO_PIN4:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH9); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      case GPIO_PIN5:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH8); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//    } // End of 'switch' statement
//  } else if ((p_gpio & 0xffffffff00) == GPIO_PORTB_BASE) {
//    switch (p_gpio & 0xffff) { // GPIO_PORTE_BASE TODO Add support for other port
//      case GPIO_PIN4:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH10); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      case GPIO_PIN5:
//        ADCSequenceStepConfigure(adc_configuration.base_address, 3, 0, ADC_CTL_CH11); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//    } // End of 'switch' statement
//  }
//  // Since sample sequence 3 is now configured, it must be enabled.
//  ADCSequenceEnable(adc_configuration.base_address, 3);
//
//  // Clear the interrupt status flag.  This is done to make sure the interrupt flag is cleared before we sample.
//  ADCIntClear(adc_configuration.base_address, 3);
}

void enable_adcs_periph(const pin_name * p_gpios, const uint32_t p_len) {
//  // Select the correct sequencer
//  uint8_t gpio;
//  for (gpio = 0; gpio < p_len; gpio++) {
//    adc_configuration.analog_gpios[adc_configuration.num_configured_analog_gpios] = &context_handles[gpio_to_index(*(p_gpios + gpio))];
//    adc_configuration.num_configured_analog_gpios += 1;
//  } // End of 'for' statement
//  switch (adc_configuration.num_configured_analog_gpios) {
//    case 4:
//      adc_configuration.sequencer = 2;
//      break;
//    default:
//      adc_configuration.sequencer = 0;
//      break;
//  } // End of 'switch' staement
//  // Setup ADC peripheral
//  for (gpio = 0; gpio < p_len; gpio++) {
//    switch (*(p_gpios + gpio) & 0xffffff00) {
//      case GPIO_PORTE_BASE:
//      case GPIO_PORTB_BASE:
//        if (!adc_configuration.initialised) {
//          adc_configuration.initialised = true;
//          adc_configuration.base_address = ADC0_BASE; // TODO Use ADC1_BASE or keep it for PWM and analog comparators?
//          SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // Enable ADC0 peripheral
//          while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));
//          // Configure the ADC to use PLL at 480 MHz divided by 24 to get an ADC clock of 20 MHz
//          ADCClockConfigSet(adc_configuration.base_address, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 24);
//          // Configures the trigger source and priority of a sample sequence
//          ADCSequenceConfigure(adc_configuration.base_address, adc_configuration.sequencer, ADC_TRIGGER_PROCESSOR, 2/*Priotity between 0 & 3*/);
//        }
//        break;
//    } // End of 'switch' statement
//    GPIOPinTypeADC(
//                   *(p_gpios + gpio) & 0xffffff00, /* Port register */
//                   *(p_gpios + gpio) & 0xff        /* Port number */
//                   );
//  } // End of 'for' statement
//
//  // Setup the channels
//  for (gpio = 0; gpio < p_len; gpio++) {
//    if ((*(p_gpios + gpio) & 0xffffff00) == GPIO_PORTE_BASE) {
//      switch (*(p_gpios + gpio) & 0xff) { // GPIO_PORTE_BASE TODO Add support for other port
//        case GPIO_PIN0:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH3); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//        case GPIO_PIN1:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH2); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//        case GPIO_PIN2:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH1); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//        case GPIO_PIN3:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH0); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//        case GPIO_PIN4:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH9); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//        case GPIO_PIN5:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH8); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      } // End of 'switch' statement
//    } else if ((*(p_gpios + gpio) & 0xffffff00) == GPIO_PORTB_BASE) {
//      switch (*(p_gpios + gpio) & 0xff) { // GPIO_PORTE_BASE TODO Add support for other port
//        case GPIO_PIN4:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH10); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//        case GPIO_PIN5:
//        ADCSequenceStepConfigure(adc_configuration.base_address, adc_configuration.sequencer, gpio, ADC_CTL_CH11); // | ADC_CTL_IE | ADC_CTL_END);
//        break;
//      } // End of 'switch' statement
//    }
//  } // End of 'for' statement
//  // Since sample sequence 3 is now configured, it must be enabled.
//  ADCSequenceEnable(adc_configuration.base_address, adc_configuration.sequencer);
//
//  // Clear the interrupt status flag.  This is done to make sure the interrupt flag is cleared before we sample.
//  ADCIntClear(adc_configuration.base_address, adc_configuration.sequencer);
}

uint32_t gpio_to_pwm_output(const pin_name p_gpio) {
  return 0;
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
//  } // End of 'switch' statement
//
//  return pwm;
}

uint32_t gpio_to_pwm_pinmap(const pin_name p_gpio) {
  return 0;
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
//  } // End of 'switch' statement
//
//  return pwm;
}

uint32_t gpio_to_pwm_module(const pin_name p_gpio) {
  return 0;
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
//  } // End of 'switch' statement
//
//  return pwm;
}

uint32_t gpio_to_pwm_enable(const pin_name p_gpio) {
  return 0;
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
//  } // End of 'switch' statement
//
//  return pwm;
}


