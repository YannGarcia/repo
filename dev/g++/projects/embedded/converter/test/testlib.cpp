/**
 * @file      testlib.cpp
 * @brief     Types converter test suite.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <cpptest.h>

#include "converter.h"
#include "helper.h"

using namespace std;

/**
 * @class Type converter test suite implementation
 */
class converter_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  converter_test_suite() {
    TEST_ADD(converter_test_suite::test_bin_to_bcd);
    TEST_ADD(converter_test_suite::test_bcd_to_bin);
    TEST_ADD(converter_test_suite::test_string_to_bytes);
    TEST_ADD(converter_test_suite::test_bytes_to_string);
    TEST_ADD(converter_test_suite::test_swap);
    TEST_ADD(converter_test_suite::test_hexa_to_bytes);
    TEST_ADD(converter_test_suite::test_bytes_to_hexa);
    TEST_ADD(converter_test_suite::test_int_to_bytes_1);
    TEST_ADD(converter_test_suite::test_int_to_bytes_2);
    TEST_ADD(converter_test_suite::test_int_to_bytes_3);
    TEST_ADD(converter_test_suite::test_bytes_to_int_1);
    TEST_ADD(converter_test_suite::test_bytes_to_int_2);
    TEST_ADD(converter_test_suite::test_bytes_to_int_3);
    TEST_ADD(converter_test_suite::test_long_to_bytes);
    TEST_ADD(converter_test_suite::test_bytes_to_long);
    TEST_ADD(converter_test_suite::test_float_to_bytes_1);
    TEST_ADD(converter_test_suite::test_float_to_bytes_2);
    TEST_ADD(converter_test_suite::test_bytes_to_float_1);
    TEST_ADD(converter_test_suite::test_bytes_to_float_2);
    TEST_ADD(converter_test_suite::test_time_to_string_1);
    TEST_ADD(converter_test_suite::test_trim);
    TEST_ADD(converter_test_suite::test_split_arguments_line_1);
  }
	
