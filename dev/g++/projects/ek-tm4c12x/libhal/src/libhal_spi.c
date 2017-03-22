/**
 * @file      libhal_spi.c
 * @brief     Implementation file for the SPI Hardware Abstract Layer library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include "libhal_spi.h"

#include "inc/hw_ssi.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#define SPI_CHANNEL_MAX 3
/*!< SPI module parameters */
static uint32_t spi_modules[SPI_CHANNEL_MAX][11] = {
  { SYSCTL_PERIPH_SSI0, SSI0_BASE, GPIO_PA4_SSI0XDAT0, GPIO_PA5_SSI0XDAT1, GPIO_PA2_SSI0CLK, GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_5, GPIO_PORTA_BASE, GPIO_PIN_2, 0xFFFFFFFF },
  { SYSCTL_PERIPH_SSI1, SSI1_BASE, GPIO_PE4_SSI1XDAT0, GPIO_PE5_SSI1XDAT1, GPIO_PB5_SSI1CLK, GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_5, GPIO_PORTB_BASE, GPIO_PIN_5, 0xFFFFFFFF },
  { SYSCTL_PERIPH_SSI2, SSI2_BASE, GPIO_PD1_SSI2XDAT0, GPIO_PD0_SSI2XDAT1, GPIO_PD3_SSI2CLK, GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_1, GPIO_PORTD_BASE, GPIO_PIN_3, 0xFFFFFFFF }
  // TODO support of multiple modules
};

int32_t libhal_spi_setup(const uint8_t p_channel, const uint32_t p_speed) {
  // Sanity checks
  if (p_channel >= SPI_CHANNEL_MAX) {
    return -1;
  }
  if (spi_modules[p_channel][10] == p_channel) {
    return p_channel;
  }
  // Enable the SSIx peripheral
  SysCtlPeripheralEnable(spi_modules[p_channel][0]/*SYSCTL_PERIPH_SSI0*/); // FIXME Use flags instead of 0..7 integer values
  SysCtlPeripheralReset(spi_modules[p_channel][0]/*SYSCTL_PERIPH_SSI0*/);
  // Configure the pin muxing for I2C0 functions on port B2 and B3.
  GPIOPinConfigure(spi_modules[p_channel][2]/*GPIO_PA4_SSI0XDAT0*/); // SPI MOSI
  GPIOPinConfigure(spi_modules[p_channel][3]/*GPIO_PA5_SSI0XDAT1*/); // SPI MISO
  GPIOPinConfigure(spi_modules[p_channel][4]/*GPIO_PA2_SSI0CLK*/);   // SPI clock
  // Select the SSIx function for these pins.
  GPIOPinTypeSSI(spi_modules[p_channel][5]/*GPIO_PORTA_BASE*/, spi_modules[p_channel][6]/*GPIO_PIN_4*/);
  GPIOPinTypeSSI(spi_modules[p_channel][5]/*GPIO_PORTA_BASE*/, spi_modules[p_channel][7]/*GPIO_PIN_5*/);
  GPIOPinTypeSSI(spi_modules[p_channel][8]/*GPIO_PORTA_BASE*/, spi_modules[p_channel][9]/*GPIO_PIN_2*/);
  // Stop the Clock, Reset and Enable SSI Module in Master Function
  SysCtlPeripheralDisable(spi_modules[p_channel][0]/*SYSCTL_PERIPH_SSI0*/);
  SysCtlPeripheralReset(spi_modules[p_channel][0]/*SYSCTL_PERIPH_SSI0*/);
  SysCtlPeripheralEnable(spi_modules[p_channel][0]/*SYSCTL_PERIPH_SSI0*/);
  // Wait for the SSIx module to be ready
  while(!SysCtlPeripheralReady(spi_modules[p_channel][0]/*SYSCTL_PERIPH_SSI0*/));
  // Initialize SPI
  SSIConfigSetExpClk(spi_modules[p_channel][1]/*SSI0_BASE*/, get_sys_clock_freq(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, p_speed, 8);
  SSIEnable(spi_modules[p_channel][1]/*SSI0_BASE*/);
  // Set device address
  spi_modules[p_channel][10] = p_channel;
  return p_channel;
}

int32_t libhal_spi_get_fd(const uint8_t p_channel) {
  // TODO sanity checks
  return spi_modules[p_channel][10];
}

int32_t libhal_spi_data_read_write(const uint8_t p_channel, uint8_t *p_buffer, const uint32_t p_length) {
  // TODO sanity checks
  uint32_t data;
  uint8_t i = 0;
  int32_t result;
  // Write command
  for ( ; i < p_length; i += 1) {
    while(SSIBusy(spi_modules[p_channel][1]/*SSI0_BASE*/));
    data = *(p_buffer + i);
    SSIDataPut(spi_modules[p_channel][1]/*SSI0_BASE*/, data);
  } // End of 'for' statement
  // Read data
  i = 0;
  while(SSIBusy(spi_modules[p_channel][1]/*SSI0_BASE*/));
  result = SSIDataGetNonBlocking(spi_modules[p_channel][1]/*SSI0_BASE*/, &data);
  while (result != 0) {
    *(p_buffer + i++) = (uint8_t)data;
    while(SSIBusy(spi_modules[p_channel][1]/*SSI0_BASE*/));
    result = SSIDataGetNonBlocking(spi_modules[p_channel][1]/*SSI0_BASE*/, &data);
  } // End of 'while' statement

  return i;
}

int32_t libhal_spi_format(const uint8_t p_channel, const uint8_t p_transfer_word_speed, const uint8_t p_mode) {
  // TODO Sanity check
  SSIDisable(spi_modules[p_channel][1]/*SSI0_BASE*/);
  SSIConfigSetExpClk(spi_modules[p_channel][1]/*SSI0_BASE*/, get_sys_clock_freq(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, p_transfer_word_speed, 8);
  SSIEnable(spi_modules[p_channel][1]/*SSI0_BASE*/);
  return 0;
}

int32_t libhal_spi_close(const uint8_t p_channel) {
  // TODO Sanity check
  // Disable SSIx peripheral
  SSIDisable(spi_modules[p_channel][1]/*SSI0_BASE*/);
  SysCtlPeripheralDisable(spi_modules[p_channel][0]/*SYSCTL_PERIPH_SSI0*/);
  // Reset default slave address
  spi_modules[p_channel][10] = 0xFFFFFFFF;

  return 0;
}
