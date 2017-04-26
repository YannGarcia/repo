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

#include <cpptest.h>

#include "helper.h"
#include "converter.h"
#include "date_time.h"
#include "ibstream.h"
#include "obstream.h"
#include "keyboard.h"
#include "get_opt.h"
#include "runnable.h"

using namespace std;

/**
 * @class helpers::keyboard test suite implementation
 */
class keyboard_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  keyboard_test_suite() {
    TEST_ADD(keyboard_test_suite::test_keyboard);
  } // End of ctor
	
private:
  /**
   * @brief Test case for @see keyboard::kbhit
   */
  void test_keyboard() {
    char expected_values[] = { 'q', 'w', 'e' };
    char value;
    int counter = 0;
    std::cout << "Strike q, w and e keys..." << std::endl;
    while (counter < 3) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      value = static_cast<char>(keyboard::kbhit());
      if (value != 0) {
	std::clog << "key pressed was: '" << value << "'" << std::endl;
	if (value != expected_values[counter]) {
	  TEST_ASSERT_MSG(value == expected_values[counter], "test_keyboard, unexpected key stricked");
	} else {
	  counter += 1;
	}
	// Read the entered character
	::getchar();
      }
    } // End of 'while' statement
  }
  
};

/**
 * @class helpers::runnable test suite implementation
 */
class runnable_test_suite : public Test::Suite {
  class thread_ : public runnable {
    uint32_t _counter;
  public:
    thread_() : _counter(0) { };
    void run() {
      _running = true;
      while (_running) {
	_counter += 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    };

    const uint32_t get_counter() const { return _counter; };
  }; // End of class thread_
  
public:
  /**
   * @brief Default ctor
   */
  runnable_test_suite() {
    TEST_ADD(runnable_test_suite::test_runnable);
  } // End of ctor
	
private:
  /**
   * @brief Test case for @see keyboard::kbhit
   */
  void test_runnable() {
    thread_ t;
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    t.stop();
    cout << "#loops: " << t.get_counter() << endl;
    
    TEST_ASSERT(t.get_counter() == 10);
  }
  
};

/**
 * @class helpers::get_opt test suite implementation
 */
class get_opt_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  get_opt_test_suite() {
    TEST_ADD(get_opt_test_suite::test_get_opt_1);
    TEST_ADD(get_opt_test_suite::test_get_opt_2);
    TEST_ADD(get_opt_test_suite::test_get_opt_3);
    TEST_ADD(get_opt_test_suite::test_get_opt_4);
    TEST_ADD(get_opt_test_suite::test_get_opt_5);
    TEST_ADD(get_opt_test_suite::test_get_opt_6);
  } // End of ctor
	
private:
  /**
   * @brief Test case for @see get_opt::get_opt
   */
  void test_get_opt_1() {
    int argc = 1;
    const char * argv[] = { (const char *)"the_app_name" };
    bool is_debug_set, auto_mode;
    get_opt::get_opt opt(argc, argv);
    opt >> get_opt::option('a', "auto_mode", auto_mode, false);
    opt >> get_opt::option('d', "debug", is_debug_set, false);
    
    TEST_ASSERT(auto_mode == false);
    TEST_ASSERT(is_debug_set == false);
  }
  
  /**
   * @brief Test case for @see get_opt::get_opt
   */
  void test_get_opt_2() {
    int argc = 3;
    const char * argv[] = { (const char *)"the_app_name", (const char *)"--auto_mode", (const char *)"--debug" };
    bool is_debug_set, auto_mode;
    get_opt::get_opt opt(argc, argv);
    opt >> get_opt::option('a', "auto_mode", auto_mode, false);
    opt >> get_opt::option('d', "debug", is_debug_set, false);
    
    TEST_ASSERT(auto_mode == true);
    TEST_ASSERT(is_debug_set == true);
  }
  
  /**
   * @brief Test case for @see get_opt::get_opt
   */
  void test_get_opt_3() {
    int argc = 3;
    const char * argv[] = { (const char *)"the_app_name", (const char *)"-a", (const char *)"-d" };
    bool is_debug_set, auto_mode;
    get_opt::get_opt opt(argc, argv);
    opt >> get_opt::option('a', "auto_mode", auto_mode, false);
    opt >> get_opt::option('d', "debug", is_debug_set, false);
    
    TEST_ASSERT(auto_mode == true);
    TEST_ASSERT(is_debug_set == true);
  }
  
  /**
   * @brief Test case for @see get_opt::get_opt
   */
  void test_get_opt_4() {
    int argc = 4;
    const char * argv[] = { (const char *)"the_app_name", (const char *)"-d", (const char *)"--host", (const char *)"127.0.0.1" };
    bool is_debug_set, auto_mode;
    std::string host;
    get_opt::get_opt opt(argc, argv);
    opt >> get_opt::option('h', "host", host, "");
    opt >> get_opt::option('a', "auto_mode", auto_mode, false);
    opt >> get_opt::option('d', "debug", is_debug_set, false);
    
    TEST_ASSERT(host.compare("127.0.0.1") == 0);
    TEST_ASSERT(auto_mode == false);
    TEST_ASSERT(is_debug_set == true);
  }
  
