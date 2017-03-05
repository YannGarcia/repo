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
    //TEST_ADD(logger_test_suite::test_logger_7);
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
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_trace_set() == false);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_debug_set() == false);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_info_set() == true);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_warning_set() == true);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_error_set() == true);
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_2() {
    std::string s("logger2");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger2.log"));
    
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).info("Test logger2 info"));
    TEST_THROWS_MSG(logger_factory::get_instance().get_logger(std::string("logger")).info("Test logger"), std::invalid_argument, "logger_factory::get_logger");
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_3() {
    std::string s("logger3");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger3.log"));
    
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).trace("Test logger3 trace"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).debug("Test logger3 debug"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).info("Test logger3 info"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).warning("Test logger3 warning"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).error(std::string("Test logger3 error")));
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_4() {
    std::string s("logger4");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger4.log", logger_levels_t::trace, logger_time_formats_t::time));
    
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_trace_set() == true);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_debug_set() == false);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_info_set() == false);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_warning_set() == false);
    TEST_ASSERT(logger_factory::get_instance().get_logger(s).is_error_set() == false);

    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).trace("Test logger4 trace"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).debug("Test logger4 debug"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).info("Test logger4 info"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).warning("Test logger4 warning"));
    TEST_THROWS_NOTHING(logger_factory::get_instance().get_logger(s).error(std::string("Test logger4 error")));
  }
  
  /**
   * @brief Test case for @see logger::logger
   * @see logger::logger_factory
   */
  void test_logger_5() {
    std::string s("logger5");
    TEST_THROWS_NOTHING(logger_factory::get_instance());

    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger5.log", logger_levels_t::trace));    
    TEST_THROWS_NOTHING(logger_factory::get_instance().remove_logger(s));
    TEST_THROWS(logger_factory::get_instance().get_logger(s), std::invalid_argument);
  }
  
  /**
   * @brief Test case for @see logger::logger
   *        Variable arguments
   */
  void test_logger_6() {
    std::string s("logger6");
    TEST_THROWS_NOTHING(logger_factory::get_instance().add_logger(s, "/tmp/logger6.log", logger_levels_t::trace | logger_levels_t::error));
    logger_factory::get_instance().get_logger(s).error("Test logger6: This a multiple variable error: %d - %s", 666, "The devil is in the details");
    TEST_THROWS_NOTHING(logger_factory::get_instance().remove_logger(s));
  }
  
  /**
   * @brief Test case for @see logger::logger
   */
  /* TODO To be continued
  void test_logger_7() {
    logger::logger l("", "/tmp/logger7.log", logger_levels_t::all, logger_time_formats_t::time);
    
    TEST_ASSERT(l.is_trace_set() == true);
    TEST_ASSERT(l.is_debug_set() == true);
    TEST_ASSERT(l.is_info_set() == true);
    TEST_ASSERT(l.is_warning_set() == true);
    TEST_ASSERT(l.is_error_set() == true);

    l << logger_levels_t::trace << "Test logger7 trace\n";
    l << logger_levels_t::info << "Test logger7 info\n";
    l << logger_levels_t::warning << "Test logger7 warning\n";
    l << logger_levels_t::error << "Test logger7 error\n";
    }*/
  
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

  cout << "Please, do not forget to remove /tmp/logger<n>.log after checks" << endl;
  system("for i in `ls /tmp/logger*.log`; do cat $i; done");
  return EXIT_SUCCESS;
}
