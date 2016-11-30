/**
 * @file    libhal.c
 * @brief   Main implementation file for the Hardware Abstract Layer library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#include "libhal.h"

#define SYSFS_GPIO_DIR  "/sys/class/gpio"               /*!< GPIO user interface */
#define SYSFS_PWM_DIR   "/sys/devices/ocp.3/pwm_test_"  /*!< PWM user interface */
#define SYSFS_ADC_DIR "/sys/devices/ocp.3/44e0d000.tscadc/tiadc/iio:device0"  /*!< ADC user interface */
#define POLL_TIMEOUT    5000 /* 5 seconds */            /*!< Default polling timeout */
#define MAX_BUFFER_SIZE 256                             /*!< Default maximum buffer size */

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

// Number of pins
#define MAX_GPIO_ID 255                                 /*!< Maximal GPIO index value */

/** @enum gpioMode
 * @brief GPIO access mode
 */
typedef enum {
  gpio_access_shared,                                    /*!< GPIO access is shared */
  gpio_access_greedy,                                    /*!< GPIO access is not shared */
  gpio_access_weak,                                      /*!< ??? */
} gpio_access_t;

/** @enum gpioi_types_t
 * @brief GPIO type
 */
typedef enum {
  gpioi_types_digital,                                  /*!< GPIO digital type */
  gpioi_types_pwm,                                      /*!< GPIO PWM type */
  gpioi_types_adc,                                      /*!< GPIO analog/digital type */
} gpioi_types_t;

typedef struct {
  int32_t (* callback_fn)(void *);                      /*!< Callback to be called on ISR. NOTE: Execution code shall be short as possible */
  void *callback_arg;
  int32_t ready;                                        /*!< Contains the pin value SysFS file descriptor, fd */
} gpioi_callback_t;

/** @struct digital GPIO user interface
 * @brief This structure describes the digital GPIO user interface
 */
typedef struct {
  int32_t fd;                                           /*!< pin value SysFS file descriptor */
  int32_t fdd;                                          /*!< direction SysFS file descriptor */
  int32_t fdm;                                          /*!< edge SysFS file descriptor */
  gpioi_callback_t callback;                            /*!< ISR callback */
} digital_context_t;

/** @struct ADC GPIO user interface
 * @brief This structure describes the ADC GPIO user interface
 */
typedef struct {
  int32_t fd;                                           /*!< pin value SysFS file descriptor */
  gpioi_callback_t callback;                            /*!< ISR callback */
} adc_context_t;

typedef struct {
  int32_t fdp;                                          /*!< PWM period SysFS file descriptor */
  int32_t fddc;                                         /*!< PWM Duty Cycle SysFS file descriptor */
  int32_t fdr;                                          /*!< PWM run/stop SysFS file descriptor */
  const char *pwmChip;                                  /*!< PWM chip number: 0: ephpwmA */
  uint32_t range;
  uint32_t clock;
  float period;                                         /*!< Period of the PWM signal (in ns) */
  float duty_cycle;                                     /*!< Duty cycle of the PWM signal (in ns) */
} pwm_context_t;

typedef struct {
  pin_name gpio;                                        /*!< Gpio port number */
  int32_t shared;                                       /*!< Gpio shared counter. Default: 0 (not shared) */
  gpioi_types_t type;
  union {
    digital_context_t digital;
    pwm_context_t pwm;                                  /*!< The PWM chip set number */
    adc_context_t adc;                                  /*!< The ADC chip set number */
  } types;
} gpio_context;

static uint8_t isInitialised = FALSE;                   /*!< Indicate if the library was initialized by the process */
static gpio_context * context_handles[MAX_GPIO_ID];     /*!< context_handles: Map a file descriptor from the /sys/class/xxx */
static pthread_t isr_thread_id = -1;                    /*!< GPIOs ISR thread identifier */
static struct pollfd isr_fds[MAX_GPIO_ID / 2];          /*!< poll file descriptors table */
static int32_t isr_gpios[MAX_GPIO_ID / 2];               /*!< poll file descriptors table */
static int32_t isr_fds_num;                             /*!< Number of file descriptors in the isr_fds table */
static uint64_t time_ms, time_us ;                      /*!< Time for easy calculations */

/**
 * @fn uint8_t shift_in(const pin_name p_data_pin, const uint8_t p_clock_pin, const shift_modes_t order)
 * @brief Initialise internal time variables
 */