  /**
   * @brief Test case for @see get_opt::get_opt
   */
  void test_get_opt_5() {
    int argc = 4;
    const char * argv[] = { (const char *)"the_app_name", (const char *)"--debug", (const char *)"-h", (const char *)"127.0.0.1" };
    bool is_debug_set, auto_mode;
    std::string host;
    get_opt::get_opt opt(argc, argv);
    opt >> get_opt::option('h', "host", host, "");
    opt >> get_opt::option('a', "auto_mode", auto_mode, false);
    opt >> get_opt::option('d', "debug", is_debug_set, false);
    
    TEST_ASSERT(host.compare("127.0.0.1") == 0);
    TEST_ASSERT(auto_mode == false);
    TEST_ASSERT(is_debug_set == true);
  }
  
  /**
   * @brief Test case for @see get_opt::get_opt
   */
  void test_get_opt_6() {
    int argc = 4;
    const char * argv[] = { (const char *)"the_app_name", (const char *)"--debug", (const char *)"-x", (const char *)"127.0.0.1" };
    bool is_debug_set, auto_mode;
    std::string host;
    get_opt::get_opt opt(argc, argv);
    opt >> get_opt::option('h', "host", host, "");
    opt >> get_opt::option('a', "auto_mode", auto_mode, false);
    opt >> get_opt::option('d', "debug", is_debug_set, false);
    
    TEST_ASSERT(host.empty());
    TEST_ASSERT(auto_mode == false);
    TEST_ASSERT(is_debug_set == true);
  }
  
}; // End of class get_opt_test_suite

/**
 * @class helpers::helper test suite implementation
 */
class helper_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  helper_test_suite() {
    TEST_ADD(helper_test_suite::test_concat);
    TEST_ADD(helper_test_suite::test_hexa_dump_1);
    TEST_ADD(helper_test_suite::test_hexa_dump_2);
    TEST_ADD(helper_test_suite::test_extract_1);
    TEST_ADD(helper_test_suite::test_extract_2);
    TEST_ADD(helper_test_suite::test_extract_3);
    TEST_ADD(helper_test_suite::test_extract_4);
    TEST_ADD(helper_test_suite::test_extract_5);
  } // End of ctor
	
private:
  /**
   * @brief Test case for @see helper::concat
   */
  void test_concat() {
    // Preambule
    std::list<std::vector<unsigned char> > vs;
    std::vector<unsigned char> v1 = { 0xca, 0xfe };
    vs.push_back(v1);
    std::vector<unsigned char> v2 = { 0xde };
    vs.push_back(v2);
    std::vector<unsigned char> v3 = { 0xca };
    vs.push_back(v3);
    // Test
    std::vector<unsigned char> vres = helper::get_instance().concat(vs);
    TEST_ASSERT(vres.size() == 4);
    unsigned char expected_result[] = { 0xca, 0xfe, 0xde, 0xca };
    TEST_ASSERT(std::equal(vres.begin(), vres.end(), expected_result));
    //std::clog << ">>> test_concat done" << std::endl;
  }
	
  /**
   * @brief Test case for @see helper::test_hexa_dump
   */
  void test_hexa_dump_1() {
    std::vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
    helper::get_instance().hexa_dump(v1);
    TEST_ASSERT(true);
    //std::clog << ">>> test_hexa_dump_1 done" << std::endl;
  }

  /**
   * @brief Test case for @see helper::test_hexa_dump
   * Check argument #2
   */
  void test_hexa_dump_2() {
    std::vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
    helper::get_instance().hexa_dump(v1, 2, 4);
    TEST_ASSERT(true);
    //std::clog << ">>> test_hexa_dump_2 done" << std::endl;
  }

  /**
   * @brief Test case for @see helper::extract
   * Check default value for argument #3
   */
  void test_extract_1() {
    std::vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
    std::vector<unsigned char> vres = helper::get_instance().extract(v1, 4);
    TEST_ASSERT(vres.size() == 4);
    unsigned char expected_result[] = { 0xde, 0xad, 0xbe, 0xef };
    TEST_ASSERT_MSG(std::equal(vres.begin(), vres.end(), expected_result), "test_extract_1 failed");
    //std::clog << ">>> test_extract_1 done" << std::endl;
  }
  
  /**
   * @brief Test case for @see helper::extract
   * Check default behaviour of the method 
   */
  void test_extract_2() {
    std::vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
    std::vector<unsigned char> vres = helper::get_instance().extract(v1, 3, 3);
    TEST_ASSERT_MSG(vres.size() == 3, "test_extract_2 Wrong arry size");
    unsigned char expected_result[] = { 0xca, 0xde, 0xad };
    TEST_ASSERT_MSG(std::equal(vres.begin(), vres.end(), expected_result), "test_extract_2 failed");
    //std::clog << ">>> test_extract_3 done" << std::endl;
  }

  /**
   * @brief Test case for @see helper::extract
   * Check invalid value for parameter #1
   */
  void test_extract_3() {
    std::vector<unsigned char> v1;
    std::vector<unsigned char> vres = helper::get_instance().extract(v1, 3);
    TEST_ASSERT_MSG(vres.size() == 0, "test_extract_3 failed");
    //std::clog << ">>> test_extract_3 done" << std::endl;
  }
  
  /**
   * @brief Test case for @see helper::extract
   * Check invalid value for parameter #2
   */
  void test_extract_4() {
    std::vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
    std::vector<unsigned char> vres = helper::get_instance().extract(v1, 30, 3);
    TEST_ASSERT_MSG(vres.size() == 0, "test_extract_4 failed");
    //std::clog << ">>> test_extract_4 done" << std::endl;
  }
  
  /**
   * @brief Test case for @see helper::extract
   * Check invalid value for parameter #2
   */
  void test_extract_5() {
    std::vector<unsigned char> v1 = { 0xca, 0xfe, 0xde, 0xca, 0xde, 0xad, 0xbe, 0xef };
    std::vector<unsigned char> vres = helper::get_instance().extract(v1, 3, 30);
    TEST_ASSERT_MSG(vres.size() == 0, "test_extract_5 failed");
    //std::clog << ">>> test_extract_5 done" << std::endl;
  }
  
}; // End of class helper_test_suite

