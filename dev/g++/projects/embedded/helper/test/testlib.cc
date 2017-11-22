/**
 * @file      testlib.cpp
 * @brief     Helper test suite.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

#include <gtest.h>
#define ASSERT_TRUE_MSG(exp1, msg) ASSERT_TRUE(exp1) << msg

#include "helper.hh"
#include "converter.hh"
#include "date_time.hh"
#include "ibstream.hh"
#include "obstream.hh"
#include "keyboard.hh"
#include "get_opt.hh"
#include "runnable.hh"

using namespace std;

/**
 * @class helpers::keyboard test suite implementation
 */
class keyboard_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case for @see keyboard::kbhit
 */
TEST(keyboard_test_suite, kbhit) {
  char expected_values[] = { 'q', 'w', 'e' };
  char value;
  int counter = 0;
  cout << "Strike q, w and e keys..." << endl;
  while (counter < 3) {
    this_thread::sleep_for(chrono::milliseconds(50));
    value = static_cast<char>(keyboard::kbhit());
    if (value != 0) {
      clog << "key pressed was: '" << value << "'" << endl;
      if (value != expected_values[counter]) {
        ASSERT_TRUE_MSG(value == expected_values[counter], "test_keyboard, unexpected key stricked");
      } else {
        counter += 1;
      }
      // Read the entered character
      ::getchar();
    }
  } // End of 'while' statement
}

/**
 * @class helpers::runnable test suite implementation
 */
class runnable_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

class thread_ : public runnable {
  uint32_t _counter;
public:
  thread_() : _counter(0) { };
  void run() {
    _running = true;
    while (_running) {
      _counter += 1;
      this_thread::sleep_for(chrono::milliseconds(500));
    }
  };
    
  const uint32_t get_counter() const { return _counter; };
}; // End of class thread_

/**
 * @brief Test case for @see keyboard::kbhit
 */
TEST(runnable_test_suite, test_runnable) {
  thread_ t;
  t.start();
  this_thread::sleep_for(chrono::milliseconds(5000));
  t.stop();
  cout << "#loops: " << t.get_counter() << endl;
  
  ASSERT_TRUE(t.get_counter() == 10);
}

/**
 * @class helpers::get_opt test suite implementation
 */
class get_opt_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case for @see get_opt::get_opt
 */
TEST(get_opt_test_suite, option_1) {
  int argc = 1;
  const char * argv[] = { (const char *)"the_app_name" };
  bool is_debug_set, auto_mode;
  get_opt::get_opt opt(argc, argv);
  opt >> get_opt::option('a', "auto_mode", auto_mode, false);
  opt >> get_opt::option('d', "debug", is_debug_set, false);
    
  ASSERT_TRUE(auto_mode == false);
  ASSERT_TRUE(is_debug_set == false);
}
  
/**
 * @brief Test case for @see get_opt::get_opt
 */
TEST(get_opt_test_suite, option_2) {
  int argc = 3;
  const char * argv[] = { (const char *)"the_app_name", (const char *)"--auto_mode", (const char *)"--debug" };
  bool is_debug_set, auto_mode;
  get_opt::get_opt opt(argc, argv);
  opt >> get_opt::option('a', "auto_mode", auto_mode, false);
  opt >> get_opt::option('d', "debug", is_debug_set, false);
    
  ASSERT_TRUE(auto_mode == true);
  ASSERT_TRUE(is_debug_set == true);
}
  
/**
 * @brief Test case for @see get_opt::get_opt
 */
TEST(get_opt_test_suite, option_3) {
  int argc = 3;
  const char * argv[] = { (const char *)"the_app_name", (const char *)"-a", (const char *)"-d" };
  bool is_debug_set, auto_mode;
  get_opt::get_opt opt(argc, argv);
  opt >> get_opt::option('a', "auto_mode", auto_mode, false);
  opt >> get_opt::option('d', "debug", is_debug_set, false);
    
  ASSERT_TRUE(auto_mode == true);
  ASSERT_TRUE(is_debug_set == true);
}
  
/**
 * @brief Test case for @see get_opt::get_opt
 */
TEST(get_opt_test_suite, option_4) {
  int argc = 4;
  const char * argv[] = { (const char *)"the_app_name", (const char *)"-d", (const char *)"--host", (const char *)"127.0.0.1" };
  bool is_debug_set, auto_mode;
  string host;
  get_opt::get_opt opt(argc, argv);
  opt >> get_opt::option('h', "host", host, "");
  opt >> get_opt::option('a', "auto_mode", auto_mode, false);
  opt >> get_opt::option('d', "debug", is_debug_set, false);
    
  ASSERT_TRUE(host.compare("127.0.0.1") == 0);
  ASSERT_TRUE(auto_mode == false);
  ASSERT_TRUE(is_debug_set == true);
}
  
