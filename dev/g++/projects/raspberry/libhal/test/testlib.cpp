/**
 * @file    testlib.cpp
 * @brief   Hardware Abstract Library test suite.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cmath> // Used for abs

#include <cpptest.h>

#include "libhal_factory.hpp"

#include "libhal_serial.h"
#include "libhal_shift.h"
#include "libhal_spi.h"
#include "libhal_i2c.h"

using namespace std;

/**
 * @class Libhal test suite implementation
 * @brief To run these tests, you need to connect a data logger to the GPIOs. 
 * @remark Refer to the DesignSpark PCP schematic of this project
 * @see https://github.com/beagleboard/devicetree-source/tree/master/arch/arm/boot/dts
 */
class libhal_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  libhal_test_suite() {
    // TEST_ADD(libhal_test_suite::test_delay_1);
    // TEST_ADD(libhal_test_suite::test_led_1);
    // TEST_ADD(libhal_test_suite::test_led_2);
    // TEST_ADD(libhal_test_suite::test_digital_out_1);
    // TEST_ADD(libhal_test_suite::test_digital_out_2);
    // TEST_ADD(libhal_test_suite::test_uart4_gps);
    // TEST_ADD(libhal_test_suite::test_bits_shift_1);
    // TEST_ADD(libhal_test_suite::test_bits_shift_2);
    // TEST_ADD(libhal_test_suite::test_adc_1);
    // TEST_ADD(libhal_test_suite::test_spi_1);
    // TEST_ADD(libhal_test_suite::test_spi_2);
    // TEST_ADD(libhal_test_suite::test_spi_3);
    // TEST_ADD(libhal_test_suite::test_spi_4);
    // TEST_ADD(libhal_test_suite::test_spi_5);
    // TEST_ADD(libhal_test_suite::test_spi_6);
    // TEST_ADD(libhal_test_suite::test_i2c_1);
    // TEST_ADD(libhal_test_suite::test_i2c_2);
    // TEST_ADD(libhal_test_suite::test_i2c_3);
    // TEST_ADD(libhal_test_suite::test_i2c_4);
    // TEST_ADD(libhal_test_suite::test_i2c_5);
    // TEST_ADD(libhal_test_suite::test_factory_1);
    TEST_ADD(libhal_test_suite::test_factory_2);
  }
	
