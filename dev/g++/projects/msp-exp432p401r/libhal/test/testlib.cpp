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
#include <memory>
#include <cstring>
#include <algorithm>
#include <cmath> // Used for abs

#include "libhal_factory.hpp"

using namespace std;

/**
 * @brief Main test program
 * @param[in] p_argc Number of argumrnt
 * @param[in] p_argv List of the arguments
 */
int main(int p_argc, char* p_argv[]) {
  try {
    std::unique_ptr<digital_out> p74_ = libhal_factory::get_instance().create_digital_out_instance(p74);
  } catch (...) {
    cout << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
