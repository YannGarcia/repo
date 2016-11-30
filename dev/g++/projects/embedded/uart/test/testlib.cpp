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

#include <cpptest.h>

#include "converter.h"
#include "rs232.h"

using namespace std;

/**
 * @class Type uart test suite implementation
 */
class uart_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  uart_test_suite() {
    TEST_ADD(uart_test_suite::test_uart_1);
    TEST_ADD(uart_test_suite::test_uart_2);
  }
	
private:
  /**
   * @brief Test case for @see uart::uart
   * @see uart::data_available
   * @see uart::read
   */
  void test_uart_1() {
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

  void test_uart_2() {
    std::string s("/dev/ttyO4");
    rs232 ttyO2(s, 9600);
    uint32_t counter = 0;
    while (counter++ < 20) {
      std::string line;
      ttyO2.read(line);
      std::clog << "Receive data: '" << line << "'" << std::endl;
    } // End of 'while' statement
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
    ts.add(unique_ptr<Test::Suite>(new uart_test_suite));

    // Run the tests
    unique_ptr<Test::Output> output(cmdline(p_argc, p_argv));
    ts.run(*output, true);

    Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
    if (html) {
      html->generate(cout, true, "Uart test suite");
    }
  } catch (...) {
    cout << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