/**
 * @brief Test case for @see get_opt::get_opt
 */
TEST(get_opt_test_suite, option_5) {
  int argc = 4;
  const char * argv[] = { (const char *)"the_app_name", (const char *)"--debug", (const char *)"-h", (const char *)"127.0.0.1" };
  bool is_debug_set, auto_mode;
  string host;
  get_opt::get_opt opt(argc, argv);
  opt >> get_opt::option('h', "host", host, "");
  opt >> get_opt::option('a', "auto_mode", auto_mode, false);
  opt >> get_opt::option('d', "debug", is_debug_set, false);
    
  ASSERT_TRUE(host.compare("127.0.0.1") == 0);
  ASSERT_TRUE(auto_mode == false);
  ASSERT_TRUE(is_debug_set == true);
}
  
/**
 * @brief Test case for @see get_opt::get_opt
 */
TEST(get_opt_test_suite, option_6) {
  int argc = 4;
  const char * argv[] = { (const char *)"the_app_name", (const char *)"--debug", (const char *)"-x", (const char *)"127.0.0.1" };
  bool is_debug_set, auto_mode;
  string host;
  get_opt::get_opt opt(argc, argv);
  opt >> get_opt::option('h', "host", host, "");
  opt >> get_opt::option('a', "auto_mode", auto_mode, false);
  opt >> get_opt::option('d', "debug", is_debug_set, false);
    
  ASSERT_TRUE(host.empty());
  ASSERT_TRUE(auto_mode == false);
  ASSERT_TRUE(is_debug_set == true);
}

/**
 * @class helpers::helper test suite implementation
 */
class helper_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case for @see helper::concat
 */
TEST(helper_test_suite, concat) {
  // Preambule
  list<vector<unsigned char> > vs;
  vector<unsigned char> v1 = { 0xca, 0xfe };
  vs.push_back(v1);
  vector<unsigned char> v2 = { 0xde };
  vs.push_back(v2);
  vector<unsigned char> v3 = { 0xca };
  vs.push_back(v3);
  // Test
  vector<unsigned char> vres = helper::get_instance().concat(vs);
  ASSERT_TRUE(vres.size() == 4);
  unsigned char expected_result[] = { 0xca, 0xfe, 0xde, 0xca };
  ASSERT_TRUE(equal(vres.begin(), vres.end(), expected_result));
  //clog << ">>> test_concat done" << endl;
}
	
/**
 * @brief Test case for @see helper::test_hexa_dump
 */
TEST(helper_test_suite, hexa_dump_1) {
  vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
  helper::get_instance().hexa_dump(v1);
  ASSERT_TRUE(true);
  //clog << ">>> test_hexa_dump_1 done" << endl;
}

/**
 * @brief Test case for @see helper::test_hexa_dump
 * Check argument #2
 */
TEST(helper_test_suite, hexa_dump_2) {
  vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
  helper::get_instance().hexa_dump(v1, 2, 4);
  ASSERT_TRUE(true);
  //clog << ">>> test_hexa_dump_2 done" << endl;
}

/**
 * @brief Test case for @see helper::extract
 * Check default value for argument #3
 */
TEST(helper_test_suite, extract_1) {
  vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
  vector<unsigned char> vres = helper::get_instance().extract(v1, 4);
  ASSERT_TRUE(vres.size() == 4);
  unsigned char expected_result[] = { 0xde, 0xad, 0xbe, 0xef };
  ASSERT_TRUE_MSG(equal(vres.begin(), vres.end(), expected_result), "test_extract_1 failed");
  //clog << ">>> test_extract_1 done" << endl;
}

/**
 * @brief Test case for @see helper::extract
 * Check default behaviour of the method 
 */
TEST(helper_test_suite, extract_2) {
  vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
  vector<unsigned char> vres = helper::get_instance().extract(v1, 3, 3);
  ASSERT_TRUE_MSG(vres.size() == 3, "test_extract_2 Wrong arry size");
  unsigned char expected_result[] = { 0xca, 0xde, 0xad };
  ASSERT_TRUE_MSG(equal(vres.begin(), vres.end(), expected_result), "test_extract_2 failed");
  //clog << ">>> test_extract_3 done" << endl;
}

/**
 * @brief Test case for @see helper::extract
 * Check invalid value for parameter #1
 */
TEST(helper_test_suite, extract_3) {
  vector<unsigned char> v1;
  vector<unsigned char> vres = helper::get_instance().extract(v1, 3);
  ASSERT_TRUE_MSG(vres.size() == 0, "test_extract_3 failed");
  //clog << ">>> test_extract_3 done" << endl;
}
  
/**
 * @brief Test case for @see helper::extract
 * Check invalid value for parameter #2
 */
