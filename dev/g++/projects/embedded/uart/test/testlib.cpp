/**
 * @file    testlib.cpp
 * @brief   Types logger test suite.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <gtest.h>
#define ASSERT_TRUE_MSG(exp1, msg) ASSERT_TRUE(exp1) << msg

#include "converter.h"
#include "rs232.h"

using namespace std;

/**
 * @class Type uart test suite implementation
 */
class uart_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
  /**
   * @brief Test case for @see uart::uart
   * @see uart::data_available
   * @see uart::read
   */
  TEST(uart_test_suite, test_uart_1() {
    std::string s("/dev/ttyO4");
    rs232 ttyO2(s, 9600);
    uint32_t counter = 0;
    while (counter++ < 10) {
      std::vector<unsigned char> result(ttyO2.data_available());
      ttyO2.read(result);
      std::string buffer = converter::get_instance().bytes_to_string(result);
      std::clog << "Receive data: '" << buffer << "'" << std::endl;
    } // End of 'while' statement
  }

  TEST(uart_test_suite, test_uart_2() {
    std::string s("/dev/ttyO4");
    rs232 ttyO2(s, 9600);
    uint32_t counter = 0;
    while (counter++ < 20) {
      std::string line;
      ttyO2.read(line);
      std::clog << "Receive data: '" << line << "'" << std::endl;
    } // End of 'while' statement
  }
  
/**
 * @brief Main test program
 * @param[in] p_argc Number of argumrnt
 * @param[in] p_argv List of the arguments
 */
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