static void initialise_time(void);
static int32_t create_new_context(const pin_name p_gpio, const gpio_access_t p_gpio_access, const gpioi_types_t p_gpio_type);
static int32_t free_context(const pin_name p_gpio);
static const uint8_t gpio_to_adc(const pin_name p_gpio); // TODO To be move into libhal_pin_name
static const char * pwm_chip_from_pin(const pin_name p_gpio); // TODO To be move into libhal_pin_name
static void pwm_update(gpio_context *p_gpio);

static void *interrupt_handler (void * p_arg);

int32_t libhal_setup(void)
{
  return libhal_setup_sys();
}

void pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode)
{
  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    return;
  }

  if (context_handles[p_gpio] == NULL) {
    switch (p_mode) {
    case gpio_modes_digital_input:
      // No break;
    case gpio_modes_digital_output:
      // Allocation GPIO
      if (create_new_context(p_gpio, gpio_access_weak, gpioi_types_digital) < 0) {
        return;
      }
      break;
    case gpio_modes_pwm_output:
      // Allocation GPIO
      if (create_new_context(p_gpio, gpio_access_weak, gpioi_types_pwm) < 0) {
        return;
      }
      break;
    case gpio_modes_adc_input:
      // TODO To be implemented
      break;
    case gpio_modes_clock:
      // TODO To be implemented
      break;
    } // End of 'switch' statement
  }

  // Set the direction
  switch (p_mode) {
  case gpio_modes_digital_input:
    // Set gpio_modes_digital_input mode
    write(context_handles[p_gpio]->types.digital.fdd, "in", 2);
    break;
  case gpio_modes_digital_output:
    // Set gpio_modes_digital_input mode
    write(context_handles[p_gpio]->types.digital.fdd, "out", 3);
    break;
  case gpio_modes_adc_input:
    // Nothing to do
    break;
  case gpio_modes_pwm_output:
    // Set duty cycle
    pwm_write(p_gpio, 50);
    // Start PWM
    write(context_handles[p_gpio]->types.pwm.fdr, "1", 1);
    break;
  case gpio_modes_clock:
    // TODO To be implemented
    break;
  } // End of 'switch' statement
  
  return;
} // End of function pin_mode

int32_t pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud) {
  // FIXME How to dynamically modify Pull Up/Down without modifying Device Tree Overlay 
  return -1;
}

digital_state_t digital_read(const pin_name p_gpio) {
  char c;

  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    return digital_state_error;
  }
  
  // On-Board Pin
  if (context_handles[p_gpio] == NULL) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpioi_types_digital) < 0) {
      return digital_state_low;
    }
    // Set direction in
    pin_mode(p_gpio, gpio_modes_digital_input);
  }
  // Read value
  lseek(context_handles[p_gpio]->types.digital.fd, 0L, SEEK_SET);
  read(context_handles[p_gpio]->types.digital.fd, &c, 1);
  return (c == '0') ? digital_state_low : digital_state_high;
}

void digital_write(const pin_name p_gpio, const digital_state_t p_value) {
  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    fprintf(stderr, "digitalWrite: Wrong parameter\n");
    return;
  }

  if (context_handles[p_gpio] == NULL) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpioi_types_digital) < 0) {
      return;
    }
    // Set direction out
    pin_mode(p_gpio, gpio_modes_digital_output);
  }
  // Write value
  lseek(context_handles[p_gpio]->types.digital.fd, 0, SEEK_SET);
  if (p_value == digital_state_low) {
    write(context_handles[p_gpio]->types.digital.fd, "0", 1);
  } else {
    write(context_handles[p_gpio]->types.digital.fd, "1", 1);
  }
  
  return;
}

int32_t pwm_write(const pin_name p_gpio, const uint32_t p_value) {
  char buffer[MAX_BUFFER_SIZE];

  // Sanity check
  if (context_handles[p_gpio] == NULL) {
    // FIXME Add default behavior
    return 0;
  }

  // Set the duty_cycle
  context_handles[p_gpio]->types.pwm.duty_cycle = context_handles[p_gpio]->types.pwm.period * (100 - p_value) / 100;
  sprintf(buffer, "%d", (uint32_t)context_handles[p_gpio]->types.pwm.duty_cycle);
  printf("Set duty cycle to %s/%d\n", buffer, (uint32_t)context_handles[p_gpio]->types.pwm.period);
  lseek(context_handles[p_gpio]->types.pwm.fddc, 0L, SEEK_SET);
  write(context_handles[p_gpio]->types.pwm.fddc, buffer, MAX_BUFFER_SIZE);
  
  return 0;
} // End of function pwm_write