TEST(helper_test_suite, extract_4) {
  vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
  vector<unsigned char> vres = helper::get_instance().extract(v1, 30, 3);
  ASSERT_TRUE_MSG(vres.size() == 0, "test_extract_4 failed");
  //clog << ">>> test_extract_4 done" << endl;
}
  
/**
 * @brief Test case for @see helper::extract
 * Check invalid value for parameter #2
 */
TEST(helper_test_suite, extract_5) {
  vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
  vector<unsigned char> vres = helper::get_instance().extract(v1, 3, 30);
  ASSERT_TRUE_MSG(vres.size() == 0, "test_extract_5 failed");
  //clog << ">>> test_extract_5 done" << endl;
}

/**
 * @class helpers::date_time test suite implementation
 */
class date_time_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case for @see helper::date_time
 */
TEST(date_time_test_suite, parse_date_time) {
  date_time d(std::string("Wed Oct 11 14:09:44 2017"));
  ASSERT_TRUE_MSG(d.get_sday().compare("Wed") == 0, "Failed, check day variable");
  ASSERT_TRUE_MSG(d.get_day() == 11, "Failed, check day variable");
  ASSERT_TRUE_MSG(d.get_month() == 10, "Failed, check month variable");
  ASSERT_TRUE_MSG(d.get_year() == 2017, "Failed, check year variable");
  ASSERT_TRUE_MSG(d.get_hour() == 14, "Failed, check hour variable");
  ASSERT_TRUE_MSG(d.get_minutes() == 9, "Failed, check minutes variable");
  ASSERT_TRUE_MSG(d.get_seconds() == 44, "Failed, check seconds variable");
}

/**
 * @brief Test case for @see helper::date_time
 */
/*TEST(date_time_test_suite, date_time) {
  date_time now;
  uint32_t day = 7;
  uint32_t month = 5;
  uint32_t year = 2015; 
  //clog << "Now Day:" << now.get_sday() << endl;
  //clog << "Now Date:" << now.get_day() << endl;
  //clog << "Now month:" << now.get_month() << endl;
  //clog << "Now Year:" << now.get_year() << endl;
  //clog << "Now Hour:" << now.get_hour() << endl;
  //clog << "Now Minutes:" << now.get_minutes() << endl;
  //clog << "Now Second:" << now.get_seconds() << endl;
  ASSERT_TRUE_MSG(now.get_day() == day, "test_date_time_1 failed, check day variable");
  ASSERT_TRUE_MSG(now.get_month() == month, "test_date_time_1 failed, check month variable");
  ASSERT_TRUE_MSG(now.get_year() == year, "test_date_time_1 failed, check year variable");
  //clog << ">>> test_date_time_1 done" << endl;
  }*/

/**
 * @class helpers::ibstream test suite implementation
 */
class ibstream_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case 
 * @see helpers::ibstream::read_bits
 * @see helpers::ibstream::gcount
 * @see helpers::ibstream::tellg
 * @see helpers::ibstream::gremaining
 */
TEST(ibstream_test_suite, ibstream_1) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, 6);
  ASSERT_TRUE_MSG(codec.gremaining() == codec.gcount() - 6, "test_ibstream_1 failed, wrong remaining length");
  ASSERT_TRUE_MSG(codec.tellg() == 6, "test_ibstream_1 failed, wrong index position");
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_1 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x32, "test_ibstream_1 failed, 0x32 expected");
  codec.read_bits(output_buffer, 2);
  ASSERT_TRUE_MSG(codec.gremaining() == codec.gcount() - 6 - 2, "test_ibstream_1 failed, wrong remaining length");
  ASSERT_TRUE_MSG(codec.tellg() == 8, "test_ibstream_1 failed, wrong index position");
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_1 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x2, "test_ibstream_1 failed, 0x02 expected");
  codec.read_bits(output_buffer, 16);
  ASSERT_TRUE_MSG(codec.gremaining() == codec.gcount() - 6 - 2 - 16, "test_ibstream_1 failed, wrong remaining length");
  ASSERT_TRUE_MSG(codec.tellg() == 24, "test_ibstream_1 failed, wrong index position");
  ASSERT_TRUE_MSG(output_buffer.size() == 2, "test_ibstream_1 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xfe, "test_ibstream_1 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xde, "test_ibstream_1 failed, 0xde expected");
  //clog << ">>> test_ibstream_1 done" << endl;
}

/**
 * @brief Test case for @see helpers::ibstream::read_bits
 * Check that the buffer can be read in one call of method read_bits
 */
TEST(ibstream_test_suite, ibstream_2) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, 36);
  ASSERT_TRUE_MSG(output_buffer.size() == 5, "test_ibstream_2 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_ibstream_2 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_ibstream_2 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_ibstream_2 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_ibstream_2 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_ibstream_2 failed, 0x70 expected");
  //clog << ">>> test_ibstream_2 done" << endl;
}

