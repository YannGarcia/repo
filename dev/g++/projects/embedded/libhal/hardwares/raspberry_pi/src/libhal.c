/*!
 * \file      libhal.c
 * \brief     Implementation file for the Hardware Abstract Layer library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <stdio.h>
#include <stdarg.h>
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <memory.h> */
/* #include <unistd.h> */
/* #include <ctype.h> */
#include <sys/time.h>
/* #include <sys/mman.h> */
/* #include <sys/stat.h> */
 #include <sys/wait.h>
/* #include <sys/ioctl.h> */
/* #include <poll.h> */
/* #include <fcntl.h> */
/* #include <pthread.h> */
/* #include <errno.h> */
#include <time.h>

#include "libhal.h"
#include "wiringPi.h"

#define PWM_FIXED_FREQUENCY ((float)100.0e6)            /*!< Default PWM ferequecy */

static uint64_t time_ms, time_us ;                      /*!< Time for easy calculations */

static void initialise_time(void);

int32_t libhal_setup(void)
{
  initialise_time();
  return wiringPiSetup();
}

void libhal_shutdown_sys(void) {
  // TODO Implement wiringPiShutdown
  return;
}

int32_t pin_mode(const pin_name p_gpio, const gpio_modes_t p_mode)
{
  // Sanity check
  pinMode((int)p_gpio, (int)p_mode);
  return 0;
} // End of function pin_mode

int32_t pull_up_dn_control(const pin_name p_gpio, const pud_t p_pud) {
  pullUpDnControl((int)p_gpio, (int)p_pud);
  return 0;
}

digital_state_t digital_read(const pin_name p_gpio) {
  int c = digitalRead((int)p_gpio);
  return (c == 0) ? digital_state_low : digital_state_high;
}

void digital_write(const pin_name p_gpio, const digital_state_t p_value) {
  digitalWrite((int)p_gpio, (p_value == digital_state_low) ? LOW : HIGH);  
  return;
}

void digital_write_byte(const uint8_t p_value) {
  digitalWriteByte((int)p_value);
}

int32_t pwm_write(const pin_name p_gpio, const uint32_t p_value) {
  pwmWrite((int)p_gpio, (int)p_value);
  return 0;
} // End of function pwm_write

void pwm_set_mode(const uint8_t  p_mode) {
  pwmSetMode((int)p_mode);
} // End of function pwm_set_mode

int32_t pwm_set_range(const pin_name p_gpio, const uint32_t p_range) {
  pwmSetRange((unsigned int)p_range);
  return 0;
} // End of function pwm_set_range

int32_t pwm_set_clock(const pin_name p_gpio, const uint32_t p_divisor) {
  pwmSetClock((int)p_divisor);  
  return 0;
} // End of function pwm_set_clock

float analog_read(const pin_name p_gpio) {
  return (float)analogRead((int)p_gpio);
} // End of function analog_read

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

static void initialise_time(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  time_ms = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  time_us = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec);
}

