/**
 * @file    libhal_i2c.c
 * @brief   Main implementation file for the I2C Hardware Abstract Layer library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h> // Use direct I2C driver
#include <linux/i2c-dev.h>

#include <unistd.h> // Used for close function

#include "libhal_i2c.h"

/** @union I2C ::ioctl data structure
 */
typedef union {
  uint8_t byte;
  uint16_t word;
  uint8_t block[I2C_SMBUS_BLOCK_MAX + 2];   /*!< block [0] is used for length + one more for PEC */
} i2c_smbus_data_t;

/** @union I2C ::ioctl data structure
 */
typedef struct {
  char read_write;
  uint8_t command;
  uint32_t size;
  i2c_smbus_data_t *data;
} i2c_smbus_ioctl_data_t;

/**
 * @fn int32_t i2c_smbus_access (const int32_t p_fd, const char p_rw, const uint8_t p_command, const int32_t p_size, union i2c_smbus_data_t *p_data)
 * @brief Write one byte from openend I2C bus
 * @param p_fd           The I2C file descriptor 
 * @param p_rw_command   The read/write command flag
 * @param p_command      The register to write. Default: 0
 * @param p_size         The ::IOCTL data size
 * @param p_data         The ::IOCTL data
 * @return 0 on success, -1 otherwise
 */