/**
 * @brief Test case for @see helper::ibstream::read_bits
 * Check with requested number of bits greather than buffer size
 */
TEST(ibstream_test_suite, ibstream_3) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, 64);
  ASSERT_TRUE_MSG(output_buffer.size() == 5, "test_ibstream_3 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_ibstream_3 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_ibstream_3 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_ibstream_3 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_ibstream_3 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_ibstream_3 failed, 0xde expected");
  //clog << ">>> test_ibstream_3 done" << endl;
}

/**
 * @brief Test case for @see helpers::ibstream::read_bits
 * Check with requested number of bits greather than remaining bits to read
 */
TEST(ibstream_test_suite, ibstream_4) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, 17);
  ASSERT_TRUE_MSG(output_buffer.size() == 3, "test_ibstream_4 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_ibstream_4 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_ibstream_4 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0x80, "test_ibstream_4 failed, 0x80 expected");
  codec.read_bits(output_buffer, 64);
  ASSERT_TRUE_MSG(output_buffer.size() == 3, "test_ibstream_4 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xbd, "test_ibstream_4 failed, 0xbd expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x94, "test_ibstream_4 failed, 0x94 expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xe0, "test_ibstream_4 failed, 0xe0 expected");
  //clog << ">>> test_ibstream_4 done" << endl;
}

/**
 * @brief Test case for @see helpers::ibstream::gcount()
 */
TEST(ibstream_test_suite, ibstream_5) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  ASSERT_TRUE_MSG(codec.gcount() == 36, "test_ibstream_5 failed, wrong length");
}

/**
 * @brief Test case for @see helpers::ibstream::read_bits
 * Check when trying to read bits while no more bits are available
 */
TEST(ibstream_test_suite, ibstream_6) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, codec.gcount());
  ASSERT_TRUE_MSG(output_buffer.size() == input_buffer.size(), "test_ibstream_6 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_ibstream_6 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_ibstream_6 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_ibstream_6 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_ibstream_6 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_ibstream_6 failed, 0x70 expected");
  uint32_t result = codec.read_bits(output_buffer, 1);
  ASSERT_TRUE_MSG(result == 0, "test_ibstream_6 failed, wrong returned code");
  ASSERT_TRUE_MSG(output_buffer.size() == 0, "test_ibstream_6 failed, wrong length");
  //clog << ">>> test_ibstream_6 done" << endl;
}

/**
 * @brief Test case
 * @see helpers::ibstream::read_bits
 * @see helpers::ibstream::ignore
 */
TEST(ibstream_test_suite, ibstream_7) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  uint32_t result = codec.ignore(codec.gcount());
  ASSERT_TRUE_MSG(result == codec.gcount(), "test_ibstream_7 failed, wrong returned code");
  result = codec.read_bits(output_buffer, 1);
  ASSERT_TRUE_MSG(result == 0, "test_ibstream_7 failed, wrong returned code");
  ASSERT_TRUE_MSG(output_buffer.size() == 0, "test_ibstream_7 failed, wrong length");
  //clog << ">>> test_ibstream_7 done" << endl;
}

/**
 * @brief Test case
 * @see helpers::ibstream::read_bits
 * @see helpers::ibstream::ignore
 */
TEST(ibstream_test_suite, ibstream_8) {
  vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  uint32_t result = codec.ignore(6);
  ASSERT_TRUE_MSG(result == 6, "test_ibstream_8 failed, wrong returned code");
  codec.read_bits(output_buffer, 2);
  ASSERT_TRUE_MSG(codec.gremaining() == codec.gcount() - 6 - 2, "test_ibstream_8 failed, wrong remaining length");
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_8 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x2, "test_ibstream_8 failed, 0x02 expected");
  codec.read_bits(output_buffer, 16);
  ASSERT_TRUE_MSG(codec.gremaining() == codec.gcount() - 6 - 2 - 16, "test_ibstream_8 failed, wrong remaining length");
  ASSERT_TRUE_MSG(output_buffer.size() == 2, "test_ibstream_8 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xfe, "test_ibstream_8 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xde, "test_ibstream_8 failed, 0xde expected");
  //clog << ">>> test_ibstream_8 done" << endl;
}

/**
 * @brief Test case for UtDenmTrigger
 * @see helpers::ibstream::read_bits
 * @see helpers::ibstream::gcount
 * @see helpers::ibstream::gremaining
 */
