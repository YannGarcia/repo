/**
 * @file      libhal_i2c.c
 * @brief     Implementation file for the I2C Hardware Abstract Layer library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
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

#define I2C_MODULE_MAX 1
/*!< I2C module parameters */
static uint32_t i2c_modules[I2C_MODULE_MAX][8] = {
  { SYSCTL_PERIPH_I2C0, I2C0_BASE, GPIO_PB3_I2C0SDA, GPIO_PB2_I2C0SCL, GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_2, 0xFFFFFFFF }
  // TODO support of multiple modules
};

int32_t libhal_i2c_setup(const uint8_t p_i2c_bus_id, const uint8_t p_device_address) {
  // Sanity checks
  uint8_t fd = p_i2c_bus_id;
  if (fd >= I2C_MODULE_MAX) {
    return -1;
  }
  if (i2c_modules[fd][7] != 0xFFFFFFFF) {
    return fd;
  }
  // Enable the I2Cx peripheral
  SysCtlPeripheralEnable(i2c_modules[fd][0]/*SYSCTL_PERIPH_I2C0*/); // FIXME Use flags instead of 0..7 integer values
  SysCtlPeripheralReset(i2c_modules[fd][0]/*SYSCTL_PERIPH_I2C0*/);
  // Configure the pin muxing for I2Cx functions on port B2 and B3.
  GPIOPinConfigure(i2c_modules[fd][3]/*GPIO_PB2_I2C0SCL*/);
  GPIOPinConfigure(i2c_modules[fd][2]/*GPIO_PB3_I2C0SDA*/);
  // Select the I2C function for these pins.
  GPIOPinTypeI2CSCL(i2c_modules[fd][4]/*GPIO_PORTB_BASE*/, i2c_modules[fd][6]/*GPIO_PIN_2*/);
  GPIOPinTypeI2C(i2c_modules[fd][4]/*GPIO_PORTB_BASE*/, i2c_modules[fd][5]/*GPIO_PIN_3*/);
  // Stop the Clock, Reset and Enable I2C Module in Master Function
  SysCtlPeripheralDisable(i2c_modules[fd][0]/*SYSCTL_PERIPH_I2C0*/);
  SysCtlPeripheralReset(i2c_modules[fd][0]/*SYSCTL_PERIPH_I2C0*/);
  SysCtlPeripheralEnable(i2c_modules[fd][0]/*SYSCTL_PERIPH_I2C0*/);
  // Wait for the I2Cx module to be ready
  while(!SysCtlPeripheralReady(i2c_modules[fd][0]/*SYSCTL_PERIPH_I2C0*/));
  // Initialize Master and Slave
  I2CMasterInitExpClk(i2c_modules[fd][1]/*I2C0_BASE*/, get_sys_clock_freq(), true/*Fast mode: 0.4KHz*/);
  I2CMasterGlitchFilterConfigSet(i2c_modules[fd][1]/*I2C0_BASE*/, I2C_MASTER_GLITCH_FILTER_8);
  // Set device address
  i2c_modules[fd][7] = p_device_address;
  // Clear I2C FIFOs
  HWREG(i2c_modules[fd][1]/*I2C0_BASE*/ + I2C_O_FIFOCTL) = 80008000;

  return fd;
}

int32_t libhal_2c_setup_interface(const char *p_i2c_bus_id, const uint8_t p_device_address) {
  //FIXME To be completed
  return libhal_i2c_setup(0, p_device_address);
}

int32_t libhal_i2c_read(const int32_t p_fd) {
  // TODO sanity checks
  // Check if the Bus is busy
  while(I2CMasterBusBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for read
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], true/*Read operation*/);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_SINGLE_RECEIVE);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Read data from slave device
  return (uint8_t)I2CMasterDataGet(i2c_modules[p_fd][1]/*I2C0_BASE*/);
}

int32_t libhal_i2c_read_register8(const int32_t p_fd, const uint8_t p_register) {
  // TODO sanity checks
  // Check if the Bus is busy
  while(I2CMasterBusBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for writing the register address
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], false/*Write operation*/);
  // Send the register address
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, p_register);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_SINGLE_SEND);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for read
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], true/*Read operation*/);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_SINGLE_RECEIVE);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Read data from slave device
  return (uint8_t)I2CMasterDataGet(i2c_modules[p_fd][1]/*I2C0_BASE*/);
}

