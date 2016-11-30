#pragma once

// MBED port definitions

// I2C ports
#define I2C_SDA_PIN         p9
#define I2C_SDL_PIN         p10
// I2C addresses
#define I2C_RTCC_ADDRESS    0xd0
#define I2C_EEPROM_ADDRESS  0x01
#define I2C_GPIO_ADDRESS    0x27
#define I2C_TEMP_ADDRESS    0xd0

// SPI ports
#define SPI_MOSI_PIN        p5
#define SPI_MISO_PIN        p6
#define SPI_SCK_PIN         p7

// RTC
#define SQWOUT      p8

// TEMPERATURE
#define TEM_ALERT   p13

// GPIO EXPENDER
#define IOEXP_INTA  p15
#define IOEXP_INTB  p16
#define IOEXP_RESET p17
#define GPA0    0
#define GPA1    1
#define GPA2    2
#define GPA3    3
#define GPA4    4
#define GPA5    5
#define GPA6    6
#define GPA7    7
#define GPB0    8
#define GPB1    9
#define GPB2    10
#define GPB3    11
#define GPB4    12
#define GPB5    13
#define GPB6    14
#define GPB7    15

// GPS
#define GPS_RX      p13
#define GPS_TX      p14
#define GPS_PPP     p12

// EV boards
#define SIPO_DATA   p30
#define SIPO_CLOCK  p29

// Power led
#define POWER_LED   LED1

// GPIO Expender port definitions
// MUX
#define MUX_ADDR0   GPB3
#define MUX_ADDR1   GPB4
#define MUX_ADDR2   GPB5
#define MUX_EN      GPB1
#define EN_BOARDS   GPB8
// SPI Enables
#define CS_MCP32    GPA2
#define CS_MCP42    GPA1
#define SH_MCP42    GPA0