/**
 * @class helpers::date_time test suite implementation
 */
class date_time_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  date_time_test_suite() {
    TEST_ADD(date_time_test_suite::test_date_time_1);
  } // End of ctor
	
private:
  
  /**
   * @brief Test case for @see helper::date_time
   */
  void test_date_time_1() {
    date_time now;
    uint32_t day = 7;
    uint32_t month = 5;
    uint32_t year = 2015; 
    //std::clog << "Now Day:" << now.get_sday() << std::endl;
    //std::clog << "Now Date:" << now.get_day() << std::endl;
    //std::clog << "Now month:" << now.get_month() << std::endl;
    //std::clog << "Now Year:" << now.get_year() << std::endl;
    //std::clog << "Now Hour:" << now.get_hour() << std::endl;
    //std::clog << "Now Minutes:" << now.get_minutes() << std::endl;
    //std::clog << "Now Second:" << now.get_seconds() << std::endl;
    TEST_ASSERT_MSG(now.get_day() == day, "test_date_time_1 failed, check day variable");
    TEST_ASSERT_MSG(now.get_month() == month, "test_date_time_1 failed, check month variable");
    TEST_ASSERT_MSG(now.get_year() == year, "test_date_time_1 failed, check year variable");
    //std::clog << ">>> test_date_time_1 done" << std::endl;
  }

}; // End of class date_time_test_suite

/**
 * @class helpers::ibstream test suite implementation
 */
class ibstream_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  ibstream_test_suite() {
    TEST_ADD(ibstream_test_suite::test_ibstream_1);
    TEST_ADD(ibstream_test_suite::test_ibstream_2);
    TEST_ADD(ibstream_test_suite::test_ibstream_3);
    TEST_ADD(ibstream_test_suite::test_ibstream_4);
    TEST_ADD(ibstream_test_suite::test_ibstream_5);
    TEST_ADD(ibstream_test_suite::test_ibstream_6);
    TEST_ADD(ibstream_test_suite::test_ibstream_7);
    TEST_ADD(ibstream_test_suite::test_ibstream_8);  
    TEST_ADD(ibstream_test_suite::test_ibstream_9);
    TEST_ADD(ibstream_test_suite::test_ibstream_10);
    TEST_ADD(ibstream_test_suite::test_ibstream_11);
    TEST_ADD(ibstream_test_suite::test_ibstream_12);
    TEST_ADD(ibstream_test_suite::test_ibstream_13);
  } // End of ctor
	