int32_t libhal_i2c_read_register16(const int32_t p_fd, const uint8_t p_register) {
  uint16_t data = 0;
  // TODO sanity checks
  // Check if the Bus is busy
  while(I2CMasterBusBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for writing the register address
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], false/*Write operation*/);
  // Send the register address
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, p_register);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_SINGLE_SEND);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for read
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], true/*Read operation*/); // Repeat start
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_BURST_RECEIVE_START);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Read data from slave device
  data = (uint8_t)I2CMasterDataGet(i2c_modules[p_fd][1]/*I2C0_BASE*/) << 8;
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Read data from slave device
  data |= (uint8_t)I2CMasterDataGet(i2c_modules[p_fd][1]/*I2C0_BASE*/);
  return data;
}

int32_t libhal_i2c_ext_read_register8(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register) {
  // TODO sanity checks
  uint32_t data;
  uint32_t address = i2c_modules[p_fd][7];
  i2c_modules[p_fd][7] = p_device_address;
  data = libhal_i2c_read_register8(p_fd, p_register);
  i2c_modules[p_fd][7] = address;
  return data;
}

int32_t libhal_i2c_ext_read_register16(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register) {
    // TODO sanity checks
  uint32_t data;
  uint32_t address = i2c_modules[p_fd][7];
  i2c_modules[p_fd][7] = p_device_address;
  data = libhal_i2c_read_register16(p_fd, p_register);
  i2c_modules[p_fd][7] = address;
  return data;
}

int32_t libhal_i2c_write(const int32_t p_fd, int32_t p_data) {
  // TODO sanity checks
  // Check if the Bus is busy
  while(I2CMasterBusBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for read
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], false/*Write operation*/);
  // Place the data to be sent in the data register
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)p_data);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_SINGLE_SEND);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  return 0;
}

int32_t libhal_i2c_write_register8(const int32_t p_fd, const uint8_t p_register, const uint8_t p_value) {
  // TODO sanity checks
  // Check if the Bus is busy
  while(I2CMasterBusBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for writing the register address
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], false/*Write operation*/);
  // Send the register address
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, p_register);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_BURST_SEND_START);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Send the value
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, p_value);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_BURST_SEND_FINISH);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));

  return 0;
}

int32_t libhal_i2c_write_register16(const int32_t p_fd, const uint8_t p_register, const uint16_t p_value) {
  // TODO sanity checks
  // Check if the Bus is busy
  while(I2CMasterBusBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for writing the register address
  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], false/*Write operation*/);
  // Send the register address
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, p_register);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_BURST_SEND_START);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for read
//  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], false/*Write operation*/);
  // Send the value
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)(p_value >> 8));
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_BURST_SEND_CONT);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));
  // Sets the address that the I2C Master places on the bus for read
//  I2CMasterSlaveAddrSet(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)i2c_modules[p_fd][7], false/*Write operation*/);
  // Send the value
  I2CMasterDataPut(i2c_modules[p_fd][1]/*I2C0_BASE*/, (uint8_t)p_value);
  // Send control byte
  I2CMasterControl(i2c_modules[p_fd][1]/*I2C0_BASE*/, I2C_MASTER_CMD_BURST_SEND_FINISH);
  // Wait for MCU to complete receive transaction
  while(I2CMasterBusy(i2c_modules[p_fd][1]/*I2C0_BASE*/));

  return 0;
}

int32_t libhal_i2c_ext_write_register8(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register, const uint8_t p_value) {
  // TODO sanity checks
  uint32_t result;
  uint32_t address = i2c_modules[p_fd][7];
  i2c_modules[p_fd][7] = p_device_address;
  result = libhal_i2c_write_register8(p_fd, p_register, p_value);
  i2c_modules[p_fd][7] = address;
  return result;
}

int32_t libhal_i2c_ext_write_register16(const int32_t p_fd, const uint8_t p_device_address, const uint16_t p_register, const uint16_t p_value) {
  // TODO sanity checks
  uint32_t result;
  uint32_t address = i2c_modules[p_fd][7];
  i2c_modules[p_fd][7] = p_device_address;
  result = libhal_i2c_write_register16(p_fd, p_register, p_value);
  i2c_modules[p_fd][7] = address;
  return result;
}

int32_t libhal_i2c_close(const int32_t p_fd) {
  // Sanity check
  if (p_fd == -1) {
    return -1;
  }
  // Disable I2C peripheral
  SysCtlPeripheralDisable(i2c_modules[p_fd][0]/*SYSCTL_PERIPH_I2C0*/);
  // Reset default slave address
  i2c_modules[p_fd][7] = 0xFFFFFFFF;

  return 0;
}