private:
  void test_delay_1() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::reset_time();
    ::wait_us(150);
    TEST_ASSERT_DELTA_MSG(150, ::elapse_us(), 10, "test_delay_1 failure, Unexpected range value");
    ::reset_time();
    ::wait_ms(5);
    TEST_ASSERT_DELTA_MSG(5, ::elapse_ms(), 1, "test_delay_1 failure, Unexpected range value");
    // Postambule
    ::libhal_shutdown_sys();
  }

  /**
   * @brief Test case for @see libhal::digital_write
   * @see libhal_setup_sys
   * @see libhal_shutdown_sys
   * @see digital_write/digital_state_high
   * @see delay_ms
   */
  void test_led_1() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p49, digital_state_high); // P9_23 - PIN:17 - GPIO:p49 => GPIO1_17 - GPIO[chip]_[pin]: Linux pin number = [chip] * 32 + [pin]
    for (int i = 0; i < 5; i++) {
      ::wait_ms(100);
    } // End of 'for' statement
    std::clog << std::endl;
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT_MSG(true, "Led red was on");
  } // End of test_led_1
  
  /**
   * @brief Test case for @see libhal::digital_write
   * @see digital_write/digital_state_high
   * @see digital_write/digital_state_low
   */
  void test_led_2() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p49, digital_state_high); // P9_23 - PIN:17 - GPIO:p49 => GPIO1_17 - GPIO[chip]_[pin]: Linux pin number = [chip] * 32 + [pin]
    for (int i = 0; i < 5; i++) {
      ::wait_ms(100);
    } // End of 'for' statement
    std::clog << std::endl;
    ::digital_write(p49, digital_state_low);
    for (int i = 0; i < 5; i++) {
      ::wait_ms(100);
    } // End of 'for' statement
    std::clog << std::endl;
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT_MSG(true, "Led red was on then off");
  } // End of test_led_2
  
  void test_digital_out_1() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    digital_out gpio(p49);
    TEST_ASSERT(gpio.read() == digital_state_t::digital_state_low);
    gpio.write(digital_state_t::digital_state_high);
    TEST_ASSERT(gpio.read() == digital_state_t::digital_state_high);
    for (int i = 0; i < 5; i++) {
      ::wait_ms(100);
    } // End of 'for' statement
    gpio = digital_state_t::digital_state_low;
    TEST_ASSERT(gpio.read() == digital_state_t::digital_state_low);
    for (int i = 0; i < 5; i++) {
      ::wait_ms(100);
    } // End of 'for' statement
    TEST_ASSERT(static_cast<const digital_state_t>(gpio) == digital_state_t::digital_state_low);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT_MSG(true, "Led red was off, on then off");
  }

  void test_digital_out_2() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    digital_out gpio(p49, digital_state_t::digital_state_high);
    TEST_ASSERT(gpio.read() == digital_state_t::digital_state_high);
    gpio.write(digital_state_t::digital_state_low);
    TEST_ASSERT(gpio.read() == digital_state_t::digital_state_low);
    for (int i = 0; i < 5; i++) {
      ::wait_ms(100);
    } // End of 'for' statement
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT_MSG(true, "Led red was on then off");
  }

  void test_uart4_gps() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    int fduart = ::serial_open("/dev/ttyO4", 9600);
    if (fduart < 0) {
      std::cerr << "Unable to open serial device" << std::endl;
    } else {
      // Loop, getting and printing characters
      int i = 0;
      while(i++ < 1000) {
	std::clog << (char)::serial_get_char(fduart);
      } // End of 'while' statement
      ::serial_close(fduart);
    }
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT_MSG(true, "NMEA2000 frame displayed");
  } // End of test_uart4_gps
  
  /**
   * @brief Test case for @see libhal::libhal_shift_out
   * Send MSB bit first
   */
  void test_bits_shift_1() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::libhal_shift_out(p66, p67, shift_modes_msb_first, 0x43);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_bits_shift_1

  /**
   * @brief Test case for @see libhal::libhal_shift_out
   * Send LSB bit first
   */
  void test_bits_shift_2() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::libhal_shift_out(p66, p67, shift_modes_lsb_first, 0x43);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_bits_shift_2

  /**
   * @brief Test case for @see libhal::libhal_shift_out
   * Check setup SPI module
   */
  void test_adc_1() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    float value = ::analog_read(A0);
    std::clog << "test_adc_1: value=" << value << "V" << std::endl;
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_adc_1
  
  /**
   * @brief Test case for @see libhal::libhal_spi_xxx
   * Check setup SPI module
   */
  void test_spi_1() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p113, digital_state_high); // SPI1_CS0
    ::digital_write(p7, digital_state_high); // SPI1_CS1
    int32_t fd = ::libhal_spi_setup(1, 1000000);
    TEST_ASSERT_MSG(fd != -1, "test_spi_1, failed to initialise SPI module");
    ::libhal_spi_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_spi_1

  /**
   * @brief Test case for @see libhal::libhal_spi_xxx
   * Check command for digipot (MCP41050)
   */
  void test_spi_2() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p113, digital_state_high); // SPI1_CS0
    ::digital_write(p7, digital_state_high); // SPI1_CS1
    int32_t fd = ::libhal_spi_setup(1, 1000000);
    TEST_ASSERT_MSG(fd != -1, "test_spi_2, failed to initialise SPI module");
    
    // SPI command registers
    uint8_t cmd0, cmd1;
    uint16_t result;
    uint8_t digipotCommand = 0x01;          // Write data - See DS11195C-page 18 FIGURE 5-2: Command Byte Format
    uint8_t digipotRegisterAddress = 0x01; // Digipot #1
    // Enable CS
    ::digital_write(p113, digital_state_low); // SPI1_CS0
    // Write command
    cmd0 = ((digipotCommand & 0x0f) << 4) | digipotRegisterAddress;
    cmd1 = 0x80; // The value
    TEST_ASSERT_MSG(::libhal_spi_data_read_write(1, &cmd0, sizeof(uint8_t)) == 0, "test_spi_2, failed to send command");
    result = (uint16_t)(cmd0 << 8) & 0xff00;
    // Read value
    TEST_ASSERT_MSG(::libhal_spi_data_read_write(1, &cmd1, sizeof(uint8_t)) == 0, "test_spi_2, failed to send command");
    result |= cmd1;
    TEST_ASSERT_MSG(result == 0, "test_spi_2, unexpected result from MCP41050");
    // Enable CS
    ::digital_write(p113, digital_state_high); // SPI1_CS0

    ::libhal_spi_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_spi_2
  
  /**
   * @brief Test case for @see libhal::libhal_spi_xxx
   * Check command for digipot (MCP41050) + BBB ADC
   */
  void test_spi_3() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p113, digital_state_high); // SPI1_CS0
    ::digital_write(p7, digital_state_high); // SPI1_CS1
    int32_t fd = ::libhal_spi_setup(1, 1000000);
    TEST_ASSERT_MSG(fd != -1, "test_spi_3, failed to initialise SPI module");
    
    // SPI command registers
    uint8_t cmd0, cmd1;
    uint16_t result;
    uint8_t digipot_command = 0x01;         // Write data - See DS11195C-page 18 FIGURE 5-2: Command Byte Format
    uint8_t digipot_register_address = 0x01; // Digipot #1
    // Enable CS
    ::digital_write(p113, digital_state_low); // SPI1_CS0
    // Write command
    cmd0 = ((digipot_command & 0x0f) << 4) | digipot_register_address;
    ::libhal_spi_data_read_write(1, &cmd0, sizeof(uint8_t));
    result = (uint16_t)(cmd0 << 8) & 0xff00;
    // Read value
    cmd1 = 0x81; // The value
    ::libhal_spi_data_read_write(1, &cmd1, sizeof(uint8_t));
    result |= cmd1;
    // TEST_ASSERT_MSG(result == 0, "test_spi_3, unexpected result from MCP41050");
    // Enable CS
    ::digital_write(p113, digital_state_high); // SPI1_CS0
    float expected_adc_value = ::analog_read(A0);
    // std::clog << "test_spi_3: initial value=" << expected_adc_value << "V" << std::endl;
    float step = 0.025;
    for (uint8_t counter = 0x80; counter > 0x00; counter--) {
      // Enable CS
      ::digital_write(p113, digital_state_low); // SPI1_CS0
      // Write command
      cmd0 = ((digipot_command & 0x0f) << 4) | digipot_register_address;
      ::libhal_spi_data_read_write(1, &cmd0, sizeof(uint8_t));
      result = (uint16_t)(cmd0 << 8) & 0xff00;
      // Read value
      cmd1 = counter; // The value
      ::libhal_spi_data_read_write(1, &cmd1, sizeof(uint8_t));
      result |= cmd1;
      // TEST_ASSERT_MSG(result == 0, "test_spi_3, unexpected result from MCP41050");
      // Enable CS
      ::digital_write(p113, digital_state_high); // SPI1_CS0
      float value = ::analog_read(A0);
      expected_adc_value -= step;
      // std::clog << "test_spi_3: value=" << value << "V" << " - expected_adc_value=" << expected_adc_value << " - delta=" << value - expected_adc_value << std::endl;
      expected_adc_value = value;
      if (std::abs(value - expected_adc_value) > 0.005) {
	std::cerr << "??? test_spi_3: value=" << value << "V" << " - expected_adc_value=" << expected_adc_value << " - delta=" << value - expected_adc_value << std::endl;
      }
      TEST_ASSERT_DELTA_MSG(value, expected_adc_value, 0.005, "test_spi_3, unexpected voltage");
    } // End of 'for' statement

    ::libhal_spi_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_spi_3
  
  /**
   * @brief Test case for @see libhal::libhal_spi_xxx
   * Check command for digipot (MCP3201)
   */
  void test_spi_4() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p113, digital_state_high); // SPI1_CS0
    ::digital_write(p7, digital_state_high); // SPI1_CS1
    int32_t fd = ::libhal_spi_setup(1, 1000000);
    TEST_ASSERT_MSG(fd != -1, "test_spi_4, failed to initialise SPI module");
    
    // SPI command registers
    uint8_t cmd0;
    uint16_t result;
    // Enable CS
    ::digital_write(p7, digital_state_low); // SPI1_CS1
    // Write dummy command
    cmd0 = 0x00;
    TEST_ASSERT_MSG(::libhal_spi_data_read_write(1, &cmd0, sizeof(uint8_t)) == 0, "test_spi_4, failed to send command");
    result = (uint16_t)(cmd0 << 8) & 0xff00;
    // Write dummy command
    cmd0 = 0x00;
    TEST_ASSERT_MSG(::libhal_spi_data_read_write(1, &cmd0, sizeof(uint8_t)) == 0, "test_spi_4, failed to send command");
    result |= cmd0;
    // Disable CS
    ::digital_write(p7, digital_state_high); // SPI1_CS1
    // Compute the ADC value
    result >>= 1; // Adjust composite integer for 12 valid bits
    float value = static_cast<float>(result & 0x0fff) * 3.30 / 4096.0;
    std::clog << "test_adc_4: value=" << value << "V" << std::endl;

    ::libhal_spi_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_spi_4
  
  /**
   * @brief Test case for @see libhal::spi
   * Check setup SPI module
   */
  void test_spi_5() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p113, digital_state_high); // SPI1_CS0
    spi spi_(1, p7); // SPI device on channel 1, /CS on p7
    // SPI command registers
    uint8_t cmd0;
    uint16_t result;
    // Write dummy command
    cmd0 = 0x00;
    TEST_ASSERT_MSG(spi_.data_read_write(static_cast<uint8_t *>(&cmd0)) == 0, "test_spi_5, failed to send command");
    result = (uint16_t)(cmd0 << 8) & 0xff00;
    // Write dummy command
    cmd0 = 0x00;
    TEST_ASSERT_MSG(spi_.data_read_write(static_cast<uint8_t *>(&cmd0)) == 0, "test_spi_5, failed to send command");
    result |= cmd0;
    // Compute the ADC value
    result >>= 1; // Adjust composite integer for 12 valid bits
    float value = static_cast<float>(result & 0x0fff) * 3.30 / 4096.0;
    std::clog << "test_adc_5: value=" << value << "V" << std::endl;
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_spi_5

  /**
   * @brief Test case for @see libhal::libhal_spi
   * Check command for digipot (MCP41050) + BBB ADC
   */
  void test_spi_6() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    ::digital_write(p7, digital_state_high); // SPI1_CS1
    spi spi_(1, p113); // SPI device on channel 1, /CS on p113
    // SPI command registers
    uint8_t cmd0, cmd1;
    uint16_t cmd, result;
    uint8_t digipot_command = 0x01;         // Write data - See DS11195C-page 18 FIGURE 5-2: Command Byte Format
    uint8_t digipot_register_address = 0x01; // Digipot #1
    // Write command
    cmd0 = ((digipot_command & 0x0f) << 4) | digipot_register_address; // The command
    cmd1 = 0x81; // The value
    cmd = cmd0 << 8 | cmd1; // WARNING Network order
    TEST_ASSERT_MSG(spi_.data_read_write(&cmd) == 0, "test_spi_6, failed to send command");
    result = cmd;
    // TEST_ASSERT_MSG(result == 0, "test_spi_6, unexpected result from MCP41050");
    analog_in adc(A0);
    float expected_adc_value = static_cast<float>(adc);
    // std::clog << "test_spi_6: initial value=" << expected_adc_value << "V" << std::endl;
    float step = 0.025;
    for (uint8_t counter = 0x80; counter > 0x00; counter--) {
      // Write command
      cmd0 = ((digipot_command & 0x0f) << 4) | digipot_register_address; // The command
      cmd1 = counter; // The value
      cmd = cmd0 << 8 | cmd1; // WARNING Network order
      TEST_ASSERT_MSG(spi_.data_read_write(&cmd) == 0, "test_spi_6, failed to send command");
      result = cmd;
      // TEST_ASSERT_MSG(result == 0, "test_spi_6, unexpected result from MCP41050");
      float value = static_cast<float>(adc);
      expected_adc_value -= step;
      // std::clog << "test_spi_6: value=" << value << "V" << " - expected_adc_value=" << expected_adc_value << " - delta=" << value - expected_adc_value << std::endl;
      if (std::abs(value - expected_adc_value) > 0.005) {
	std::cerr << "??? test_spi_3: value=" << value << "V" << " - expected_adc_value=" << expected_adc_value << " - delta=" << value - expected_adc_value << std::endl;
      }
      expected_adc_value = value;
      TEST_ASSERT_DELTA_MSG(value, expected_adc_value, 0.005, "test_spi_6, unexpected voltage");
    } // End of 'for' statement
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_spi_6
  
  /**
   * @brief Test case for @see libhal::libhal_i2c_xxx
   * Check setup I2C module
   */
  void test_i2c_1() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    int32_t fd = ::libhal_i2c_setup(0, 0x34);
    TEST_ASSERT_MSG(fd != -1, "test_i2c_1, failed to initialise I2C module");
    ::libhal_i2c_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_i2c_1

  /**
   * @brief Test case for @see libhal::libhal_i2c_xxx
   * Check libhal_i2c_read_register8
   * Check libhal_i2c_read_register16
   * Run command 'sudo i2cdump -r 0x000-0x0ff -y 0 0x34' to get correct values
   */
  void test_i2c_2() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    int32_t fd = ::libhal_i2c_setup(0, 0x34);
    TEST_ASSERT_MSG(fd != -1, "test_i2c_2, failed to initialise I2C module");
    int32_t value = ::libhal_i2c_read_register8(fd, 0xf0);
    TEST_ASSERT_MSG(value != -1, "test_i2c_2, failed to access I2C module");
    TEST_ASSERT_MSG(static_cast<uint8_t>(value) == 0x03, "test_i2c_2, Unexpected value");
    value = ::libhal_i2c_read_register16(fd, 0xf4);
    TEST_ASSERT_MSG(value != -1, "test_i2c_2, failed to access I2C module");
    TEST_ASSERT_MSG(static_cast<uint16_t>(value) == 0xca81, "test_i2c_2, Unexpected value");
    value = ::libhal_i2c_read_register16(fd, 0xf7);
    TEST_ASSERT_MSG(value != -1, "test_i2c_2, failed to access I2C module");
    TEST_ASSERT_MSG(static_cast<uint16_t>(value) == 0xff00, "test_i2c_2, Unexpected value");
    ::libhal_i2c_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_i2c_2

  /**
   * @brief Test case for @see libhal::libhal_i2c_xxx
   * Check libhal_i2c_ext_read_register8
   * Check libhal_i2c_ext_write_register8
   */
  void test_i2c_3() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    int32_t fd = ::libhal_i2c_setup(2, 0x57);
    TEST_ASSERT_MSG(fd != -1, "test_i2c_3, failed to initialise I2C module");
    int32_t value = ::libhal_i2c_ext_write_register8(fd, 0x57, 0x0000, 0xca);
    TEST_ASSERT_MSG(value != -1, "test_i2c_3, failed to write I2C module");
    ::wait_ms(5); //  ACKNOWLEDGE POLLING - See DS20001203U-page 11
    value = ::libhal_i2c_ext_read_register8(fd, 0x57, 0x0000);
    TEST_ASSERT_MSG(value != -1, "test_i2c_3, failed to read I2C module");
    TEST_ASSERT_MSG(static_cast<uint8_t>(value) == 0xca, "test_i2c_3, Expected value is 0xca");
    ::libhal_i2c_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_i2c_3

  /**
   * @brief Test case for @see libhal::libhal_i2c_xxx
   * Check libhal_i2c_ext_read_register16
   * Check libhal_i2c_ext_write_register16
   */
  void test_i2c_4() {
    // Preambule
    ::libhal_setup_sys();
    // Test
    int32_t fd = ::libhal_i2c_setup(2, 0x57);
    TEST_ASSERT_MSG(fd != -1, "test_i2c_4, failed to initialise I2C module");
    int32_t value = ::libhal_i2c_ext_write_register16(fd, 0x57, 0x0000, 0xcafe);
    TEST_ASSERT_MSG(value != -1, "test_i2c_4, failed to write I2C module");
    ::wait_ms(5); //  ACKNOWLEDGE POLLING - See DS20001203U-page 11
    value = ::libhal_i2c_ext_read_register16(fd, 0x57, 0x0000);
    TEST_ASSERT_MSG(value != -1, "test_i2c_4, failed to read I2C module");
    TEST_ASSERT_MSG(static_cast<uint16_t>(value) == 0xcafe, "test_i2c_4, Expected value is 0xcafe");
    ::libhal_i2c_close(fd);
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_i2c_4

  /**
   * @brief Test case for @see libhal::i2c
   * Check i2c::read_register
   * Check i2c::write_register
   */
  void test_i2c_5() {
    libhal_factory::get_instance().create_i2c_instance(2, 0x57);
    // Preambule
    ::libhal_setup_sys();
    // Test
    i2c i2c_(2, 0x57);
    uint16_t value = 0xdeca;
    i2c_.write_register(static_cast<uint16_t>(0x0010), value);
    ::wait_ms(5); //  ACKNOWLEDGE POLLING - See DS20001203U-page 11
    value = 0;
    i2c_.read_register(static_cast<uint16_t>(0x0010), &value);
    TEST_ASSERT_MSG(value == 0xdeca, "test_i2c_5, Expected value is 0xdeca");
    // Postambule
    ::libhal_shutdown_sys();
    TEST_ASSERT(true);
  } // End of test_i2c_5

  /**
   * @brief Test case for @see libhal::libhal_factory
   * Check libhal_factory::create_digital_out_instance
   * Check libhal_factory::create_analog_in_instance
   * Check libhal_factory::create_i2c_instance
   * Check libhal_factory::create_spi_instance
   */
  void test_factory_1() {
    // Preambule
    // Test
    std::unique_ptr<digital_out> p49_ = libhal_factory::get_instance().create_digital_out_instance(p49);
    TEST_ASSERT(p49_.get() != NULL);
    std::unique_ptr<analog_in> adc0_ = libhal_factory::get_instance().create_analog_in_instance(A0);
    TEST_ASSERT(adc0_.get() != NULL);
    std::unique_ptr<i2c> i2c_ = libhal_factory::get_instance().create_i2c_instance(2, 0x57);
    TEST_ASSERT(i2c_.get() != NULL);
    std::unique_ptr<spi> spi_ = libhal_factory::get_instance().create_spi_instance(1, p7);
    TEST_ASSERT(spi_.get() != NULL);
    // Postambule
    p49_.reset();
    adc0_.reset();
    i2c_.reset();
    spi_.reset();
  } // End of test_factory_1

  void test_factory_2() {
    // Preambule
    std::unique_ptr<digital_out> p49_ = libhal_factory::get_instance().create_digital_out_instance(p49);
    std::unique_ptr<analog_in> adc0_ = libhal_factory::get_instance().create_analog_in_instance(A0);
    std::unique_ptr<i2c> i2c_ = libhal_factory::get_instance().create_i2c_instance(2, 0x57);
    std::unique_ptr<spi> spi_ = libhal_factory::get_instance().create_spi_instance(1, p7);
    // Test
    p49_->write(digital_state_t::digital_state_high);
    for (int i = 0; i < 5; i++) {
      ::wait_ms(100);
    } // End of 'for' statement
    p49_->write(digital_state_t::digital_state_low);
    std::clog << "test_factory_2: initial value=" << static_cast<float>(*adc0_) << "V" << std::endl;
    // Postambule
    p49_.reset();
    adc0_.reset();
    i2c_.reset();
    spi_.reset();
  } // End of test_factory_2

};