private:

  /**
   * @brief Test case 
   * @see helpers::ibstream::read_bits
   * @see helpers::ibstream::gcount
   * @see helpers::ibstream::tellg
   * @see helpers::ibstream::gremaining
   */
  void test_ibstream_1() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, 6);
    TEST_ASSERT_MSG(codec.gremaining() == codec.gcount() - 6, "test_ibstream_1 failed, wrong remaining length");
    TEST_ASSERT_MSG(codec.tellg() == 6, "test_ibstream_1 failed, wrong index position");
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_1 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x32, "test_ibstream_1 failed, 0x32 expected");
    codec.read_bits(output_buffer, 2);
    TEST_ASSERT_MSG(codec.gremaining() == codec.gcount() - 6 - 2, "test_ibstream_1 failed, wrong remaining length");
    TEST_ASSERT_MSG(codec.tellg() == 8, "test_ibstream_1 failed, wrong index position");
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_1 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x2, "test_ibstream_1 failed, 0x02 expected");
    codec.read_bits(output_buffer, 16);
    TEST_ASSERT_MSG(codec.gremaining() == codec.gcount() - 6 - 2 - 16, "test_ibstream_1 failed, wrong remaining length");
    TEST_ASSERT_MSG(codec.tellg() == 24, "test_ibstream_1 failed, wrong index position");
    TEST_ASSERT_MSG(output_buffer.size() == 2, "test_ibstream_1 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xfe, "test_ibstream_1 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xde, "test_ibstream_1 failed, 0xde expected");
    //std::clog << ">>> test_ibstream_1 done" << std::endl;
  }

  /**
   * @brief Test case for @see helpers::ibstream::read_bits
   * Check that the buffer can be read in one call of method read_bits
   */
  void test_ibstream_2() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, 36);
    TEST_ASSERT_MSG(output_buffer.size() == 5, "test_ibstream_2 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_ibstream_2 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_ibstream_2 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_ibstream_2 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_ibstream_2 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_ibstream_2 failed, 0x70 expected");
    //std::clog << ">>> test_ibstream_2 done" << std::endl;
  }

  /**
   * @brief Test case for @see helper::ibstream::read_bits
   * Check with requested number of bits greather than buffer size
   */
  void test_ibstream_3() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, 64);
    TEST_ASSERT_MSG(output_buffer.size() == 5, "test_ibstream_3 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_ibstream_3 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_ibstream_3 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_ibstream_3 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_ibstream_3 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_ibstream_3 failed, 0xde expected");
    //std::clog << ">>> test_ibstream_3 done" << std::endl;
  }

  /**
   * @brief Test case for @see helpers::ibstream::read_bits
   * Check with requested number of bits greather than remaining bits to read
   */
  void test_ibstream_4() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, 17);
    TEST_ASSERT_MSG(output_buffer.size() == 3, "test_ibstream_4 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_ibstream_4 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_ibstream_4 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0x80, "test_ibstream_4 failed, 0x80 expected");
    codec.read_bits(output_buffer, 64);
    TEST_ASSERT_MSG(output_buffer.size() == 3, "test_ibstream_4 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xbd, "test_ibstream_4 failed, 0xbd expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x94, "test_ibstream_4 failed, 0x94 expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xe0, "test_ibstream_4 failed, 0xe0 expected");
    //std::clog << ">>> test_ibstream_4 done" << std::endl;
  }

  /**
   * @brief Test case for @see helpers::ibstream::gcount()
   */
  void test_ibstream_5() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    TEST_ASSERT_MSG(codec.gcount() == 36, "test_ibstream_5 failed, wrong length");
  }

  /**
   * @brief Test case for @see helpers::ibstream::read_bits
   * Check when trying to read bits while no more bits are available
   */
  void test_ibstream_6() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, codec.gcount());
    TEST_ASSERT_MSG(output_buffer.size() == input_buffer.size(), "test_ibstream_6 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_ibstream_6 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_ibstream_6 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_ibstream_6 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_ibstream_6 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_ibstream_6 failed, 0x70 expected");
    uint32_t result = codec.read_bits(output_buffer, 1);
    TEST_ASSERT_MSG(result == 0, "test_ibstream_6 failed, wrong returned code");
    TEST_ASSERT_MSG(output_buffer.size() == 0, "test_ibstream_6 failed, wrong length");
    //std::clog << ">>> test_ibstream_6 done" << std::endl;
  }

  /**
   * @brief Test case
   * @see helpers::ibstream::read_bits
   * @see helpers::ibstream::ignore
   */
  void test_ibstream_7() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    uint32_t result = codec.ignore(codec.gcount());
    TEST_ASSERT_MSG(result == codec.gcount(), "test_ibstream_7 failed, wrong returned code");
    result = codec.read_bits(output_buffer, 1);
    TEST_ASSERT_MSG(result == 0, "test_ibstream_7 failed, wrong returned code");
    TEST_ASSERT_MSG(output_buffer.size() == 0, "test_ibstream_7 failed, wrong length");
    //std::clog << ">>> test_ibstream_7 done" << std::endl;
  }

  /**
   * @brief Test case
   * @see helpers::ibstream::read_bits
   * @see helpers::ibstream::ignore
   */
  void test_ibstream_8() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    uint32_t result = codec.ignore(6);
    TEST_ASSERT_MSG(result == 6, "test_ibstream_8 failed, wrong returned code");
    codec.read_bits(output_buffer, 2);
    TEST_ASSERT_MSG(codec.gremaining() == codec.gcount() - 6 - 2, "test_ibstream_8 failed, wrong remaining length");
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_8 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x2, "test_ibstream_8 failed, 0x02 expected");
    codec.read_bits(output_buffer, 16);
    TEST_ASSERT_MSG(codec.gremaining() == codec.gcount() - 6 - 2 - 16, "test_ibstream_8 failed, wrong remaining length");
    TEST_ASSERT_MSG(output_buffer.size() == 2, "test_ibstream_8 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xfe, "test_ibstream_8 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xde, "test_ibstream_8 failed, 0xde expected");
    //std::clog << ">>> test_ibstream_8 done" << std::endl;
  }

  /**
   * @brief Test case for UtDenmTrigger
   * @see helpers::ibstream::read_bits
   * @see helpers::ibstream::gcount
   * @see helpers::ibstream::gremaining
   */
  void test_ibstream_9() {
    std::vector<unsigned char> input_buffer = converter::get_instance().hexa_to_bytes("10f000517bc55f2f000002000000005b0000000000000000");
    helpers::ibstream codec(input_buffer, (8 * sizeof(uint8_t)) * input_buffer.size());
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(codec.gremaining() == codec.gcount() - 8, "test_ibstream_9 failed, wrong remaining lgremength");
    TEST_ASSERT_MSG(output_buffer[0] == 0x10, "test_ibstream_9 failed, 0x10 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xf0, "test_ibstream_9 failed, 0xf0 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 6);
    TEST_ASSERT_MSG(output_buffer.size() == 6, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x51, "test_ibstream_9 failed, 0x51 expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0x7b, "test_ibstream_9 failed, 0x7b expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xc5, "test_ibstream_9 failed, 0xc5 expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x5f, "test_ibstream_9 failed, 0x5f expected");
    TEST_ASSERT_MSG(output_buffer[5] == 0x2f, "test_ibstream_9 failed, 0x2f expected");
    TEST_ASSERT_MSG(converter::get_instance().bytes_to_long(output_buffer) == 349968883503, "test_ibstream_9 failed, 349968883503 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 3);
    TEST_ASSERT_MSG(output_buffer.size() == 3, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0x02, "test_ibstream_9 failed, 0x02 expected");
    TEST_ASSERT_MSG(converter::get_instance().bytes_to_int(output_buffer) == 2, "test_ibstream_9 failed, 2 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x5b, "test_ibstream_9 failed, 0x5b expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 2);
    TEST_ASSERT_MSG(output_buffer.size() == 2, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(converter::get_instance().bytes_to_int(output_buffer) == 0, "test_ibstream_9 failed, 0 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 2);
    TEST_ASSERT_MSG(output_buffer.size() == 2, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(converter::get_instance().bytes_to_int(output_buffer) == 0, "test_ibstream_9 failed, 0 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_9 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_9 failed, 0x00 expected");
    TEST_ASSERT_MSG(codec.gremaining() == 0, "test_ibstream_9 failed, wrong remaining length");

    //std::clog << ">>> test_ibstream_9 done" << std::endl;
  }

  /**
   * @brief Test case for UtTerminateDenmEvent
   * @see helpers::ibstream::read_bits
   * @see helpers::ibstream::gcount
   * @see helpers::ibstream::gremaining
   */
  void test_ibstream_10() {
    std::vector<unsigned char> input_buffer = converter::get_instance().hexa_to_bytes("14000082350002");
    helpers::ibstream codec(input_buffer, (8 * sizeof(uint8_t)) * input_buffer.size());
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_10 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x14, "test_ibstream_10 failed, 0x14 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 4);
    TEST_ASSERT_MSG(output_buffer.size() == 4, "test_ibstream_10 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_10 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x00, "test_ibstream_10 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0x82, "test_ibstream_10 failed, 0x82 expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0x35, "test_ibstream_10 failed, 0x35 expected");
    TEST_ASSERT_MSG(converter::get_instance().bytes_to_int(output_buffer) == 33333, "test_ibstream_10 failed, 3333 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t) * 2);
    TEST_ASSERT_MSG(output_buffer.size() == 2, "test_ibstream_10 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_10 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x02, "test_ibstream_10 failed, 0x02 expected");
    TEST_ASSERT_MSG(converter::get_instance().bytes_to_int(output_buffer) == 2, "test_ibstream_10 failed, 2 expected");
    TEST_ASSERT_MSG(codec.gremaining() == 0, "test_ibstream_10 failed, wrong remaining length");

    //std::clog << ">>> test_ibstream_10 done" << std::endl;
  }

  /**
   * @brief Test case for GeoNetworking::BasicHeader
   * @see helpers::ibstream::read_bits
   * @see helpers::ibstream::gcount
   * @see helpers::ibstream::gremaining
   */
  void test_ibstream_11() {
    std::vector<unsigned char> input_buffer = converter::get_instance().hexa_to_bytes("01231b01");
    helpers::ibstream codec(input_buffer, (8 * sizeof(uint8_t)) * input_buffer.size());
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, 4);
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x00, "test_ibstream_11 failed, 0x00 expected");
    codec.read_bits(output_buffer, 4);
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x01, "test_ibstream_11 failed, 0x01 expected");
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x23, "test_ibstream_11 failed, 0x23 expected");
    codec.read_bits(output_buffer, 6);
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x06, "test_ibstream_11 failed, 0x06 expected");
    codec.read_bits(output_buffer, 2);
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x03, "test_ibstream_11 failed, 0x03 expected");    
    codec.read_bits(output_buffer, 8 * sizeof(uint8_t));
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_11 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x01, "test_ibstream_11 failed, 0x01 expected");
    TEST_ASSERT_MSG(codec.gremaining() == 0, "test_ibstream_11 failed, wrong remaining length");

    //std::clog << ">>> test_ibstream_11 done" << std::endl;
  }
  
  /**
   * @brief Test case for @see helpers::ibstream::read_bits
   * @see helpers::ibstream::seekg
   */
  void test_ibstream_12() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xff, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream codec(input_buffer, 32 + 4);
    std::vector<unsigned char> output_buffer;
    codec.read_bits(output_buffer, codec.gcount());
    TEST_ASSERT_MSG(output_buffer.size() == input_buffer.size(), "test_ibstream_12 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_ibstream_12 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xff, "test_ibstream_12 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_ibstream_12 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_ibstream_12 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_ibstream_12 failed, 0x70 expected");
    codec.seekg(3);
    uint8_t value;
    codec.read<uint8_t>(value);
    TEST_ASSERT_MSG(value == 0x57, "test_ibstream_12 failed, 0xa7 expected");
    codec.seekg(3);
    output_buffer = codec.peek(4);
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_12 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x05, "test_ibstream_12 failed, 0x0a expected");
    codec.seekg(4);
    output_buffer = codec.peek(4);
    TEST_ASSERT_MSG(output_buffer.size() == 1, "test_ibstream_12 failed, wrong length");
    TEST_ASSERT_MSG(output_buffer[0] == 0x0a, "test_ibstream_12 failed, 0x0a expected");
    
    //std::clog << ">>> test_ibstream_12 done" << std::endl;
  }
  
  /**
   * @brief Test case for @see helpers::ibstream::read_bits
   * @see helpers::ibstream::is_equal
   */
  void test_ibstream_13() {
    std::vector<unsigned char> input_buffer = { 0xca, 0xff, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::ibstream buffer1(input_buffer, 32 + 4);
    TEST_ASSERT_MSG(buffer1 == buffer1, "test_ibstream_13 failed, buffer is not equal to itself");
    input_buffer[3] =0xcd; // This value is different
    helpers::ibstream buffer2(input_buffer, 32 + 4);
    TEST_ASSERT_MSG(buffer1 != buffer2, "test_ibstream_13 failed, buffer1 is not equal to buffer2");
    
    //std::clog << ">>> test_ibstream_13 done" << std::endl;
  }
  
}; // End of class ibstream_test_suite