private:
  /**
   * @brief Test case for @see converter::bin_to_bcd
   */
  void test_bin_to_bcd() {
    TEST_ASSERT((int)converter::get_instance().bin_to_bcd(0x00) == (int)0x00);
    TEST_ASSERT((int)converter::get_instance().bin_to_bcd(0x01) == (int)0x01);
    TEST_ASSERT((int)converter::get_instance().bin_to_bcd(0x0A) == (int)0x10);
    TEST_ASSERT((int)converter::get_instance().bin_to_bcd(0x0F) == (int)0x15);
  }

  /**
   * @brief Test case for @see converter::bcd_to_bin
   */
  void test_bcd_to_bin() {
    TEST_ASSERT((int)converter::get_instance().bcd_to_bin(0x00) == (int)0x00);
    TEST_ASSERT((int)converter::get_instance().bcd_to_bin(0x01) == (int)0x01);
    TEST_ASSERT((int)converter::get_instance().bcd_to_bin(0x13) == (int)0x0D);
    TEST_ASSERT((int)converter::get_instance().bcd_to_bin(0x15) == (int)0x0F);
  }

  /**
   * @brief Test case for @see converter::string_to_bytes
   */
  void test_string_to_bytes() {
    std::string frombytes("Hello");
    std::vector<uint8_t> to_bytes(converter::get_instance().string_to_bytes(frombytes));
    TEST_ASSERT(to_bytes.size() == 5);
    uint8_t expected_result[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::bytes_to_string
   */
  void test_bytes_to_string() {
    std::vector<uint8_t> from_bytes = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    TEST_ASSERT(converter::get_instance().bytes_to_string(from_bytes).compare("Hello") == 0);
  }

  /**
   * @brief Test case for @see converter::swap
   */
  void test_swap() {
    TEST_ASSERT(converter::get_instance().swap(static_cast<const uint16_t>(0xcafe)) == 0xfeca);
    TEST_ASSERT(converter::get_instance().swap(static_cast<const uint32_t>(0xcafedeca)) == 0xcadefeca);
  }

  /**
   * @brief Test case for @see converter::hexa_to_bytes
   */
  void test_hexa_to_bytes() {
    string str("5f83af3bfb6c5a8c5ef0794cc6d4cff418ef4cbb09b2785801477a92907ba828");
    std::vector<uint8_t> to_bytes = converter::get_instance().hexa_to_bytes(str);
    TEST_ASSERT(to_bytes.size() == 32);
    uint8_t expected_result[] = { 0x5f, 0x83, 0xaf, 0x3b, 0xfb, 0x6c, 0x5a, 0x8c, 0x5e, 0xf0, 0x79, 0x4c, 0xc6, 0xd4, 0xcf, 0xf4, 0x18, 0xef, 0x4c, 0xbb, 0x09, 0xb2, 0x78, 0x58, 0x01, 0x47, 0x7a, 0x92, 0x90, 0x7b, 0xa8, 0x28 };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::hexa_to_bytes
   */
  void test_bytes_to_hexa() {
    std::vector<uint8_t> from_bytes = { 0x5f, 0x83, 0xaf, 0x3b, 0xfb, 0x6c, 0x5a, 0x8c, 0x5e, 0xf0, 0x79, 0x4c, 0xc6, 0xd4, 0xcf, 0xf4, 0x18, 0xef, 0x4c, 0xbb, 0x09, 0xb2, 0x78, 0x58, 0x01, 0x47, 0x7a, 0x92, 0x90, 0x7b, 0xa8, 0x28 };
    string str = converter::get_instance().bytes_to_hexa(from_bytes);
    TEST_ASSERT(str.compare("5f83af3bfb6c5a8c5ef0794cc6d4cff418ef4cbb09b2785801477a92907ba828") == 0);
  }

  /**
   * @brief Test case for @see converter::int_to_bytes
   */
  void test_int_to_bytes_1() {
    vector<uint8_t> to_bytes = converter::get_instance().int_to_bytes(3405700810);
    TEST_ASSERT(to_bytes.size() == sizeof(int));
    uint8_t expected_result[] = { 0xca, 0xfe, 0xde, 0xca };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::int_to_bytes
   */
  void test_int_to_bytes_2() {
    vector<uint8_t> to_bytes = converter::get_instance().int_to_bytes(2434158052);
    TEST_ASSERT(to_bytes.size() == sizeof(int));
    uint8_t expected_result[] = { 0x91, 0x16, 0x4d, 0xe4 };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::int_to_bytes
   */
  void test_int_to_bytes_3() {
    vector<uint8_t> to_bytes = converter::get_instance().int_to_bytes(3830257297);
    TEST_ASSERT(to_bytes.size() == sizeof(int));
    uint8_t expected_result[] = { 0xe4, 0x4d, 0x16, 0x91 };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::bytes_to_int
   */
  void test_bytes_to_int_1() {
    vector<uint8_t> from_bytes = { 0xca, 0xfe, 0xde, 0xca };
    TEST_ASSERT(converter::get_instance().bytes_to_int(from_bytes) == static_cast<int>(3405700810));
  }

  /**
   * @brief Test case for @see converter::bytes_to_int
   */
  void test_bytes_to_int_2() {
    vector<uint8_t> from_bytes = { 0x91, 0x16, 0x4d, 0xe4 };
    TEST_ASSERT(converter::get_instance().bytes_to_int(from_bytes) == static_cast<int>(2434158052));
  }

  /**
   * @brief Test case for @see converter::bytes_to_int
   */
  void test_bytes_to_int_3() {
    vector<uint8_t> from_bytes = { 0xe4, 0x4d, 0x16, 0x91 };
    TEST_ASSERT(converter::get_instance().bytes_to_int(from_bytes) == static_cast<int>(3830257297));
  }

  /**
   * @brief Test case for @see converter::long_to_bytes
   */
  void test_long_to_bytes() {
    vector<uint8_t> to_bytes = converter::get_instance().long_to_bytes(static_cast<int64_t>(4621188755229004256));
    TEST_ASSERT(to_bytes.size() == sizeof(int64_t));
    uint8_t expected_result[] = { 0x40, 0x21, 0xc2, 0xb0, 0x53, 0x74, 0x51, 0xe0 };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::bytes_to_long
   */
  void test_bytes_to_long() {
    vector<uint8_t> from_bytes = { 0x40, 0x21, 0xc2, 0xb0, 0x53, 0x74, 0x51, 0xe0 };
    TEST_ASSERT(converter::get_instance().bytes_to_long(from_bytes) == static_cast<int64_t>(4621188755229004256));
  }

  /**
   * @brief Test case for @see converter::float_to_bytes
   */
  void test_float_to_bytes_1() {
    vector<uint8_t> to_bytes = converter::get_instance().float_to_bytes(3.141519);
    TEST_ASSERT(to_bytes.size() == sizeof(float));
    uint8_t expected_result[] = { 0xa6, 0x0e, 0x49, 0x40 };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::float_to_bytes
   */
  void test_float_to_bytes_2() {
    vector<uint8_t> to_bytes = converter::get_instance().float_to_bytes(-3.141519);
    TEST_ASSERT(to_bytes.size() == sizeof(float));
    uint8_t expected_result[] = { 0xa6, 0x0e, 0x49, 0xc0 };
    TEST_ASSERT(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  }

  /**
   * @brief Test case for @see converter::bytes_to_float
   */
  void test_bytes_to_float_1() {
    vector<uint8_t> from_bytes = { 0xa6, 0x0e, 0x49, 0x40 };
    TEST_ASSERT(converter::get_instance().bytes_to_float(from_bytes) == static_cast<float>(3.141519));
  }

  /**
   * @brief Test case for @see converter::bytes_to_float
   */
  void test_bytes_to_float_2() {
    vector<uint8_t> from_bytes = { 0xa6, 0x0e, 0x49, 0xc0 };
    TEST_ASSERT(converter::get_instance().bytes_to_float(from_bytes) == static_cast<float>(-3.141519));
  }

  void test_time_to_string_1() {
    time_t current_time = 1489755780;
    std::string str = converter::get_instance().time_to_string(current_time);
    //cout << "test_time_to_string_1: " << str << "\r" << endl;
    std::string result("Fri, 17 Mar 2017 14:03:00 +0100");
    //cout << "test_time_to_string_1: " << result << "\r" << endl;
    //cout << "test_time_to_string_1: " << str.compare(result) << "\r" << endl;
    TEST_ASSERT(str.compare(result) == 0); // http://www.unixtimestamp.com/
  }

  /**
   * @brief Test case for @see converter::trim
   */
  void test_trim() {
    std::string str = "   A  lot of  white spaces    ";
    std::string trimmedStr = converter::get_instance().trim(str);
    TEST_ASSERT(trimmedStr.compare("A  lot of  white spaces") == 0);
  }

  /**
   * @brief Test case for @see converter::split_arguments_line
   */
  void test_split_arguments_line_1() {
    std::string str = "--host localhost --port 12345 --duration -1";
    std::vector<std::string> tokens = converter::get_instance().split_arguments_line(str);
    std::clog << "Tokens: " << std::endl;
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
      std::clog << "   " << *it << std::endl;
    }
    TEST_ASSERT(tokens.size() == 4);
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
    ts.add(unique_ptr<Test::Suite>(new converter_test_suite));

    // Run the tests
    unique_ptr<Test::Output> output(cmdline(p_argc, p_argv));
    ts.run(*output, true);

    Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
    if (html) {
      html->generate(cout, true, "Converter test suite");
    }
  } catch (...) {
    cout << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
