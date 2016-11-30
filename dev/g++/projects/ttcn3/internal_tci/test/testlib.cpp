/**
 * @file    testlib.cpp
 * @brief   internal::ttcn3::tci test suite.
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

#include "tci_type_impl.h"
#include "tci_module_id_impl.h"

using namespace std;

/**
 * @class tcis::tci test suite implementation
 */
class tci_type_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  tci_type_test_suite() {
    TEST_ADD(tci_type_test_suite::default_ctor);
  } // End of ctor
	
private:
  /**
   * @brief Test case for @see tci_type_impl::tci_type_impl()
   */
  void default_ctor() {
    tci_type_impl t;
    TEST_ASSERT(t.get_name().length() == 0);
    TEST_ASSERT(t.get_type_class() == tci_anytype);
  }
  
}; // End of class tci_type_test_suite

/**
 * @class tcis::date_time test suite implementation
 */
class tci_module_id_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  tci_module_id_test_suite() {
    TEST_ADD(tci_module_id_test_suite::default_ctor_1);
  } // End of ctor
	
private:
  
  /**
   * @brief Test case for @see tci_module_id_impl::tci_module_id_impl()
   */
  void default_ctor_1() {
    tci_module_id_impl m;
    TEST_ASSERT(m.get_object_name().length() == 0);
    TEST_ASSERT(m.compute_length() == sizeof(uint32_t));
    std::vector<uint8_t> buffer(8, 0x00);
    TEST_ASSERT(m.encode(static_cast<uint8_t *>(buffer.data()), 0) == sizeof(uint32_t));
    TEST_ASSERT_MSG(buffer[0] == 0x00, "default_ctor_1, 0x00 expected");
    TEST_ASSERT_MSG(buffer[1] == 0x00, "default_ctor_1, 0x00 expected");
    TEST_ASSERT_MSG(buffer[2] == 0x00, "default_ctor_1, 0x00 expected");
    TEST_ASSERT_MSG(buffer[3] == 0x00, "default_ctor_1, 0x00 expected");
  }

}; // End of class tci_module_id_test_suite

/**
 * @brief Display User help
 */
static void usage() {
  clog << "usage: testlib [MODE]\n"
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
      clog << "invalid commandline argument: " << arg << endl;
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
    ts.add(unique_ptr<Test::Suite>(new tci_type_test_suite));
    ts.add(unique_ptr<Test::Suite>(new tci_module_id_test_suite));

    // Run the tests
    unique_ptr<Test::Output> output(cmdline(p_argc, p_argv));
    ts.run(*output, true);

    Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
    if (html) {
      html->generate(clog, true, "Tci test suite");
    }
  } catch (...) {
    clog << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