/**
 * @class helpers::obstream test suite implementation
 */
class obstream_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  obstream_test_suite() {
    TEST_ADD(obstream_test_suite::test_obstream_1);
    // TEST_ADD(obstream_test_suite::test_obstream_2);
    // TEST_ADD(obstream_test_suite::test_obstream_3);
    // TEST_ADD(obstream_test_suite::test_obstream_4);
    // TEST_ADD(obstream_test_suite::test_obstream_5);
    // TEST_ADD(obstream_test_suite::test_obstream_6);
    // TEST_ADD(obstream_test_suite::test_obstream_7);
    // TEST_ADD(obstream_test_suite::test_obstream_8);  
    // TEST_ADD(obstream_test_suite::test_obstream_9);
    // TEST_ADD(obstream_test_suite::test_obstream_10);
    // TEST_ADD(obstream_test_suite::test_obstream_11);
    // TEST_ADD(obstream_test_suite::test_obstream_12);
  } // End of ctor
	
private:

  /**
   * @brief Test case 
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_1() {
    helpers::obstream codec;
    std::vector<unsigned char> output_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    codec.write_bits(output_buffer, 36);
    TEST_ASSERT_MSG(codec.gcount() == 36, "test_obstream_1 failed, wrong length");
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_1 failed, 0xca expected");
    // TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_1 failed, 0xfe expected");
    // TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_obstream_1 failed, 0xde expected");
    // TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_obstream_1 failed, 0xca expected");
    // TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_obstream_1 failed, 0x70 expected"); 

    //std::clog << ">>> test_obstream_1 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * Check that buffer capacity is increased according to the required size
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_2() {
    helpers::obstream codec(4);
    std::vector<unsigned char> output_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    codec.write_bits(output_buffer, 36);
    TEST_ASSERT_MSG(codec.gcount() == 36, "test_obstream_2 failed, wrong length");
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_2 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_2 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_obstream_2 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_obstream_2 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_obstream_2 failed, 0x70 expected");
    
    //std::clog << ">>> test_obstream_2 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_3() {
    helpers::obstream codec;
    std::vector<unsigned char> output_buffer = { 0x0c };
    codec.write_bits(output_buffer, 4);
    output_buffer = { 0x0a };
    codec.write_bits(output_buffer, 4);
    output_buffer = { 0x0f };
    codec.write_bits(output_buffer, 4);
    output_buffer = { 0x0e };
    codec.write_bits(output_buffer, 4);
    output_buffer = { 0x0d };
    codec.write_bits(output_buffer, 4);
    TEST_ASSERT_MSG(codec.gcount() == 20, "test_obstream_3 failed, wrong length");
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_3 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_3 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xd0, "test_obstream_3 failed, 0xd0 expected");
    
    //std::clog << ">>> test_obstream_3 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_4() {
    helpers::obstream codec;
    std::vector<unsigned char> output_buffer = { 0x06 };
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
    TEST_ASSERT_MSG(codec.gcount() == 36, "test_obstream_4 failed, wrong length");
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_4 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_4 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_obstream_4 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_obstream_4 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_obstream_4 failed, 0x70 expected");
    
    //std::clog << ">>> test_obstream_4 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_5() {
    helpers::obstream codec;
    std::vector<unsigned char> output_buffer = { 0x00 };
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
    TEST_ASSERT_MSG(codec.gcount() == 32, "test_obstream_5 failed, wrong length");
    helper::get_instance().hexa_dump(codec.to_bytes(), 0, codec.gcount() / 8);
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());    
    TEST_ASSERT_MSG(output_buffer[0] == 0x01, "test_obstream_5 failed, 0x01 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x23, "test_obstream_5 failed, 0x23 expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0x1b, "test_obstream_5 failed, 0x1b expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0x01, "test_obstream_5 failed, 0x01 expected");
    
    //std::clog << ">>> test_obstream_5 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_6() {
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
    TEST_ASSERT_MSG(codec.gcount() == 36, "test_obstream_6 failed, wrong length");
    std::vector<unsigned char> output_buffer(codec.to_bytes());    
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_6 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_6 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_obstream_6 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_obstream_6 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_obstream_6 failed, 0x70 expected");
    
    //std::clog << ">>> test_obstream_6 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_7() {
    helpers::obstream codec;
    codec.write<uint8_t>(0x10);
    codec.write<uint8_t>(0xf0);
    codec.write<uint64_t>(349968883503, (8 * sizeof(uint8_t) * 6));
    codec.write<uint32_t>(2, (8 * sizeof(uint8_t) * 3));    
    // std::vector<unsigned char> convert = converter::get_instance().long_to_bytes(349968883503);
    // codec.write_bits(std::vector<unsigned char>(convert.cbegin() + 2, convert.cend()), (8 * sizeof(uint8_t)) * 6);
    // convert = converter::get_instance().int_to_bytes(2);
    // codec.write_bits(std::vector<unsigned char>(convert.cbegin() + 1, convert.cend()), (8 * sizeof(uint8_t)) * 3);
    TEST_ASSERT_MSG(codec.gcount() == (8 * sizeof(uint8_t)) * 11, "test_obstream_7 failed, wrong length");
    helper::get_instance().hexa_dump(codec.to_bytes(), 0, codec.gcount() / 8);
    std::vector<unsigned char> output_buffer(codec.to_bytes());    
    TEST_ASSERT_MSG(output_buffer[0] == 0x10, "test_obstream_7 failed, 0x10 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xf0, "test_obstream_7 failed, 0xf0 expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0x00, "test_obstream_7 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0x51, "test_obstream_7 failed, 0x51 expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x7b, "test_obstream_7 failed, 0x7b expected");
    TEST_ASSERT_MSG(output_buffer[5] == 0xc5, "test_obstream_7 failed, 0xc5 expected");
    TEST_ASSERT_MSG(output_buffer[6] == 0x5f, "test_obstream_7 failed, 0x5f expected");
    TEST_ASSERT_MSG(output_buffer[7] == 0x2f, "test_obstream_7 failed, 0x2f expected");
    TEST_ASSERT_MSG(output_buffer[8] == 0x00, "test_obstream_7 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[9] == 0x00, "test_obstream_7 failed, 0x00 expected");
    TEST_ASSERT_MSG(output_buffer[10] == 0x02, "test_obstream_7 failed, 0x02 expected");
    
    //std::clog << ">>> test_obstream_7 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write<float>
   */
  void test_obstream_8() {
    helpers::obstream codec;
    codec.write<float>(3.141519);
    std::vector<unsigned char> output_buffer(codec.to_bytes());    
    TEST_ASSERT_MSG(output_buffer[0] == 0xa6, "test_obstream_8 failed, 0xa6 expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0x0e, "test_obstream_8 failed, 0x0e expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0x49, "test_obstream_8 failed, 0x49 expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0x40, "test_obstream_8 failed, 0x40 expected");
    
    //std::clog << ">>> test_obstream_8 done" << std::endl;
  }

  /**
   * @brief Test case
   * @see helpers::obstream::write<float>
   */
  void test_obstream_9() {
    std::vector<unsigned char> output_buffer = { 0xca, 0xfe, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::obstream codec;
    //    codec.write<std::vector<unsigned char>& >(output_buffer, 36);
    codec.write_bits(output_buffer, 36);
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_9 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_9 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_obstream_9 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_obstream_9 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_obstream_9 failed, 0x70 expected");
    
    //std::clog << ">>> test_obstream_9 done" << std::endl;
  }

  /**
   * @brief Test case
   * @see helpers::obstream::seekp
   */
  void test_obstream_10() {
    std::vector<unsigned char> output_buffer = { 0xca, 0xff, 0xde, 0xca, 0x70 }; // 32 + 4 bits length
    helpers::obstream codec;
    //    codec.write<std::vector<unsigned char>& >(output_buffer, 36);
    codec.write_bits(output_buffer, 36);
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_10 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xff, "test_obstream_10 failed, 0xff expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_obstream_10 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_obstream_10 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x70, "test_obstream_10 failed, 0x70 expected");
    codec.seekp(1 * (8 * sizeof(uint8_t)));
    codec.write<uint8_t>(0xfe);
    output_buffer.assign(codec.to_bytes().cbegin(), codec.to_bytes().cend());
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_10 failed, 0xfe expected");
    
    //std::clog << ">>> test_obstream_10 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   */
  void test_obstream_11() {
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
    TEST_ASSERT_MSG(codec.gcount() == 48, "test_obstream_11 failed, wrong length");
    std::vector<unsigned char> output_buffer(codec.to_bytes());    
    TEST_ASSERT_MSG(output_buffer[0] == 0xca, "test_obstream_11 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[1] == 0xfe, "test_obstream_11 failed, 0xfe expected");
    TEST_ASSERT_MSG(output_buffer[2] == 0xde, "test_obstream_11 failed, 0xde expected");
    TEST_ASSERT_MSG(output_buffer[3] == 0xca, "test_obstream_11 failed, 0xca expected");
    TEST_ASSERT_MSG(output_buffer[4] == 0x7f, "test_obstream_11 failed, 0x7f expected");
    TEST_ASSERT_MSG(output_buffer[5] == 0xaa, "test_obstream_11 failed, 0xaa expected");
    
    //std::clog << ">>> test_obstream_11 done" << std::endl;
  }
  
  /**
   * @brief Test case
   * @see helpers::obstream::write_bits
   * @see helpers::ibstream::gcount
   * @see helpers::ibstream::is_equal
   */
  void test_obstream_12() {
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
    TEST_ASSERT_MSG(buffer1.gcount() == 48, "test_obstream_12 failed, wrong length");
    TEST_ASSERT_MSG(buffer1 == buffer1, "test_obstream_12 failed, buffer is not equal to itself");
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
    TEST_ASSERT_MSG(buffer2.gcount() == 48, "test_obstream_12 failed, wrong length");
    std::vector<unsigned char> output_buffer(buffer2.to_bytes());    
    TEST_ASSERT_MSG(buffer1 != buffer2, "test_obstream_12 failed, buffer1 is equal to buffer2");
    //std::clog << ">>> test_obstream_12 done" << std::endl;
  }
  
}; // End of class obstream

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
    // ts.add(unique_ptr<Test::Suite>(new keyboard_test_suite));
    // ts.add(unique_ptr<Test::Suite>(new runnable_test_suite));
    // ts.add(unique_ptr<Test::Suite>(new get_opt_test_suite));
    // ts.add(unique_ptr<Test::Suite>(new helper_test_suite));
    // ts.add(unique_ptr<Test::Suite>(new date_time_test_suite));
    // ts.add(unique_ptr<Test::Suite>(new ibstream_test_suite));
    ts.add(unique_ptr<Test::Suite>(new obstream_test_suite));

    // Run the tests
    unique_ptr<Test::Output> output(cmdline(p_argc, p_argv));
    ts.run(*output, true);

    Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
    if (html) {
      html->generate(clog, true, "Helper test suite");
    }
  } catch (...) {
    clog << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