void pwm_set_mode(const uint8_t  p_mode) {
} // End of function pwm_set_mode

int32_t pwm_set_range(const pin_name p_gpio, const uint32_t p_range) {
  // Sanity check
  if (context_handles[p_gpio] == NULL) {
    return -1;
  }

  context_handles[p_gpio]->types.pwm.range = p_range;
  pwm_update(context_handles[p_gpio]);
  
  return 0;
} // End of function pwm_set_rangexo

int32_t pwm_set_clock(const pin_name p_gpio, const uint32_t p_divisor) {
  // Sanity check
  if (context_handles[p_gpio] == NULL) {
    return -1;
  }

  context_handles[p_gpio]->types.pwm.clock = p_divisor;
  pwm_update(context_handles[p_gpio]);
  
  return 0;
} // End of function pwm_set_clock

float analog_read(const pin_name p_gpio) {
  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    fprintf(stderr, "analog_read: Wrong parameter\n");
    return 0.0;
  }

  if (context_handles[p_gpio] == NULL) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpioi_types_adc) < 0) {
      return 0.0;
    }
    // Set direction out
    pin_mode(p_gpio, gpio_modes_adc_input);
  }

  // Read value
  uint8_t buffer[8];
  lseek(context_handles[p_gpio]->types.adc.fd, 0L, SEEK_SET);
  read(context_handles[p_gpio]->types.adc.fd, &buffer, 8);
  // Convert it
  float value = (uint32_t)atoi((const char *)&buffer[0]) / 1000.0;
  uint8_t i = 0;
  for ( ; i < 3; i++) {
    // Read value
    read(context_handles[p_gpio]->types.adc.fd, &buffer, 8);
    // Convert it
    value += (uint32_t)atoi((const char *)&buffer[0]) / 1000.0;
  } // End of 'for' statement
  value /= 4.0;

  return value;
} // End of function analogRead

int32_t libhal_isr(const pin_name p_gpio, const isr_levels_t p_levels, int32_t (*p_function)(void *)) {
  const char *isr_level;
  char buffer[MAX_BUFFER_SIZE];
  int32_t flags;

  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    fprintf(stderr, "libhal_isr: Wrong parameter\n");
    return -1;
  } 
  // p_function could be null

  if (context_handles[p_gpio] == NULL) {
    // Allocation GPIO
    if (create_new_context(p_gpio, gpio_access_weak, gpioi_types_digital) < 0) {
      return -2;
    }
    // Set direction in
    pin_mode(p_gpio, gpio_modes_digital_input);    
  }

  // Set mode
  if (context_handles[p_gpio]->types.digital.fdm == -1) { // Open mode file descriptor
    sprintf (buffer, "%s/gpio%d/edge", SYSFS_GPIO_DIR, p_gpio);
    printf("libhal_isr: open '%s'\n", buffer);
    context_handles[p_gpio]->types.digital.fdm = open(buffer, O_RDWR);
  } // File descriptor is opened
  if (p_levels != isr_levels_setup) {
    if (p_levels == isr_levels_edge_falling)
      isr_level = "falling" ;
    else if (p_levels == isr_levels_edge_rising)
      isr_level = "rising" ;
    else
      isr_level = "both" ;
    printf("libhal_isr: mode '%s'\n", isr_level);
    write(context_handles[p_gpio]->types.digital.fdm, isr_level, strlen(isr_level) + 1);
  }

  // Set data structures
  context_handles[p_gpio]->types.digital.callback.callback_fn = p_function;
  context_handles[p_gpio]->types.digital.callback.ready = isr_fds_num;
  context_handles[p_gpio]->types.digital.callback.callback_arg = (void *)context_handles[p_gpio];
  // Add O_NONBLOCK to the file descriptor flags 
  flags = fcntl(context_handles[p_gpio]->types.digital.fd, F_GETFL, 0);
  if (fcntl(context_handles[p_gpio]->types.digital.fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    fprintf(stderr, "libhal_isr: fcntl call failed: %s", strerror(errno));
  }
  // TODO Block execution here 
  isr_fds[isr_fds_num].fd = context_handles[p_gpio]->types.digital.fd;
  isr_fds[isr_fds_num].events = POLLIN | POLLPRI | POLLHUP;
  isr_fds[isr_fds_num].revents = 0;
  isr_gpios[isr_fds_num] = p_gpio;
  isr_fds_num += 1;
  // Start thread
  if (isr_thread_id == -1) { // Create the ISR thread
    printf("libhal_isr: Create thread\n");
    pthread_create(&isr_thread_id, NULL, interrupt_handler, NULL);
  }
  // TODO Unblock execution here

  return 0;
} // End of function libhal_isr

