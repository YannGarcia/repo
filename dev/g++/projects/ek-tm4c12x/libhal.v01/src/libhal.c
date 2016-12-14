/**
 * @file    libhal.c
 * @brief   Main implementation file for the Hardware Abstract Layer library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "libhal.h"

#define PWM_FIXED_FREQUENCY ((float)100.0e6)            /*!< Default PWM ferequecy */

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
#define MAX_GPIO_ID 90                                  /*!< Maximal GPIO index value */

/** @enum gpioMode
 * @brief GPIO access mode
 */
typedef enum {
  gpio_access_shared,                                    /*!< GPIO access is shared */
  gpio_access_greedy,                                    /*!< GPIO access is not shared */
  gpio_access_weak                                       /*!< ??? */
} gpio_access_t;

/** @enum gpio_types_t
 * @brief GPIO type
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
 * @brief This structure describes the digital GPIO user interface
 */
typedef struct {
  gpio_callback_t callback;                            /*!< ISR callback */
} digital_context_t;

/** @struct ADC GPIO user interface
 * @brief This structure describes the ADC GPIO user interface
 */
typedef struct {
  gpio_callback_t callback;                            /*!< ISR callback */
} adc_context_t;

typedef struct {
  uint32_t range;
  uint32_t clock;
  float period;                                         /*!< Period of the PWM signal (in ns) */
  float duty_cycle;                                     /*!< Duty cycle of the PWM signal (in ns) */
} pwm_context_t;

typedef struct {
  pin_name gpio;                                        /*!< Gpio port number */
  int32_t shared;                                       /*!< Gpio shared counter. Default: 0 (not shared) */
  gpio_types_t type;
  union {
    digital_context_t digital;
    pwm_context_t pwm;                                  /*!< The PWM chip set number */
    adc_context_t adc;                                  /*!< The ADC chip set number */
  } types;
} gpio_context;

static uint8_t is_initialised = FALSE;                  /*!< Indicate if the library was initialized by the process */
static uint32_t sysClock = -1;                          /*!< PLL descriptor */
static gpio_context * context_handles[MAX_GPIO_ID];     /*!< context_handles: Map a file descriptor from the /sys/class/xxx */
static uint64_t time_ms, time_us ;                      /*!< Time for easy calculations */

/**
 * @fn uint8_t shift_in(const pin_name p_data_pin, const uint8_t p_clock_pin, const shift_modes_t order)
 * @brief Initialise internal time variables
 */
static void initialise_time(void);
static int32_t create_new_context(const pin_name p_gpio, const gpio_access_t p_gpio_access, const gpio_types_t p_gpio_type);
static int32_t free_context(const pin_name p_gpio);
static void pwm_update(gpio_context *p_gpio);

int32_t libhal_setup(void)
{
  return libhal_setup_sys();
}

void pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode)
{
  /* Sanity check */
  if (p_gpio > MAX_GPIO_ID) {
    return;
  }

  if (context_handles[p_gpio] == NULL) {
    switch (p_mode) {
    case gpio_modes_digital_input:
      /* No break; */
    case gpio_modes_digital_output:
    /* Allocation GPIO */
      if (create_new_context(p_gpio, gpio_access_weak, gpio_types_digital) < 0) {
        return;
      }
      break;
    case gpio_modes_pwm_output:
      /* Allocation GPIO */
      if (create_new_context(p_gpio, gpio_access_weak, gpio_types_pwm) < 0) {
        return;
      }
      break;
    case gpio_modes_adc_input:
      /* TODO To be implemented */
      break;
    case gpio_modes_clock:
      /* TODO To be implemented */
      break;
    } // End of 'switch' statement
  }

  /* Set the direction */
  switch (p_mode) {
  case gpio_modes_digital_input:
    /* Set gpio_modes_digital_input mode */
    GPIOPinTypeGPIOInput(p_gpio >> 8 & 0xff, p_gpio & 0xff);
    break;
  case gpio_modes_digital_output:
    /* Set gpio_modes_digital_outut mode */
    GPIOPinTypeGPIOOutput(p_gpio >> 8 & 0xff, p_gpio & 0xff);
    break;
  case gpio_modes_adc_input:
    /* TODO */
    break;
  case gpio_modes_pwm_output:
    /* TODO */
    break;
  case gpio_modes_clock:
    /* TODO To be implemented */
    break;
  } /* End of 'switch' statement */
  
  return;
} /* End of function pin_mode */