/**
 * @brief Display User help
 */
static void usage() {
  cout << "usage: testlib [MODE]\n"
       << "where MODE may be one of:\n"
       << "  --compiler\n"
       << "  --html\n"
       << "  --text-terse (default)\n"
       << "  --text-verbose\n";
  exit(0);
}

/**
 * @brief Process command line options
 * @param[in] p_argc Number of argumrnt
 * @param[in] p_argv List of the arguments
 */
static unique_ptr<Test::Output> cmdline(int p_argc, char* p_argv[]) {
  if (p_argc > 2) {
    usage(); // will not return
  }
  
  Test::Output* output = NULL;
	
  if (p_argc == 1) {
    output = new Test::TextOutput(Test::TextOutput::Verbose);
  } else {
    const char* arg = p_argv[1];
    if (strcmp(arg, "--compiler") == 0) {
      output = new Test::CompilerOutput;
    } else if (strcmp(arg, "--html") == 0) {
      output =  new Test::HtmlOutput;
    } else if (strcmp(arg, "--text-terse") == 0) {
      output = new Test::TextOutput(Test::TextOutput::Terse);
    } else if (strcmp(arg, "--text-verbose") == 0) {
      output = new Test::TextOutput(Test::TextOutput::Verbose);
    } else {
      cout << "invalid commandline argument: " << arg << endl;
      usage(); // will not return
    }
  }
  
  return unique_ptr<Test::Output>(output);
}

/**
 * @brief Main test program
 * @param[in] p_argc Number of argumrnt
 * @param[in] p_argv List of the arguments
 */
int main(int p_argc, char* p_argv[]) {
  try {
    Test::Suite ts;
    ts.add(unique_ptr<Test::Suite>(new libhal_test_suite));

    // Run the tests
    unique_ptr<Test::Output> output(cmdline(p_argc, p_argv));
    ts.run(*output, true);

    Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
    if (html) {
      html->generate(cout, true, "Libhal test suite");
    }
  } catch (...) {
    cout << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
