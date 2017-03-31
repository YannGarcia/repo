/**
 * @file      main.c
 * @brief     Application to validate libhal libary
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2016-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   1.0
 * @see       TivaWare™ Peripheral Driver Library USER’S GUIDE - SW-TM4C-DRL-UG-2.1.3.156
 * @see       EK-TM4C129EXL Firmware Development Package User's Guide
 * @see       EK-TM4C1294XL Firmware Development Package User's Guide
 */

#include "libhal.h" /* Hardware Abstraction Level library (Beagle Bone, Raspbery MBED & TI launchpads */
#include "libhal_serial.h"
#include "libhal_i2c.h"
#include "libhal_spi.h"

/*!<
 * @defgroup Booststarter #1 pin assignment
 * @{
 */
#define LED3       p108      /*!< On board LED1, used for debug - MSB 4 bytes state machine status */
#define LED2       p107      /*!< On board LED2, used for debug */
#define LED1       p46       /*!< On board LED3, used for debug */
#define LED0       p42       /*!< On board LED4, used for debug - LSB 4 bytes state machine status */
#define SW1        p116      /*!< On board Switch1 */
#define SW2        p117      /*!< On board Switch2 */

#define L_RED      p44
#define L_GREEN    p45
#define L_BLUE     p49

#define LIGHT_INT  p22 /*!< TI OPT3001 Light Sensor on BOOSTXL-EDUMKII Educational BoosterPack™ Mark II Plug-in Module See SLAU599–August 2015 Clause 2.1.2 TI OPT3001 Light Sensor */

// TODO Checked pin values
#define BUT1       p1 /*!< On board Switch1 Switch1 */
#define BUT2       p1 /*!< On board Switch1 Switch2 */

#define J_SW       p23 /*!< Select button - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */
#define J_X        A9  /*!< Horizontal X-axis - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */
#define J_Y        A0  /*!< Horizontal Y-axis - See Joystick reference: https://www.itead.cc/playstation2-analog-joystick.html */

#define A_X        A3  /*!< Accelerometer, X-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */
#define A_Y        A2  /*!< Accelerometer, Y-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */
#define A_Z        A1  /*!< Accelerometer, Z-axis - See KXTC9-2050 reference: http://www.kionix.com/product/KXTC9-2050 */

#define TFT_CS     p109 /*!< TFT /CS - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_MOSI   p2   /*!< TFT SPI MOSI - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_CLK    p4   /*!< TFT SPI clock - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_RST    p32  /*!< TFT reset - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_RS     p84  /*!< TFT command indicator - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_WIDTH  128  /*!< TFT screen width in pixels - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
#define TFT_HEIGHT 128  /*!< TFT screen heigh in pixels - See CFAF128128B-0145T color 128x128-pixel TFT LCD reference: https://www.crystalfontz.com/product/cfaf128128b0145t-graphical-tft-128x128-lcd-display-module */
/*!<
 * @defgroup TFT screen color descriptions
 * @{
 */
#define TFT_BLACK   0x0000
#define TFT_BLUE    0x001F
#define TFT_RED     0xF800
#define TFT_GREEN   0x07E0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0
#define TFT_WHITE   0xFFFF
/*!< @} */

/*!< @} */

/*!<
 * @fn void set_debug_state(const uint8_t p_state)
 * @brief Display the current debug state using the four on-board LEDs
 * @param[in] p_state The state value to display (between 0b0000 to 0b1111)
 */
void set_debug_state(const uint8_t p_state);

/*!<
 * @brief Main function of the bouncing robot software application
 * @return Shall never returned
 */