int32_t pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud) {
  // FIXME How to dynamically modify Pull Up/Down without modifying Device Tree Overlay 
  return -1;
}

digital_state_t digital_read(const pin_name p_gpio) {
  int32_t c;

  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    return digital_state_error;
  }
  
  // On-Board Pin
  if (context_handles[p_gpio] == NULL) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_digital) < 0) {
      return digital_state_low;
    }
    // Set direction in
    pin_mode(p_gpio, gpio_modes_digital_input);
  }
  // Read value
  c = GPIOPinRead(p_gpio >> 8 & 0xff, p_gpio & 0xff);
  return ((c & (1 << p_gpio & 0xff)) == '0') ? digital_state_low : digital_state_high;
}

void digital_write(const pin_name p_gpio, const digital_state_t p_value) {
  int32_t c;

  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    fprintf(stderr, "digitalWrite: Wrong parameter\n");
    return;
  }

  if (context_handles[p_gpio] == NULL) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_digital) < 0) {
      return;
    }
    // Set direction out
    pin_mode(p_gpio, gpio_modes_digital_output);
  }
  // Write value
  c = GPIOPinRead(p_gpio >> 8 & 0xff, p_gpio & 0xff);
  if (p_value == digital_state_low) {
    c &= ~(1 << (p_gpio & 0xff));
  } else {
    c |= 1 << (p_gpio & 0xff);
  }
  GPIOPinWrite(p_gpio >> 8 & 0xff, p_gpio & 0xff, c);
  
  return;
}

int32_t pwm_write(const pin_name p_gpio, const uint32_t p_value) {
  // Sanity check
  if (context_handles[p_gpio] == NULL) {
    // FIXME Add default behavior
    return 0;
  }

  return -1;
} // End of function pwm_write

void pwm_set_mode(const uint8_t  p_mode) {
} // End of function pwm_set_mode

int32_t pwm_set_range(const pin_name p_gpio, const uint32_t p_range) {
  // Sanity check
  if (context_handles[p_gpio] == NULL) {
    return -1;
  }

  return -1;
} // End of function pwm_set_range

int32_t pwm_set_clock(const pin_name p_gpio, const uint32_t p_divisor) {
  // Sanity check
  if (context_handles[p_gpio] == NULL) {
    return -1;
  }

  return -1;
} // End of function pwm_set_clock

float analog_read(const pin_name p_gpio) {
  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    fprintf(stderr, "analog_read: Wrong parameter\n");
    return 0.0;
  }

  if (context_handles[p_gpio] == NULL) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpio_types_adc) < 0) {
      return 0.0;
    }
    // Set direction out
    pin_mode(p_gpio, gpio_modes_adc_input);
  }

  // Read value
  /* uint8_t buffer[8]; */
  /* lseek(context_handles[p_gpio]->types.adc.fd, 0L, SEEK_SET); */
  /* read(context_handles[p_gpio]->types.adc.fd, &buffer, 8); */
  /* // Convert it */
  /* float value = (uint32_t)atoi((const char *)&buffer[0]) / 1000.0; */
  /* uint8_t i = 0; */
  /* for ( ; i < 3; i++) { */
  /*   // Read value */
  /*   read(context_handles[p_gpio]->types.adc.fd, &buffer, 8); */
  /*   // Convert it */
  /*   value += (uint32_t)atoi((const char *)&buffer[0]) / 1000.0; */
  /* } // End of 'for' statement */
  /* value /= 4.0; */

  /* return value; */
  return 0.0;
} // End of function analogRead

void wait_ms(const uint32_t p_delay_ms)
{
  ROM_SysCtlDelay(p_delay_ms * (ROM_SysCtlClockGet() / 3000));
}