static int32_t i2c_smbus_access(const int32_t p_fd, const char p_rw_command, const uint8_t p_command, const uint32_t p_size, i2c_smbus_data_t *p_data)
{
  i2c_smbus_ioctl_data_t args;

  args.read_write = p_rw_command;
  args.command    = p_command;
  args.size       = p_size;
  args.data       = p_data;
  if (ioctl(p_fd, I2C_SMBUS, &args) == -1) {
    libhal_failure(HAL_ALMOST, "I2C_SMBUS ioctl failure: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}

int32_t libhal_i2c_setup(const uint8_t p_i2c_bus_id, const uint8_t p_device_address) {
  char device[32];

  // FIXME Check how to setup I2C device using DTO
  sprintf(device, "/dev/i2c-%1d", (char)p_i2c_bus_id);

  return libhal_i2c_setup_interface(device, p_device_address);
}

int32_t libhal_i2c_setup_interface(const char *p_i2c_bus_id, const uint8_t p_device_address) {
  int32_t fd;

  if ((fd = open(p_i2c_bus_id, O_RDWR)) == -1) {
    libhal_failure(HAL_ALMOST, "Unable to access I2C bus: %s\n", strerror(errno));
    return -1;
  }
  
  if (ioctl(fd, I2C_SLAVE, p_device_address) == -1) {
    libhal_failure(HAL_ALMOST, "Unable to select I2C device: %s\n", strerror(errno));
    return -1;
  }
  
  return fd;
}

int32_t libhal_i2c_read(const int32_t p_fd)
{
  i2c_smbus_data_t data;

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  if (i2c_smbus_access(p_fd, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data)) {
    return -1;
  }
  return data.byte & 0xFF;
}

int32_t libhal_i2c_read_register8(const int32_t p_fd, const uint8_t p_register) {
  i2c_smbus_data_t data;

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  if (i2c_smbus_access(p_fd, I2C_SMBUS_READ, p_register, I2C_SMBUS_BYTE_DATA, &data)) {
    return -1;
  }
  return data.byte & 0xFF;
}

int32_t libhal_i2c_read_register16(const int32_t p_fd, const uint8_t p_register) {
  i2c_smbus_data_t data;

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  if (i2c_smbus_access(p_fd, I2C_SMBUS_READ, p_register, I2C_SMBUS_WORD_DATA, &data)) {
    return -1;
  }

  return (((data.word >> 8) & 0x00ff) | ((data.word << 8) & 0xff00)) & 0xffff;
}

int32_t libhal_i2c_ext_read_register8(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register) {
  uint8_t buffer[2]; // Register address on 2 bytes
  struct i2c_rdwr_ioctl_data io;
  struct i2c_msg msgs[2];
  uint8_t value;

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  // 1.Prepare buffer
  buffer[0] = (uint8_t)(p_register >> 8);
  buffer[1] = (uint8_t)(p_register & 0xff);
  // 2. Prepare I2C start + memory address
  io.msgs = msgs;
  io.nmsgs = 2;
  msgs[0].addr = p_device_address;
  msgs[0].flags = 0; // Write
  msgs[0].len  = 2;
  msgs[0].buf = (uint8_t *)buffer; 
  // 3. Prepare read data + I2C stop
  msgs[1].addr = p_device_address;
  msgs[1].flags = I2C_M_RD; // Read
  msgs[1].len  = 1;
  msgs[1].buf = &value;
      
  if (ioctl(p_fd, I2C_RDWR, &io) == -1) {
    libhal_failure(HAL_ALMOST, "I2C_RDWR ioctl failure: %s\n", strerror(errno));
    return -1;
  }

  return value;
}

int32_t libhal_i2c_ext_read_register16(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register) {
  uint8_t buffer[2]; // Register address on 2 bytes
  struct i2c_rdwr_ioctl_data io;
  struct i2c_msg msgs[2];
  uint16_t value;

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  // 1.Prepare buffer
  buffer[0] = (uint8_t)(p_register >> 8);
  buffer[1] = (uint8_t)(p_register & 0xff);
  // 2. Prepare I2C start + memory address
  io.msgs = msgs;
  io.nmsgs = 2;
  msgs[0].addr = p_device_address;
  msgs[0].flags = 0;
  msgs[0].len  = 2;
  msgs[0].buf = (uint8_t *)buffer; 
  // 3. Prepare read data + I2C stop
  msgs[1].addr = p_device_address;
  msgs[1].flags = I2C_M_RD;
  msgs[1].len  = 2;
  msgs[1].buf = (uint8_t *)&value;
      
  if (ioctl(p_fd, I2C_RDWR, &io) == -1) {
    libhal_failure(HAL_ALMOST, "I2C_RDWR ioctl failure: %s\n", strerror(errno));
    return -1;
  }

  return (((value >> 8) & 0x00ff) | ((value << 8) & 0xff00)) & 0xffff;
}

int32_t libhal_i2c_write(const int32_t p_fd, int32_t p_data) {
  return i2c_smbus_access(p_fd, I2C_SMBUS_WRITE, p_data, I2C_SMBUS_BYTE, NULL);
}

int32_t libhal_i2c_write_register8(const int32_t p_fd, const uint8_t p_register, const uint8_t p_value) {
  i2c_smbus_data_t data;

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  data.byte = p_value;
  return i2c_smbus_access(p_fd, I2C_SMBUS_WRITE, p_register, I2C_SMBUS_BYTE_DATA, &data);
}

int32_t libhal_i2c_write_register16(const int32_t p_fd, const uint8_t p_register, const uint16_t p_value) {
  i2c_smbus_data_t data;

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  data.word = p_value;
  return i2c_smbus_access(p_fd, I2C_SMBUS_WRITE, p_register, I2C_SMBUS_WORD_DATA, &data);
}

int32_t libhal_i2c_ext_write_register8(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register, const uint8_t p_value) {
  uint8_t buffer[3]; // Register address on 2 bytes + one byte of data
  struct i2c_rdwr_ioctl_data io;
  struct i2c_msg msgs[1];

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  // 1.Prepare buffer
  buffer[0] = (uint8_t)(p_register >> 8);
  buffer[1] = (uint8_t)(p_register & 0xff);
  buffer[2] = p_value;
  // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
  io.msgs = msgs;
  io.nmsgs = 1;
  msgs[0].addr = p_device_address;
  msgs[0].flags = 0;
  msgs[0].len  = 3;
  msgs[0].buf = (uint8_t *)buffer; 
  if (ioctl(p_fd, I2C_RDWR, &io) == -1) {
    libhal_failure(HAL_ALMOST, "I2C_RDWR ioctl failure: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}

int32_t libhal_i2c_ext_write_register16(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register, const uint16_t p_value) {
  uint8_t buffer[4]; // Register address on 2 bytes + two bytes of data
  struct i2c_rdwr_ioctl_data io;
  struct i2c_msg msgs[1];

  // Sanity check
  if (p_fd == -1) {
    return -1;
  }
  
  // 1.Prepare buffer
  buffer[0] = (uint8_t)(p_register >> 8);
  buffer[1] = (uint8_t)(p_register & 0xff);
  buffer[2] = (uint8_t)(p_value >> 8);
  buffer[3] = (uint8_t)(p_value & 0xff);
  // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
  io.msgs = msgs;
  io.nmsgs = 1;
  msgs[0].addr = p_device_address;
  msgs[0].flags = 0;
  msgs[0].len  = 4;
  msgs[0].buf = (uint8_t *)buffer; 
  if (ioctl(p_fd, I2C_RDWR, &io) == -1) {
    libhal_failure(HAL_ALMOST, "I2C_RDWR ioctl failure: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}

int32_t libhal_i2c_close(const int32_t p_fd) {
  // Sanity check
  if (p_fd == -1) {
    return -1;
  }

  return close(p_fd);
}