int32_t main(void) {
  uint8_t debug_state = 0; /*!< Debug state status */
  int32_t uart0;           /*!< UART handle */
  int32_t i2c0;            /*!< I2C bus 0 handle */
  uint32_t counter = 0;    /*!< Counter */
  uint8_t spi_cmd_rsp;     /*!< 8bit SPI command/response buffer */
  int8_t float2str[8];

  /* Initialise the HAL */
  libhal_setup();

//  digital_write(L_GREEN, digital_state_high);
  /* State 0: display 0b1111 then 0x0000 */
  set_debug_state(debug_state);

  /* Setup GPIO input ports */
  pin_mode(SW1, gpio_modes_digital_input);
  pull_up_dn_control(SW1, pud_up);         /* Configuration of the pull-up is required for USR_SW1 */
  pin_mode(SW2, gpio_modes_digital_input);
  pull_up_dn_control(SW2, pud_up);         /* Configuration of the pull-up is required for USR_SW2 */

  /* Setup GPIO input ports for MKII buttons, see MKII Circuit Diagram */
  pin_mode(BUT1, gpio_modes_digital_input);
  pin_mode(BUT2, gpio_modes_digital_input);

  // Open serial console
  uart0 = serial_open("/dev/tty0", 115200);
  debug_state += 1;
  set_debug_state(debug_state);

  // Setup Joystick
  pin_mode(J_SW, gpio_modes_digital_input);
  pin_mode(J_X, gpio_modes_adc_input);
  pin_mode(J_Y, gpio_modes_adc_input);
  float j_x = analog_read(J_X);
  float j_y = analog_read(J_Y);

  // Setup accelerator
  const pin_name acc_pins[3] = {A_X, A_Y, A_Z};
  float values[3] = {0};
  pins_mode(acc_pins, 3, gpio_modes_adc_input);
  analog_multiple_read(acc_pins, 3, values);
  debug_state += 1;
  set_debug_state(debug_state);

  // Setup I2C bus 0 to access TI OPT3001 Light Sensor
//  digital_write(L_BLUE, digital_state_high);
  pin_mode(LIGHT_INT, gpio_modes_digital_input);
  i2c0 = libhal_i2c_setup(0/*"/dev/i2c0"*/, 0x44); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.3.4.1 Serial Bus Address
  // Read Manufacturer ID
  uint16_t data = libhal_i2c_read_register16(i2c0, 0x7E); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  // Read Device ID
  data = libhal_i2c_read_register16(i2c0, 0x7F); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  wait_us(1);
  // Read config
  data = libhal_i2c_read_register16(i2c0, 0x01); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  wait_us(1);
  // Read Low Limit
  data = libhal_i2c_read_register16(i2c0, 0x02); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  wait_us(1);
  // Read High Limit
  data = libhal_i2c_read_register16(i2c0, 0x03); // See SBOS681B –JULY 2014–REVISED DECEMBER 2014 Clause 7.6.1 Internal Registers
  wait_us(1);
  debug_state += 1;
  set_debug_state(debug_state);

  // Setup TFT screen
  /*digital_write(L_RED, digital_state_high); // TODO Check why it does not work!?
  pin_mode(TFT_RS, gpio_modes_digital_output); // TFT /RS pin, command indicator
  digital_write(TFT_RS, digital_state_low); // Send command mode
  pin_mode(TFT_RST, gpio_modes_digital_output); // TFT /RESET pin
  digital_write(TFT_RST, digital_state_high);
  pin_mode(TFT_CS, gpio_modes_digital_output); // SPI /CS
  digital_write(TFT_CS, digital_state_high);
  int32_t tft_hd = libhal_spi_setup(2, 1000000);
  // Init TFT
  {
    // Reset the LCD controller
    digital_write(TFT_RST, digital_state_low);
    wait_ms(5); // 10uS min
    // Enable the TFT
    digital_write(TFT_RST, digital_state_high);
    wait_ms(150); // 120ms max

    // SPI Enable TFT
    digital_write(TFT_CS, digital_state_low);

    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x11; // Sleep out and Charge Pump on
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(500);
    // Unset RS to send command
    // Already done - digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xB1; // Frame rate ctrl - normal mode
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x01;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2c;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2d;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    // Already done - digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xB2; // Frame rate control - idle mode
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x01;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2c;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2d;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    // Already done - digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xB3; // Frame rate ctrl - partial mode
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x01;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2c;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2d;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x01;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2c;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2d;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xB4; // Display inversion ctrl
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x07;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xC0; // Power control
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0xa2;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x02;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x84;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xC1; // Power control
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0xc5;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xC2; // Power control
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x0a;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xC3; // Power control
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x8a;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x2a;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xC4; // Power control
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x8a;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0xee;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0xC5; // Power control
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x0e;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0xee;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x20; // Don't invert display
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x36; // Memory access control (directions)
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0xc8;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x2A; // Column addr set
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = TFT_WIDTH;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x2B; // Row address set
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = 0x00;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    spi_cmd_rsp = TFT_HEIGHT;
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(1);
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x13; // Normal display on
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(10);
    // Unset RS to send command
    // Already done-digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x29; // Main screen turn on
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    wait_ms(100);
    // Disable the TFT
    digital_write(TFT_CS, digital_state_high);
  }
  // Fill screen in black
  uint8_t x_cursor = 0, y_cursor = 0;
  {
    uint16_t color = TFT_GREEN;
    uint8_t hi = color >> 8, lo = color;
    // Enable the TFT
    digital_write(TFT_CS, digital_state_low);
    // Set RAM area to be modified: (x_cursor, y_cursor) to (x_cursor + TFT_WIDTH - 1, y_cursor + TFT_HEIGHT - 1)
    {
        // Unset RS to send command
        digital_write(TFT_RS, digital_state_low);
        spi_cmd_rsp = 0x2A; // Column address set
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        // Set RS to send datum
        digital_write(TFT_RS, digital_state_high);
        spi_cmd_rsp = 0x00;
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        spi_cmd_rsp = 0x00; // Column index start + shift: x_cursor + shift = 0
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        spi_cmd_rsp = 0x00;
        digital_write(TFT_RS, digital_state_high);
        spi_cmd_rsp = TFT_WIDTH - 1; // Column index end: x_cursor + TFT_WIDTH - 1 + shift = 0
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        // Unset RS to send command
        digital_write(TFT_RS, digital_state_low);
        spi_cmd_rsp = 0x2B; // Row address set
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        // Set RS to send datum
        digital_write(TFT_RS, digital_state_high);
        spi_cmd_rsp = 0x00;
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        spi_cmd_rsp = 0x00; // Row index start + shift: y_cursor + shift = 0
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        spi_cmd_rsp = 0x00;
        digital_write(TFT_RS, digital_state_high);
        spi_cmd_rsp = TFT_HEIGHT - 1; // Row index end: y_cursor + TFT_HEIGHT - 1 + shift = 0
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    }
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    spi_cmd_rsp = 0x2C; // Write to RAM
    libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
    // Set RS to send datum
    digital_write(TFT_RS, digital_state_high);
    for(y_cursor = TFT_HEIGHT; y_cursor > 0; y_cursor--) {
      for(x_cursor = TFT_WIDTH; x_cursor > 0; x_cursor--) {
        spi_cmd_rsp = hi;
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
        spi_cmd_rsp = lo;
        libhal_spi_data_read_write(tft_hd, &spi_cmd_rsp, 1);
      } // End of 'for' statement
    } // End of 'for' statement
    // Unset RS to send command
    digital_write(TFT_RS, digital_state_low);
    // Disable the TFT
    digital_write(TFT_CS, digital_state_high);
  }*/

  while (true) {
    serial_printf(uart0, "Please push User button #2 (%d)\r\n", counter++);

    /* Wait event on Switch2 - PullUp ==> pushed = digital_state_low */
    while (digital_read(SW2) == digital_state_high); // TODO To be checked due to change in libhal
    /* wait for 170 ms for button debouncing */
    wait_ms(170);

    /* Increment debug_state */
    debug_state = (debug_state + 1) % 8;

    /* Change debug state */
    set_debug_state(debug_state);

//    digital_toggle(L_BLUE); // Marker between two I2C transactions
    // Read Result register
    data = libhal_i2c_read_register8(i2c0, 0x00);
//    digital_toggle(L_BLUE); // Marker between two I2C transactions
    serial_printf(uart0, "Light sensor value: %d\r\n", data);

    // Joystick position
    j_x = analog_read(J_X);
    j_y = analog_read(J_Y);
    ftoa(j_x, float2str, 5);
    serial_printf(uart0, "Joystick position: (%s, ", float2str);
    ftoa(j_y, float2str, 5);
    serial_printf(uart0, "%s)\r\n", float2str, 5);
    // Accelerator values
    analog_multiple_read(acc_pins, 3, values);
    ftoa(values[0], float2str, 5);
    serial_printf(uart0, "Accelerator (X, Y, Z): (%s, ", float2str, 5);
    ftoa(values[1], float2str, 5);
    serial_printf(uart0, "%s, ", float2str, 5);
    ftoa(values[2], float2str, 5);
    serial_printf(uart0, "%s)\r\n", float2str, 5);
  } /* End of 'while' statement */

  return 0;
} /* End of main function*/

void set_debug_state(const uint8_t p_state) {
  switch (p_state) {
  case 0x00:
    digital_write(LED3, digital_state_high);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
    wait_ms(50);
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_low);
    break;
  case 0x01:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_high);
    break;
  case 0x02:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_low);
    break;
  case 0x03:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
    break;
  case 0x04:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_low);
    break;
  case 0x05:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_high);
    break;
  case 0x06:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_low);
    break;
  case 0x07:
    digital_write(LED3, digital_state_low);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
    break;
  case 0x08:
    digital_write(LED3, digital_state_high);
    digital_write(LED2, digital_state_low);
    digital_write(LED1, digital_state_low);
    digital_write(LED0, digital_state_low);
    break;
  default:
    digital_write(LED3, digital_state_high);
    digital_write(LED2, digital_state_high);
    digital_write(LED1, digital_state_high);
    digital_write(LED0, digital_state_high);
  } /* End of 'switch' statement */

} /* End of function set_debug_state */
