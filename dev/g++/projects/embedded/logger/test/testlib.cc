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

using namespace std;

/*!
 * \class logger test suite implementation
 */
class logger_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
/*!
 * \brief Test case for @see logger::logger
 * @see logger::logger_factory
 */
TEST(logger_test_suite, add_logger) {
  std::string s("logger1");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  
  ASSERT_NO_THROW(logger_factory::get_instance());

  ASSERT_NO_THROW(logger_factory::get_instance().add_logger(s, path));
  ASSERT_NO_THROW(logger_factory::get_instance().add_logger(s, path));
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_trace_set() == false);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_debug_set() == false);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_info_set() == true);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_warning_set() == true);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_error_set() == true);
}
  
/*!
 * \brief Test case for @see logger::logger
 * @see logger::logger_factory
 */
TEST(logger_test_suite, get_logger) {
  std::string s("logger2");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  
  ASSERT_NO_THROW(logger_factory::get_instance());

  ASSERT_NO_THROW(logger_factory::get_instance().add_logger(s, path));
    
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).info("Test logger2 info"));
  ASSERT_THROW(logger_factory::get_instance().get_logger(std::string("logger")).info("Test logger"), std::invalid_argument);
}
  
/*!
 * \brief Test case for @see logger::logger
 * @see logger::logger_factory
 */
TEST(logger_test_suite, logger_basic) {
  std::string s("logger3");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  
  ASSERT_NO_THROW(logger_factory::get_instance());

  ASSERT_NO_THROW(logger_factory::get_instance().add_logger(s, path));
    
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).trace("Test logger3 trace"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).debug("Test logger3 debug"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).info("Test logger3 info"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).warning("Test logger3 warning"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).error(std::string("Test logger3 error")));
}
  
/*!
 * \brief Test case for @see logger::logger
 * @see logger::logger_factory
 */
TEST(logger_test_suite, logger_simple) {
  std::string s("logger4");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  
  ASSERT_NO_THROW(logger_factory::get_instance());

  ASSERT_NO_THROW(logger_factory::get_instance().add_logger(s, path, logger_levels_t::trace, logger_time_formats_t::time));
    
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_trace_set() == true);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_debug_set() == false);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_info_set() == false);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_warning_set() == false);
  ASSERT_TRUE(logger_factory::get_instance().get_logger(s).is_error_set() == false);

  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).trace("Test logger4 trace"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).debug("Test logger4 debug"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).info("Test logger4 info"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).warning("Test logger4 warning"));
  ASSERT_NO_THROW(logger_factory::get_instance().get_logger(s).error(std::string("Test logger4 error")));
}
  
/*!
 * \brief Test case for @see logger::logger
 * @see logger::logger_factory
 */
TEST(logger_test_suite, remove_logger) {
  std::string s("logger5");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  
  ASSERT_NO_THROW(logger_factory::get_instance());

  ASSERT_NO_THROW(logger_factory::get_instance().add_logger(s, path, logger_levels_t::trace));
  ASSERT_NO_THROW(logger_factory::get_instance().remove_logger(s));
  ASSERT_THROW(logger_factory::get_instance().get_logger(s), std::invalid_argument);
}
  
/*!
 * \brief Test case for @see logger::logger
 *        Variable arguments
 */
TEST(logger_test_suite, logger_multiple) {
  std::string s("logger6");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  
  ASSERT_NO_THROW(logger_factory::get_instance().add_logger(s, path, logger_levels_t::trace | logger_levels_t::error));
  logger_factory::get_instance().get_logger(s).error("Test logger6: This a multiple variable error: %d - %s", 666, "The devil is in the details");
  ASSERT_NO_THROW(logger_factory::get_instance().remove_logger(s));
}
  
/*!
 * \brief Test case for @see logger::logger
 */
/* TODO To be continued
   void test_logger_7() {
   logger::logger l("", "~/tmp/logger7.log", logger_levels_t::all, logger_time_formats_t::time);
    
   ASSERT_TRUE(l.is_trace_set() == true);
   ASSERT_TRUE(l.is_debug_set() == true);
   ASSERT_TRUE(l.is_info_set() == true);
   ASSERT_TRUE(l.is_warning_set() == true);
   ASSERT_TRUE(l.is_error_set() == true);

   l << logger_levels_t::trace << "Test logger7 trace\n";
   l << logger_levels_t::info << "Test logger7 info\n";
   l << logger_levels_t::warning << "Test logger7 warning\n";
   l << logger_levels_t::error << "Test logger7 error\n";
   }*/

/*!
 * \brief Main test program
 * \param[in] p_argc Number of argumrnt
 * \param[in] p_argv List of the arguments
 */
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