void wait_ms(const uint32_t p_delay_ms)
{
  struct timespec sleeper, dummy;

  sleeper.tv_sec  = (time_t)(p_delay_ms / 1000);
  sleeper.tv_nsec = (uint64_t)(p_delay_ms % 1000) * 1000000;

  nanosleep(&sleeper, &dummy);
}

void delay_us_hardware(const uint32_t p_delay_ms) {
  struct timeval now, t_long, t_end;

  gettimeofday (&now, NULL);
  t_long.tv_sec  = p_delay_ms / 1000000;
  t_long.tv_usec = p_delay_ms % 1000000;
  timeradd (&now, &t_long, &t_end);

  while (timercmp(&now, &t_end, < )) {
    gettimeofday (&now, NULL);
  } // End of 'while' statement
}

void wait_us(const uint32_t p_delay_us) {
  struct timespec sleeper;
  uint32_t useconds = p_delay_us % 1000000;
  uint32_t wseconds = p_delay_us / 1000000;

  if (p_delay_us == 0) {
    return;
  } else if (p_delay_us  < 100) {
    delay_us_hardware(p_delay_us);
  } else {
      sleeper.tv_sec  = wseconds ;
      sleeper.tv_nsec = (long)(useconds * 1000L);
      nanosleep (&sleeper, NULL);
    }
}

void delay_us_hardware(const uint32_t p_delay_ms) {
  struct timeval now, t_long, t_end;

  gettimeofday (&now, NULL);
  t_long.tv_sec  = p_delay_ms / 1000000;
  t_long.tv_usec = p_delay_ms % 1000000;
  timeradd (&now, &t_long, &t_end);

  while (timercmp(&now, &t_end, < )) {
    gettimeofday (&now, NULL);
  } // End of 'while' statement
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
 * @fn void board_revision_failure(const char *p_message)
 * @brief Display fatal error when hardware revision of the board cannot not be retrieved
 *
 * @param p_message   Error message to be displayed
 */
static void board_revision_failure(const char *p_message)
{
  fprintf(stderr, "board_revision: Unable to determine board revision from /proc/cpuinfo\n");
  fprintf(stderr, " -> %s\n", p_message);
  fprintf(stderr, " ->  You may want to check:\n");
  fprintf(stderr, " ->  http://www.raspberrypi.org/phpBB3/viewtopic.php?p=184410#p184410\n");
  exit(EXIT_FAILURE);
}

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
  FILE *cpuFd = NULL;
  char *line = NULL;
  char *c, lastChar;
  static int32_t  board_revision = -1 ;

  line = (char *)malloc(128);
  memset((void *)line, 0x00, 128);
  if (board_revision != -1) {   // No point32_t checking twice
    free(line);
    return board_revision;
  }
  if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL) {
    board_revision_failure("Unable to open /proc/cpuinfo");
  }

  while (fgets(line, 127, cpuFd) != NULL) {
    if (strncmp(line, "Revision", 8) == 0) {
      break;
    }
  } // End of 'while' statement
  
  fclose(cpuFd);

  if (strncmp(line, "Revision", 8) != 0) {
    board_revision_failure("No \"Revision\" line");
  }
  
  for (c = &line [strlen (line) - 1]; (*c == '\n') || (*c == '\r'); --c) {
    *c = 0 ;
  } // End of 'for' statement
  
  for (c = line; *c; ++c) {
    if (isdigit(*c)) {
      break;
    }
  } // End of 'for' statement

  if (!isdigit(*c)) {
    board_revision_failure("No numeric revision string");
  }
  lastChar = line[strlen(line) - 1];

  /* if ((lastChar == '2') || (lastChar == '3'))
     board_revision = 1 ;
     else
     board_revision = 2 ;*/
  board_revision = 1;
  
  free(line);

  return board_revision;
}