void wait_us(const uint32_t p_delay_us) {
  ROM_SysCtlDelay(p_delay_us * ((ROM_SysCtlClockGet() / 3000) / 1000));
}

void reset_time(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  time_ms = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  time_us = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec);
}

uint32_t elapse_ms(void) {
  struct timeval tv ;
  uint64_t now ;

  gettimeofday (&tv, NULL);
  now = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);

  return (uint32_t)(now - time_ms);
}

uint32_t elapse_us(void) {
  struct timeval tv;
  uint64_t now;

  gettimeofday (&tv, NULL);
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec ;

  return (uint32_t)(now - time_us);
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
    
  // Sanity check
  if (is_initialised == TRUE) {
    return -1;
  }
  // Configure table according to board version
  revision = board_revision();
  if (revision == 1) {
  } // else, handle errors
    
  initialise_time();
    
  // Initalize context_handles table
  memset((void *)context_handles, 0x00, MAX_GPIO_ID * sizeof(gpio_context *));
  
  memset((void *)&isr_fds, 0x00, sizeof(isr_fds));
  isr_fds_num = 0;
  
  // Run from the PLL at 120 MHz.
  sysClock = MAP_SysCtlClockFreqSet(
				    (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
				    120000000);
  
  // Configure the device pins.
  PinoutSet(false, false);
  
  is_initialised = TRUE;
  
  return 0;
}

void libhal_shutdown_sys(void) {
  int32_t i = 0;
  
  for(i = 0; i < MAX_GPIO_ID; i++) {
    if (context_handles[i] != NULL) {
      free_context(context_handles[i]->gpio);
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

  // Sanity checks
  if (p_gpio > MAX_GPIO_ID) {
    return -1;
  }

  if (context_handles[p_gpio] == NULL) {
    // Allocate value
    gpio = (gpio_context *)malloc(sizeof(gpio_context));
    gpio->gpio = p_gpio;
    gpio->shared = 0;
    // Export it 
    switch (p_gpio_type) {
    case gpio_types_digital:
      gpio->type = gpio_types_digital;
      gpio->types.digital.callback.callback_fn = NULL;
      gpio->types.digital.callback.callback_arg = NULL;
      gpio->types.digital.callback.ready = 0xff;
      break;
    case gpio_types_pwm:
      gpio->type = gpio_types_pwm;
      gpio->types.pwm.range = 256;
      gpio->types.pwm.clock = 1;
      pwm_update(gpio);
      break;
    case gpio_types_adc:
      gpio->type = gpio_types_adc;
      break;
    } // End of 'switch' mode
    context_handles[p_gpio] = gpio;
  } else if (p_gpio_access == gpio_access_weak) {
    free(gpio);
    return -1; // TODO Error
  } else {
    gpio = context_handles[p_gpio];
    if (p_gpio_access == gpio_access_shared) {
      gpio->shared += 1;
    }
  }
  
  return 0;
}

int32_t free_context(const pin_name p_gpio) {
  gpio_context * gpio;
  
  // Sanity checks
  if ((p_gpio > MAX_GPIO_ID) || (context_handles[p_gpio] == NULL)) {
    return -1;
  }

  gpio = context_handles[p_gpio];
  if (gpio->shared == 0) { // Free it
    switch (gpio->type) {
    case gpio_types_digital:
      if (gpio->types.digital.callback.ready != 0xff) {
	// TODO
	gpio->types.digital.callback.ready = 0xff;
      }
      break;
    case gpio_types_pwm:
      // TODO
      break;
    case gpio_types_adc:
      // TODO
      break;
      // FIXME TO be done
      break;
    } // End of 'switch' statement
    // Free the GPIO descriptor
    free((void *)gpio);
    context_handles[p_gpio] = NULL;
  } else {
    gpio->shared -= 1;
  }
  
  return 0;
}

void pwm_update(gpio_context *p_gpio) {
} // End of function pwm_update

static void initialise_time(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  time_ms = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  time_us = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec);
}

