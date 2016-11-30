/**
 * @file    testlib.cpp
 * @brief   Logger test suite.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <cpptest.h>

#include "logger_factory.h"

using namespace std;

/**
 * @class logger test suite implementation
 */
class logger_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  logger_test_suite() {
    TEST_ADD(logger_test_suite::test_logger_1);
    TEST_ADD(logger_test_suite::test_logger_2);
    TEST_ADD(logger_test_suite::test_logger_3);
    TEST_ADD(logger_test_suite::test_logger_4);
    TEST_ADD(logger_test_suite::test_logger_5);
    TEST_ADD(logger_test_suite::test_logger_6);
  }
	
private:
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_1() {
    std::string s("logger1");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger1.log"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger1.log"));
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_2() {
    std::string s("logger1");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger1.log"));
    
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).info("Test logger info"));
    TEST_THROWS(logger_factory::get_instance().get_logger(std::string("logger2")).info("Test logger"), std::invalid_argument);
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_3() {
    std::string s("logger1");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger1.log"));
    
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).trace("Test logger trace"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).debug("Test logger debug"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).info("Test logger info"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).warning("Test logger warning"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).error(std::string("Test logger error")));
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_4() {
    std::string s("logger1");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger1.log", logger_levels_t::trace));
    
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).trace("Test logger trace"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).debug("Test logger debug"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).info("Test logger info"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).warning("Test logger warning"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).error(std::string("Test logger error")));
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_5() {
    std::string s("logger1");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger1.log", logger_levels_t::trace));    
    TEST_THROWS_NOTHING(logger_factory::get_instance().remove_logger(s));
    TEST_THROWS(logger_factory::get_instance().get_logger(s), std::invalid_argument);
  }
  
  /**
   * @brief Test case for @see logger::logger
   *        Variable arguments
   */
  void test_logger_6() {
    std::string s("logger1");
    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger1.log", logger_levels_t::trace | logger_levels_t::error));
    logger_factory::get_instance().get_logger(s).error("This a multiple variable error: %d - %s", 666, "The devil is in the details");
    TEST_THROWS_NOTHING(logger_factory::get_instance().remove_logger(s));
  }
  
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
    ts.add(unique_ptr<Test::Suite>(new logger_test_suite));

    // Run the tests
    unique_ptr<Test::Output> output(cmdline(p_argc, p_argv));
    ts.run(*output, true);

    Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
    if (html) {
      html->generate(cout, true, "Logger test suite");
    }
  } catch (...) {
    cout << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
