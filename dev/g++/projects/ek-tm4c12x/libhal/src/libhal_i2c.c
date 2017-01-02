/**
 * @file      libhal_i2c.c
 * @brief     Implementation file for the I2C Hardware Abstract Layer library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */

#include "libhal_i2c.h"

#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

uint32_t i2c_modules[1][7] = {
  { SYSCTL_PERIPH_I2C0, I2C0_BASE, GPIO_PB3_I2C0SDA, GPIO_PB2_I2C0SCL, GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_2 }
  // TODO support of multiple modules
};

int32_t libhal_i2c_setup(const uint8_t p_i2c_bus_id, const uint8_t p_device_address) {
  // TODO sanity checks
  // Enable the I2C0 peripheral
  SysCtlPeripheralEnable(i2c_modules[p_i2c_bus_id][0]);
  SysCtlPeripheralReset(i2c_modules[p_i2c_bus_id][0]);
  // Configure the pin muxing for I2C0 functions on port B2 and B3.
  GPIOPinConfigure(i2c_modules[p_i2c_bus_id][3]);
  GPIOPinConfigure(i2c_modules[p_i2c_bus_id][2]);
  // Select the I2C function for these pins.
  GPIOPinTypeI2CSCL(i2c_modules[p_i2c_bus_id][4], i2c_modules[p_i2c_bus_id][6]);
  GPIOPinTypeI2C(i2c_modules[p_i2c_bus_id][4], i2c_modules[p_i2c_bus_id][5]);
  // Wait for the I2C0 module to be ready
  while(!SysCtlPeripheralReady(i2c_modules[p_i2c_bus_id][0]));
  // Initialize Master and Slave
  I2CMasterInitExpClk(i2c_modules[p_i2c_bus_id][1], get_sys_clock_freq(), true);

  return p_i2c_bus_id;
}

int32_t libhal_i2c_setup_interface(const char *p_i2c_bus_id, const uint8_t p_device_address) {
  return -1;
}

int32_t libhal_i2c_read(const int32_t p_fd)
{
  return wiringPiI2CRead((int)p_fd);
}

int32_t libhal_i2c_read_register8(const int32_t p_fd, const uint8_t p_register) {
  return wiringPiI2CReadReg8((int)p_fd, (int)p_register);
}

int32_t libhal_i2c_read_register16(const int32_t p_fd, const uint8_t p_register) {
  return wiringPiI2CReadReg16((int)p_fd, (int)p_register);
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
  return wiringPiI2CWrite((int)p_fd, (int)p_data);
}

int32_t libhal_i2c_write_register8(const int32_t p_fd, const uint8_t p_register, const uint8_t p_value) {
  return wiringPiI2CWriteReg8((int)p_fd, (int)p_register, (int)p_value);
}

int32_t libhal_i2c_write_register16(const int32_t p_fd, const uint8_t p_register, const uint16_t p_value) {
  return wiringPiI2CWriteReg16((int)p_fd, (int)p_register, (int)p_value);
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
  SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C0);
  return 0;
}
