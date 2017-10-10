/**
 * @file    libhal_spi.c
 * @brief   Main implementation file for the Serial Hardware Abstract Layer library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libhal_serial.h"

int serial_open(const char *p_device, const int p_baud_rate) {
  struct termios options;
  speed_t baud_rate;
  int status, fd;

  switch (p_baud_rate) {
  case     50:	baud_rate =     B50; break;
  case     75:	baud_rate =     B75; break;
  case    110:	baud_rate =    B110; break;
  case    134:	baud_rate =    B134; break;
  case    150:	baud_rate =    B150; break;
  case    200:	baud_rate =    B200; break;
  case    300:	baud_rate =    B300; break;
  case    600:	baud_rate =    B600; break;
  case   1200:	baud_rate =   B1200; break;
  case   1800:	baud_rate =   B1800; break;
  case   2400:	baud_rate =   B2400; break;
  case   4800:	baud_rate =   B4800; break;
  case   9600:	baud_rate =   B9600; break;
  case  19200:	baud_rate =  B19200; break;
  case  38400:	baud_rate =  B38400; break;
  case  57600:	baud_rate =  B57600; break;
  case 115200:	baud_rate = B115200; break;
  case 230400:	baud_rate = B230400; break;
  default:
    return -2;
  } // End of 'switch' statement

  if ((fd = open(p_device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1) {
    return -1;
  }
  
  fcntl(fd, F_SETFL, O_RDWR);

  // Get and modify current options
  tcgetattr (fd, &options);

  cfmakeraw(&options);
  cfsetispeed(&options, baud_rate);
  cfsetospeed(&options, baud_rate);

  options.c_cflag |= (CLOCAL | CREAD);
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;

  options.c_cc[VMIN]  = 0;
  options.c_cc[VTIME] = 100; // Ten seconds (100 deciseconds)

  tcsetattr(fd, TCSANOW | TCSAFLUSH, &options);

  ioctl(fd, TIOCMGET, &status);

  status |= TIOCM_DTR;
  status |= TIOCM_RTS;

  ioctl(fd, TIOCMSET, &status);

  usleep(10000); // Wait for 10 ms

  return fd;
}

void serial_close(const int p_fd) {
  close(p_fd);
}

void serial_flush(const int p_fd) {
  tcflush(p_fd, TCIOFLUSH);
}

void serial_put_char (const int p_fd, const uint8_t p_char) {
  write(p_fd, &p_char, 1);
}

void serial_put_string(const int p_fd, const char *p_string) {
  // Sanity check
  if ((p_string == NULL) || (strlen(p_string) == 0)) {
    return;
  }
  
  write(p_fd, p_string, strlen(p_string));
}

void serial_printf(const int p_fd, const char *p_message, ...) {
  va_list argp;
  char *buffer = (char *)malloc(1024);

  va_start (argp, p_message);
  vsnprintf (buffer, 1023, p_message, argp);
  va_end (argp);

  serial_put_string(p_fd, buffer);
  free(buffer);
}

int serial_data_available(const int p_fd) {
  int result;

  if (ioctl(p_fd, FIONREAD, &result) == -1)
    return -1;

  return result;
}

int serial_get_char(const int p_fd) {
  uint8_t c;

  if (read(p_fd, &c, 1) != 1) {
    return -1;
  }

  return ((int)c) & 0xFF;
}