TEST(ibstream_test_suite, ibstream_9) {
  vector<unsigned char> input_buffer = converter::get_instance().hexa_to_bytes("10f000517bc55f2f000002000000005b0000000000000000");
  helpers::ibstream codec(input_buffer, (8 * sizeof(uint8_t)) * input_buffer.size());
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(codec.gremaining() == codec.gcount() - 8, "test_ibstream_9 failed, wrong remaining lgremength");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x10, "test_ibstream_9 failed, 0x10 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xf0, "test_ibstream_9 failed, 0xf0 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 6);
  ASSERT_TRUE_MSG(output_buffer.size() == 6, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x51, "test_ibstream_9 failed, 0x51 expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0x7b, "test_ibstream_9 failed, 0x7b expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xc5, "test_ibstream_9 failed, 0xc5 expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x5f, "test_ibstream_9 failed, 0x5f expected");
  ASSERT_TRUE_MSG(output_buffer[5] == 0x2f, "test_ibstream_9 failed, 0x2f expected");
  ASSERT_TRUE_MSG(converter::get_instance().bytes_to_long(output_buffer) == 349968883503, "test_ibstream_9 failed, 349968883503 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 3);
  ASSERT_TRUE_MSG(output_buffer.size() == 3, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0x02, "test_ibstream_9 failed, 0x02 expected");
  ASSERT_TRUE_MSG(converter::get_instance().bytes_to_int(output_buffer) == 2, "test_ibstream_9 failed, 2 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x5b, "test_ibstream_9 failed, 0x5b expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 2);
  ASSERT_TRUE_MSG(output_buffer.size() == 2, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(converter::get_instance().bytes_to_int(output_buffer) == 0, "test_ibstream_9 failed, 0 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 2);
  ASSERT_TRUE_MSG(output_buffer.size() == 2, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(converter::get_instance().bytes_to_int(output_buffer) == 0, "test_ibstream_9 failed, 0 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
  ASSERT_TRUE_MSG(codec.gremaining() == 0, "test_ibstream_9 failed, wrong remaining length");

  //clog << ">>> test_ibstream_9 done" << endl;
}

/**
 * @brief Test case for UtTerminateDenmEvent
 * @see helpers::ibstream::read_bits
 * @see helpers::ibstream::gcount
 * @see helpers::ibstream::gremaining
 */
TEST(ibstream_test_suite, ibstream_10) {
  vector<unsigned char> input_buffer = converter::get_instance().hexa_to_bytes("14000082350002");
  helpers::ibstream codec(input_buffer, (8 * sizeof(uint8_t)) * input_buffer.size());
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_10 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x14, "test_ibstream_10 failed, 0x14 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 4);
  ASSERT_TRUE_MSG(output_buffer.size() == 4, "test_ibstream_10 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_10 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x00, "test_ibstream_10 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0x82, "test_ibstream_10 failed, 0x82 expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0x35, "test_ibstream_10 failed, 0x35 expected");
  ASSERT_TRUE_MSG(converter::get_instance().bytes_to_int(output_buffer) == 33333, "test_ibstream_10 failed, 3333 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 2);
  ASSERT_TRUE_MSG(output_buffer.size() == 2, "test_ibstream_10 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_10 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x02, "test_ibstream_10 failed, 0x02 expected");
  ASSERT_TRUE_MSG(converter::get_instance().bytes_to_int(output_buffer) == 2, "test_ibstream_10 failed, 2 expected");
  ASSERT_TRUE_MSG(codec.gremaining() == 0, "test_ibstream_10 failed, wrong remaining length");

  //clog << ">>> test_ibstream_10 done" << endl;
}

/**
 * @brief Test case for GeoNetworking::BasicHeader
 * @see helpers::ibstream::read_bits
 * @see helpers::ibstream::gcount
 * @see helpers::ibstream::gremaining
 */
TEST(ibstream_test_suite, ibstream_11) {
  vector<unsigned char> input_buffer = converter::get_instance().hexa_to_bytes("01231b01");
  helpers::ibstream codec(input_buffer, (8 * sizeof(uint8_t)) * input_buffer.size());
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, 4);
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x00, "test_ibstream_11 failed, 0x00 expected");
  codec.read_bits(output_buffer, 4);
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x01, "test_ibstream_11 failed, 0x01 expected");
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x23, "test_ibstream_11 failed, 0x23 expected");
  codec.read_bits(output_buffer, 6);
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x06, "test_ibstream_11 failed, 0x06 expected");
  codec.read_bits(output_buffer, 2);
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x03, "test_ibstream_11 failed, 0x03 expected");    
  codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x01, "test_ibstream_11 failed, 0x01 expected");
  ASSERT_TRUE_MSG(codec.gremaining() == 0, "test_ibstream_11 failed, wrong remaining length");

  //clog << ">>> test_ibstream_11 done" << endl;
}
  
/**
 * @brief Test case for @see helpers::ibstream::read_bits
 * @see helpers::ibstream::seekg
 */
TEST(ibstream_test_suite, ibstream_12) {
  vector<unsigned char> input_buffer = { 0xca, 0xff, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream codec(input_buffer, 32 + 4);
  vector<unsigned char> output_buffer;
  codec.read_bits(output_buffer, codec.gcount());
  ASSERT_TRUE_MSG(output_buffer.size() == input_buffer.size(), "test_ibstream_12 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_ibstream_12 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xff, "test_ibstream_12 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_ibstream_12 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_ibstream_12 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_ibstream_12 failed, 0x70 expected");
  codec.seekg(3);
  uint8_t value;
  codec.read<uint8_t>(value);
  ASSERT_TRUE_MSG(value == 0x57, "test_ibstream_12 failed, 0xa7 expected");
  codec.seekg(3);
  output_buffer = codec.peek(4);
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_12 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x05, "test_ibstream_12 failed, 0x0a expected");
  codec.seekg(4);
  output_buffer = codec.peek(4);
  ASSERT_TRUE_MSG(output_buffer.size() == 1, "test_ibstream_12 failed, wrong length");
  ASSERT_TRUE_MSG(output_buffer[0] == 0x0a, "test_ibstream_12 failed, 0x0a expected");
    
  //clog << ">>> test_ibstream_12 done" << endl;
}
  
/**
 * @brief Test case for @see helpers::ibstream::read_bits
 * @see helpers::ibstream::is_equal
 */
TEST(ibstream_test_suite, ibstream_13) {
  vector<unsigned char> input_buffer = { 0xca, 0xff, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::ibstream buffer1(input_buffer, 32 + 4);
  ASSERT_TRUE_MSG(buffer1 == buffer1, "test_ibstream_13 failed, buffer is not equal to itself");
  input_buffer[3] =0xcd; // This value is different
  helpers::ibstream buffer2(input_buffer, 32 + 4);
  ASSERT_TRUE_MSG(buffer1 != buffer2, "test_ibstream_13 failed, buffer1 is not equal to buffer2");
    
  //clog << ">>> test_ibstream_13 done" << endl;
}

/**
 * @class helpers::obstream test suite implementation
 */
class obstream_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case 
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_1) {
  helpers::obstream codec;
  vector<unsigned char> output_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  codec.write_bits(output_buffer, 36);
  ASSERT_TRUE_MSG(codec.gcount() == 36, "test_obstream_1 failed, wrong length");
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_1 failed, 0xca expected");
  // ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_1 failed, 0xfe expected");
  // ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_obstream_1 failed, 0xde expected");
  // ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_obstream_1 failed, 0xca expected");
  // ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_obstream_1 failed, 0x70 expected"); 

  //clog << ">>> test_obstream_1 done" << endl;
}
  
/**
 * @brief Test case
 * Check that buffer capacity is increased according to the required size
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_2) {
  helpers::obstream codec(4);
  vector<unsigned char> output_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  codec.write_bits(output_buffer, 36);
  ASSERT_TRUE_MSG(codec.gcount() == 36, "test_obstream_2 failed, wrong length");
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_2 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_2 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_obstream_2 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_obstream_2 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_obstream_2 failed, 0x70 expected");
    
  //clog << ">>> test_obstream_2 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_3) {
  helpers::obstream codec;
  vector<unsigned char> output_buffer = { 0x0c };
  codec.write_bits(output_buffer, 4);
  output_buffer = { 0x0a };
  codec.write_bits(output_buffer, 4);
  output_buffer = { 0x0f };
  codec.write_bits(output_buffer, 4);
  output_buffer = { 0x0e };
  codec.write_bits(output_buffer, 4);
  output_buffer = { 0x0d };
  codec.write_bits(output_buffer, 4);
  ASSERT_TRUE_MSG(codec.gcount() == 20, "test_obstream_3 failed, wrong length");
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_3 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_3 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xd0, "test_obstream_3 failed, 0xd0 expected");
    
  //clog << ">>> test_obstream_3 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_4) {
  helpers::obstream codec;
  vector<unsigned char> output_buffer = { 0x06 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x02 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x05 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x07 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x07 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x03 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x03 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x06 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x06 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x02 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x04 };
  codec.write_bits(output_buffer, 3);
  output_buffer = { 0x07 };
  codec.write_bits(output_buffer, 3);
  ASSERT_TRUE_MSG(codec.gcount() == 36, "test_obstream_4 failed, wrong length");
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_4 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_4 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_obstream_4 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_obstream_4 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_obstream_4 failed, 0x70 expected");
    
  //clog << ">>> test_obstream_4 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_5) {
  helpers::obstream codec;
  vector<unsigned char> output_buffer = { 0x00 };
  codec.write_bits(output_buffer, 4);
  output_buffer = { 0x01 };
  codec.write_bits(output_buffer, 4);
  output_buffer = { 0x23 };
  codec.write_bits(output_buffer, 8);
  output_buffer = { 0x06 };
  codec.write_bits(output_buffer, 6);
  output_buffer = { 0x03 };
  codec.write_bits(output_buffer, 2);
  output_buffer = { 0x01 };
  codec.write_bits(output_buffer, 8);
  ASSERT_TRUE_MSG(codec.gcount() == 32, "test_obstream_5 failed, wrong length");
  helper::get_instance().hexa_dump(codec.to_bytes(), 0, codec.gcount() / 8);
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0x01, "test_obstream_5 failed, 0x01 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x23, "test_obstream_5 failed, 0x23 expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0x1b, "test_obstream_5 failed, 0x1b expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0x01, "test_obstream_5 failed, 0x01 expected");
    
  //clog << ">>> test_obstream_5 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_6) {
  helpers::obstream codec;
  codec.write_bits(0x06, 3);
  codec.write_bits(0x02, 3);
  codec.write_bits(0x05, 3);
  codec.write_bits(0x07, 3);
  codec.write_bits(0x07, 3);
  codec.write_bits(0x03, 3);
  codec.write_bits(0x03, 3);
  codec.write_bits(0x06, 3);
  codec.write_bits(0x06, 3);
  codec.write_bits(0x02, 3);
  codec.write_bits(0x04, 3);
  codec.write_bits(0x07, 3);
  ASSERT_TRUE_MSG(codec.gcount() == 36, "test_obstream_6 failed, wrong length");
  vector<unsigned char> output_buffer(codec.to_bytes());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_6 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_6 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_obstream_6 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_obstream_6 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_obstream_6 failed, 0x70 expected");
    
  //clog << ">>> test_obstream_6 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_7) {
  helpers::obstream codec;
  codec.write<uint8_t>(0x10);
  codec.write<uint8_t>(0xf0);
  codec.write<uint64_t>(349968883503, (8 * sizeof(uint8_t) * 6));
  codec.write<uint32_t>(2, (8 * sizeof(uint8_t) * 3));    
  // vector<unsigned char> convert = converter::get_instance().long_to_bytes(349968883503);
  // codec.write_bits(vector<unsigned char>(convert.cbegin() + 2, convert.cend()), (8 * sizeof(uint8_t)) * 6);
  // convert = converter::get_instance().int_to_bytes(2);
  // codec.write_bits(vector<unsigned char>(convert.cbegin() + 1, convert.cend()), (8 * sizeof(uint8_t)) * 3);
  ASSERT_TRUE_MSG(codec.gcount() == (8 * sizeof(uint8_t)) * 11, "test_obstream_7 failed, wrong length");
  helper::get_instance().hexa_dump(codec.to_bytes(), 0, codec.gcount() / 8);
  vector<unsigned char> output_buffer(codec.to_bytes());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0x10, "test_obstream_7 failed, 0x10 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xf0, "test_obstream_7 failed, 0xf0 expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0x00, "test_obstream_7 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0x51, "test_obstream_7 failed, 0x51 expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x7b, "test_obstream_7 failed, 0x7b expected");
  ASSERT_TRUE_MSG(output_buffer[5] == 0xc5, "test_obstream_7 failed, 0xc5 expected");
  ASSERT_TRUE_MSG(output_buffer[6] == 0x5f, "test_obstream_7 failed, 0x5f expected");
  ASSERT_TRUE_MSG(output_buffer[7] == 0x2f, "test_obstream_7 failed, 0x2f expected");
  ASSERT_TRUE_MSG(output_buffer[8] == 0x00, "test_obstream_7 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[9] == 0x00, "test_obstream_7 failed, 0x00 expected");
  ASSERT_TRUE_MSG(output_buffer[10] == 0x02, "test_obstream_7 failed, 0x02 expected");
    
  //clog << ">>> test_obstream_7 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write<float>
 */
TEST(obstream_test_suite, obstream_8) {
  helpers::obstream codec;
  codec.write<float>(3.141519);
  vector<unsigned char> output_buffer(codec.to_bytes());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0xa6, "test_obstream_8 failed, 0xa6 expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0x0e, "test_obstream_8 failed, 0x0e expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0x49, "test_obstream_8 failed, 0x49 expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0x40, "test_obstream_8 failed, 0x40 expected");
    
  //clog << ">>> test_obstream_8 done" << endl;
}

/**
 * @brief Test case
 * @see helpers::obstream::write<float>
 */
TEST(obstream_test_suite, obstream_9) {
  vector<unsigned char> output_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::obstream codec;
  //    codec.write<vector<unsigned char>& >(output_buffer, 36);
  codec.write_bits(output_buffer, 36);
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_9 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_9 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_obstream_9 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_obstream_9 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_obstream_9 failed, 0x70 expected");
    
  //clog << ">>> test_obstream_9 done" << endl;
}

/**
 * @brief Test case
 * @see helpers::obstream::seekp
 */
TEST(obstream_test_suite, obstream_10) {
  vector<unsigned char> output_buffer = { 0xca, 0xff, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
  helpers::obstream codec;
  //    codec.write<vector<unsigned char>& >(output_buffer, 36);
  codec.write_bits(output_buffer, 36);
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_10 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xff, "test_obstream_10 failed, 0xff expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_obstream_10 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_obstream_10 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x70, "test_obstream_10 failed, 0x70 expected");
  codec.seekp(1 * (8 * sizeof(uint8_t)));
  codec.write<uint8_t>(0xfe);
  output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_10 failed, 0xfe expected");
    
  //clog << ">>> test_obstream_10 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 */
TEST(obstream_test_suite, obstream_11) {
  helpers::obstream codec;
  codec.write_bits(0x06, 3);
  codec.write_bits(0x02, 3);
  codec.write_bits(0x05, 3);
  codec.write_bits(0x07, 3);
  codec.write_bits(0x07, 3);
  codec.write_bits(0x03, 3);
  codec.write_bits(0x03, 3);
  codec.write_bits(0x06, 3);
  codec.write_bits(0x06, 3);
  codec.write_bits(0x02, 3);
  codec.write_bits(0x04, 3);
  codec.write_bits(0x07, 3);
  codec.write_bits(0x0f, 4);
  codec.write_bits(0xaa, 8);
  ASSERT_TRUE_MSG(codec.gcount() == 48, "test_obstream_11 failed, wrong length");
  vector<unsigned char> output_buffer(codec.to_bytes());    
  ASSERT_TRUE_MSG(output_buffer[0] == 0xca, "test_obstream_11 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[1] == 0xfe, "test_obstream_11 failed, 0xfe expected");
  ASSERT_TRUE_MSG(output_buffer[2] == 0xde, "test_obstream_11 failed, 0xde expected");
  ASSERT_TRUE_MSG(output_buffer[3] == 0xca, "test_obstream_11 failed, 0xca expected");
  ASSERT_TRUE_MSG(output_buffer[4] == 0x7f, "test_obstream_11 failed, 0x7f expected");
  ASSERT_TRUE_MSG(output_buffer[5] == 0xaa, "test_obstream_11 failed, 0xaa expected");
    
  //clog << ">>> test_obstream_11 done" << endl;
}
  
/**
 * @brief Test case
 * @see helpers::obstream::write_bits
 * @see helpers::ibstream::gcount
 * @see helpers::ibstream::is_equal
 */
TEST(obstream_test_suite, obstream_12) {
  helpers::obstream buffer1;
  buffer1.write_bits(0x06, 3);
  buffer1.write_bits(0x02, 3);
  buffer1.write_bits(0x05, 3);
  buffer1.write_bits(0x07, 3);
  buffer1.write_bits(0x07, 3);
  buffer1.write_bits(0x03, 3);
  buffer1.write_bits(0x03, 3);
  buffer1.write_bits(0x06, 3);
  buffer1.write_bits(0x06, 3);
  buffer1.write_bits(0x02, 3);
  buffer1.write_bits(0x04, 3);
  buffer1.write_bits(0x07, 3);
  buffer1.write_bits(0x0f, 4);
  buffer1.write_bits(0xaa, 8);
  ASSERT_TRUE_MSG(buffer1.gcount() == 48, "test_obstream_12 failed, wrong length");
  ASSERT_TRUE_MSG(buffer1 == buffer1, "test_obstream_12 failed, buffer is not equal to itself");
  helpers::obstream buffer2;
  buffer2.write_bits(0x06, 3);
  buffer2.write_bits(0x02, 3);
  buffer2.write_bits(0x05, 3);
  buffer2.write_bits(0x07, 3);
  buffer2.write_bits(0x07, 3);
  buffer2.write_bits(0x07, 3); // This one is different
  buffer2.write_bits(0x03, 3);
  buffer2.write_bits(0x06, 3);
  buffer2.write_bits(0x06, 3);
  buffer2.write_bits(0x02, 3);
  buffer2.write_bits(0x04, 3);
  buffer2.write_bits(0x07, 3);
  buffer2.write_bits(0x0f, 4);
  buffer2.write_bits(0xaa, 8);
  ASSERT_TRUE_MSG(buffer2.gcount() == 48, "test_obstream_12 failed, wrong length");
  vector<unsigned char> output_buffer(buffer2.to_bytes());    
  ASSERT_TRUE_MSG(buffer1 != buffer2, "test_obstream_12 failed, buffer1 is equal to buffer2");
  //clog << ">>> test_obstream_12 done" << endl;
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
