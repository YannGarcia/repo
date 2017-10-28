/**
 * @file      libhal_spi.c
 * @brief     Implementation file for the SPI Hardware Abstract Layer library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include "libhal_spi.h"

#define SPI_CHANNEL_MAX 1
/*!< SPI module parameters */
static uint32_t spi_modules[SPI_CHANNEL_MAX][9] = {
  { EUSCI_B0_BASE, INT_EUSCIB0, EUSCI_B_SPI_RECEIVE_INTERRUPT, EUSCI_B_SPI_TRANSMIT_INTERRUPT, GPIO_PIN6, GPIO_PIN7, GPIO_PIN5, GPIO_PORT_P1, 0xFFFFFFFF }
  // TODO support of multiple modules
};

int32_t libhal_spi_setup(const uint8_t p_channel, const uint32_t p_speed) {
  // Sanity checks
  if (p_channel >= SPI_CHANNEL_MAX) {
    return -1;
  }
  if (spi_modules[p_channel][8] == p_channel) {
    return p_channel;
  }
  /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                                                 spi_modules[p_channel][8]/*GPIO_PIN_4*/,
                                                 spi_modules[p_channel][4]/*GPIO_PIN6*/ |
                                                 spi_modules[p_channel][5]/*GPIO_PIN7*/ |
                                                 spi_modules[p_channel][6]/*GPIO_PIN5*/,
                                                 GPIO_PRIMARY_MODULE_FUNCTION
                                                 );
  /* Configuring SPI in 3wire master mode */
  /* SPI Master Configuration Parameter */
  eUSCI_SPI_MasterConfig spiMasterConfig = {
    EUSCI_B_SPI_CLOCKSOURCE_SMCLK,                              // SMCLK Clock Source
    get_sys_clock_freq(),                                       // SMCLK = DCO
    p_speed,                                                    // SPICLK = 500khz
    EUSCI_B_SPI_MSB_FIRST,                                      // MSB First
    EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
    EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,                  // High polarity
    EUSCI_B_SPI_3PIN                                            // 3Wire SPI Mode
  };
  MAP_SPI_initMaster(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/, &spiMasterConfig);
  /* Enable SPI module */
  MAP_SPI_enableModule(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/);
  /* Enabling interrupts */
  MAP_SPI_enableInterrupt(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/, spi_modules[p_channel][3]/*EUSCI_B_SPI_RECEIVE_INTERRUPT*/);
  MAP_Interrupt_enableInterrupt(spi_modules[p_channel][1]/*INT_EUSCIB0*/);
  /* Set device address */
  spi_modules[p_channel][8] = p_channel;
  return p_channel;
}

int32_t libhal_spi_get_fd(const uint8_t p_channel) {
  // TODO sanity checks
  return spi_modules[p_channel][8];
}

int32_t libhal_spi_data_read_write(const uint8_t p_channel, uint8_t *p_buffer, const uint32_t p_length) {
  // TODO sanity checks
  uint8_t data;
  uint8_t i = 0;
  // Write command
  for ( ; i < p_length; i += 1) {
//    while(MAP_SPI_isBusy(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/));
    /* Polling to see if the TX buffer is ready */
    while (!(MAP_SPI_getInterruptStatus(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/, spi_modules[p_channel][3]/*EUSCI_B_SPI_RECEIVE_INTERRUPT*/)));
    data = *(p_buffer + i);
    MAP_SPI_transmitData(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/, data);
  } // End of 'for' statement
  // Read data
  while(MAP_SPI_isBusy(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/));
  i = 0;
  while (MAP_SPI_getInterruptStatus(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/, spi_modules[p_channel][4]/*EUSCI_B_SPI_TRANSMIT_INTERRUPT*/)) {
    MAP_SPI_clearInterruptFlag(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/, MAP_SPI_getEnabledInterruptStatus(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/));
    data = MAP_SPI_receiveData(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/);
    *(p_buffer + i++) = (uint8_t)data;
    while(MAP_SPI_isBusy(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/));
  } // End of 'while' statement
  return i;
}

int32_t libhal_spi_format(const uint8_t p_channel, const uint8_t p_transfer_word_speed, const uint8_t p_mode) {
  // TODO Sanity check
  // TODO
  return 1;
}

int32_t libhal_spi_close(const uint8_t p_channel) {
  // TODO Sanity check
  /* Enabling interrupts */
  MAP_SPI_disableInterrupt(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/, spi_modules[p_channel][3]/*EUSCI_B_SPI_RECEIVE_INTERRUPT*/);
  MAP_Interrupt_disableInterrupt(spi_modules[p_channel][1]/*INT_EUSCIB0*/);
  // Disable SSIx peripheral
  MAP_SPI_disableModule(spi_modules[p_channel][0]/*EUSCI_B0_BASE*/);
  // Reset default slave address
  spi_modules[p_channel][8] = (uint32_t)0xFFFFFFFF;

  return 0;
}