int32_t libhal_setup_sys(void) {
  int32_t revision;
    
  // Sanity check
  if (isInitialised == TRUE) {
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
  
  isInitialised = TRUE;
  
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

int32_t create_new_context(const pin_name p_gpio, const gpio_access_t p_gpio_access, const gpioi_types_t p_gpio_type) {
  gpio_context *gpio;
  char buffer[MAX_BUFFER_SIZE];

  // Sanity checks
  if (p_gpio > MAX_GPIO_ID) {
    return -1;
  }

  if (context_handles[p_gpio] == NULL) {
    int32_t fd; // Used for export
    
    // Allocate value
    gpio = (gpio_context *)malloc(sizeof(gpio_context));
    gpio->gpio = p_gpio;
    gpio->shared = 0;
    // Export it 
    switch (p_gpio_type) {
    case gpioi_types_digital:
      gpio->type = gpioi_types_digital;
      gpio->types.digital.callback.callback_fn = NULL;
      gpio->types.digital.callback.callback_arg = NULL;
      gpio->types.digital.callback.ready = 0xff;
      sprintf(buffer, "%s/export", SYSFS_GPIO_DIR);
      if ((fd = open(buffer, O_SYNC | O_WRONLY)) < 0) {
	free(gpio);
	return -1;
      }
      sprintf(buffer, "%d", p_gpio);
      write(fd, buffer, MAX_BUFFER_SIZE);
      close(fd);
      // Open gpio/value file descriptor
      sprintf (buffer, "%s/gpio%d/value", SYSFS_GPIO_DIR, p_gpio);
      gpio->types.digital.fd = open(buffer, O_RDWR);
      sprintf (buffer, "%s/gpio%d/direction", SYSFS_GPIO_DIR, p_gpio);
      gpio->types.digital.fdd = open(buffer, O_RDWR);
      gpio->types.digital.fdm = -1;
      break;
    case gpioi_types_pwm:
      gpio->type = gpioi_types_pwm;
      gpio->types.pwm.pwmChip = pwm_chip_from_pin(p_gpio);
      sprintf(buffer, "%s%s/run", SYSFS_PWM_DIR, gpio->types.pwm.pwmChip);
      if ((gpio->types.pwm.fdr = open(buffer, O_SYNC | O_WRONLY)) < 0) {
	free(gpio);
	return -1;
      }
      write(gpio->types.pwm.fdr, "0", 1);
      sprintf(buffer, "%s%s/period", SYSFS_PWM_DIR, gpio->types.pwm.pwmChip);
      gpio->types.pwm.fdp = open(buffer, O_SYNC | O_WRONLY);
      sprintf(buffer, "%s%s/duty", SYSFS_PWM_DIR, gpio->types.pwm.pwmChip);
      gpio->types.pwm.fddc = open(buffer, O_SYNC | O_WRONLY);
      gpio->types.pwm.range = 256;
      gpio->types.pwm.clock = 1;
      pwm_update(gpio);
      break;
    case gpioi_types_adc:
      gpio->type = gpioi_types_adc;
      // Open gpio/value file descriptor
      sprintf (buffer, "%s/in_voltage%d_raw", SYSFS_ADC_DIR, gpio_to_adc(p_gpio));
      gpio->types.adc.fd = open(buffer, O_RDONLY);
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
    int32_t fd; // Used for unexport
    char buffer[MAX_BUFFER_SIZE];

    switch (gpio->type) {
    case gpioi_types_digital:
      // Close file descriptors
      close(gpio->types.digital.fd);
      close(gpio->types.digital.fdd);
      if (gpio->types.digital.fdm != -1) {
	close(gpio->types.digital.fdm);
      }
      if (gpio->types.digital.callback.ready != 0xff) {
	isr_fds[gpio->types.digital.callback.ready].fd = -1;
	gpio->types.digital.callback.ready = 0xff;
	isr_fds_num -= 1;
      }
      // Unexport the GPIO
      sprintf(buffer, "%s/unexport", SYSFS_GPIO_DIR);
      if ((fd = open(buffer, O_SYNC | O_WRONLY)) >= 0) {
	sprintf(buffer, "%d", p_gpio);
	write(fd, buffer, MAX_BUFFER_SIZE);
	close(fd);
      }
      break;
    case gpioi_types_pwm:
      // Disable the PWM
      lseek(gpio->types.pwm.fdr, 0L, SEEK_SET);
      write(gpio->types.pwm.fdr, "0", 1);
      // Close file descriptors
      close(gpio->types.pwm.fdr);
      close(gpio->types.pwm.fdp);
      close(gpio->types.pwm.fddc);
      break;
    case gpioi_types_adc:
      // Close file descriptors
      close(gpio->types.adc.fd);
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

const uint8_t gpio_to_adc(const pin_name p_gpio) { // TODO To be move into libhal_pin_name
  uint8_t adc_id = 0;
  switch (p_gpio) {
  case P9_33:
    adc_id = 4;
    break;
  case P9_35:
    adc_id = 6;
    break;
  case P9_36:
    adc_id = 5;
    break;
  case P9_37:
    adc_id = 2;
    break;
  case P9_38:
    adc_id = 3;
    break;
  case P9_40:
    adc_id = 1;
    break;
  default:
    break;
  } // End of 'switch' statement

  return adc_id;
}

void pwm_update(gpio_context *p_gpio) {
  char buffer[MAX_BUFFER_SIZE];

  // Set the period
  printf("pwm_update: Divisor=%d - Clock=%d\n", p_gpio->types.pwm.range, p_gpio->types.pwm.clock);
  p_gpio->types.pwm.period = 1e9 / (PWM_FIXED_FREQUENCY / p_gpio->types.pwm.clock / p_gpio->types.pwm.range);
  sprintf(buffer, "%d", (uint32_t)p_gpio->types.pwm.period);
  printf("pwm_update: Set period to %s\n", buffer);
  lseek(p_gpio->types.pwm.fdp, 0L, SEEK_SET);
  write(p_gpio->types.pwm.fdp, buffer, MAX_BUFFER_SIZE);
  /*sprintf(buffer, "echo %d > %s%s/period", (uint32_t)p_gpio->types.pwm.period, SYSFS_PWM_DIR, p_gpio->types.pwm.pwmChip);
    printf("pwm_update: call system with %s\n", buffer);
    system(buffer);*/
} // End of function pwm_update

const char * pwm_chip_from_pin(const pin_name p_gpio) { // TODO To be moved in pin_names.h
  printf("pwm_chip_from_pin: %d\n", p_gpio);
  switch (p_gpio) { // FIXME Check /sys/devices/ocp.3/pwm_test_Px.yy to check the correct extention
  case p3: // p9.21 - gpio0_03
    return NULL;
  case p2: // p9.22 - gpio0_02
    return NULL;
  case p22: // P8.19 - gpio0_22
    return "P8_19.13";
  case p23: // P8.13 - gpio0_23
    return NULL;
  case p50: // P9.14 - gpio1_18
    return "P9_14.12";
  case p51: // p9.16 - gpio1_19
    return "P9_16.16";
  default:
    return NULL;
  } // End of 'switch' statement

  return NULL;
} // End of function pwm_chip_from_pin

static void * interrupt_handler(void * p_arg) {
  int32_t result;

  for (;;) {
    if (isr_fds_num == 0) {
      printf("interrupt_handler: No GPIOs to poll: terminate thread\n");
      break;
    }
    result = poll(isr_fds, isr_fds_num + 1, POLL_TIMEOUT); 
    if (result == -1) {
      fprintf(stderr, "interrupt_handler: select call failed: %s\n", strerror(errno));
      break;
    } else if (result != 0) { // Check on which GPIO the interrupt occurs
      //      printf("interrupt_handler: Something happening: %d\n", result);
      int32_t i = 0;
      for ( ; i < isr_fds_num; i++) {
    if (isr_fds[i].revents & POLLIN) {
      uint8_t c;

      //      printf("interrupt_handler: isr on gpio %d\n", context_handles[isr_gpios[i]]->gpio);
      read(context_handles[isr_gpios[i]]->types.digital.fd, &c, 1); // Clear ISR
      isr_fds[i].revents = 0;
      context_handles[isr_gpios[i]]->types.digital.callback.callback_fn(context_handles[isr_gpios[i]]->types.digital.callback.callback_arg);      
    }
      } // End of 'for' statement
    } // No ISR, nothing to do
  } // End of 'for' statement

  memset((void *)&isr_fds, 0x00, sizeof(isr_fds));
  isr_fds_num = 0;
  pthread_exit(NULL); // Terminate thread
  return NULL ;
}

static void initialise_time(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  time_ms = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  time_us = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec);
}

