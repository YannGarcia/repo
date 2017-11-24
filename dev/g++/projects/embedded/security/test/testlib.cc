/*!
 * \file    testlib.cpp
 * \brief   Logger test suite.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <gtest.h>
#define ASSERT_TRUE_MSG(exp1, msg) ASSERT_TRUE(exp1) << msg

#include "logger_factory.hh"

#include "sha256.hh"

using namespace std;

/*!
 * \class logger test suite implementation
 */
class security_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
/*!
 * \brief Test case for @see logger::logger
 * \sa logger::logger_factory
 */
TEST(security_test_suite, sha256) {
  std::string s("logger1");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  
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
