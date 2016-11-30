/**
 * @file    testlib.cpp
 * @brief   GPS test suite.
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

#include "gps_device_factory.h"

using namespace std;

/**
 * @class NMEA 2000 test suite implementation
 */
class nmea2000_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  nmea2000_test_suite() {
    TEST_ADD(nmea2000_test_suite::test_nmea2000_1);
    TEST_ADD(nmea2000_test_suite::test_nmea2000_2);
    TEST_ADD(nmea2000_test_suite::test_nmea2000_3);
  }
	
private:
  /**
   * @brief Test case for @see gps_parser_factory::create
   */
  void test_nmea2000_1() {
    TEST_THROWS_NOTHING(gps_parser_factory::get_instance().create(parsers_t::nmea_2000)); 
  }  
  
  /**
   * @brief Test case for @see gps_parser::process
   * @see gps_parser_factory:::gps_parser::process_rmc
   */
  void test_nmea2000_2() {
    std::shared_ptr<gps_parser> p(gps_parser_factory::get_instance().create()); // NMEA 2000
    TEST_ASSERT_MSG(p->process("$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57\r\n") == 0, "$GPRMC frame processing failed");
    std::map<uint8_t, std::string> & values = (std::map<uint8_t, std::string> &)p->get_result();
    TEST_ASSERT_MSG(values.size() == (uint32_t)gps_parser::yawrate_idx + 1, "Invalid values length");
    TEST_ASSERT(values[gps_parser::utc_time_idx] == "091202 083559.00");
    TEST_ASSERT(values[gps_parser::latitude_idx] == "4717.11437 N");
    TEST_ASSERT(values[gps_parser::longitude_idx] == "00833.91522 E");
    TEST_ASSERT(values[gps_parser::speed_idx] == "0.004");
    TEST_ASSERT(values[gps_parser::altitude_idx].empty());
    TEST_ASSERT(values[gps_parser::heading_idx] == "77.52");
    TEST_ASSERT(values[gps_parser::yawrate_idx].empty());
    TEST_ASSERT_MSG(p->process("$GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62\r\n") != 0, "$GPRMC frame processing shall fail due to invalid UTC Time, Time of position fix format");
  }  
  
  /**
   * @brief Test case for @see gps_parser::process
   * @see gps_parser_factory:::gps_parser::process_gga
   */
  void test_nmea2000_3() {
    std::shared_ptr<gps_parser> p(gps_parser_factory::get_instance().create()); // NMEA 2000
    TEST_ASSERT_MSG(p->process("$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B\r\n") == 0, "$GPGGA frame processing failed");
    std::map<uint8_t, std::string> & values = (std::map<uint8_t, std::string> &)p->get_result();
    TEST_ASSERT_MSG(values.size() == (uint32_t)gps_parser::yawrate_idx + 1, "Invalid values length");
    TEST_ASSERT(values[gps_parser::utc_time_idx].empty());
    TEST_ASSERT(values[gps_parser::latitude_idx].empty());
    TEST_ASSERT(values[gps_parser::longitude_idx].empty());
    TEST_ASSERT(values[gps_parser::speed_idx].empty());
    TEST_ASSERT(values[gps_parser::altitude_idx] == "499.6");
    TEST_ASSERT(values[gps_parser::heading_idx].empty());
    TEST_ASSERT(values[gps_parser::yawrate_idx].empty());
    TEST_ASSERT_MSG(p->process("$GPGGA,092725.00,4717.11399,H,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B\r\n") != 0, "$GPGGA frame processing shall fail due to invalid Latitude direction");
  }  
  
};

/**
 * @class GPS test suite implementation
 */
class gps_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  gps_test_suite() {
    TEST_ADD(gps_test_suite::test_gps_device_1);
    TEST_ADD(gps_test_suite::test_gps_device_2);
  }
	
private:
  /**
   * @brief Test case for @see gps_device_factory
   * @see gps_device_factory:::gps_device_factory::add_gps
   * @see gps_device_factory:::gps_device_factory::get_gps
   * @see gps_device_factory:::gps_device_factory::remove_gps
   */
  void test_gps_device_1() {
    TEST_THROWS_NOTHING(gps_device_factory::get_instance().add_gps("/dev/ttyO4")); // UBlox/NMEA 2000 
    TEST_THROWS_NOTHING(gps_device_factory::get_instance().get_gps("/dev/ttyO4"));
    TEST_THROWS_NOTHING(gps_device_factory::get_instance().remove_gps("/dev/ttyO4"));
    TEST_THROWS(gps_device_factory::get_instance().get_gps("/dev/ttyO4"), std::invalid_argument);
  }  
  
  /**
   * @brief Test case for @see gps_device_factory
   * @see gps_device_factory:::gps_device_factory::get_result
   */
  void test_gps_device_2() {
    std::string s("/dev/ttyO4");
    gps_device_factory::get_instance().add_gps(s); // UBlox/NMEA 2000 
    gps_device_factory::get_instance().get_gps(s).start();
    int32_t counter = 0;
    while (counter++ < 60) {
      const std::map<uint8_t, std::string> & values = gps_device_factory::get_instance().get_gps(s).get_result();
      TEST_ASSERT_MSG(values.size() == (uint32_t)gps_parser::yawrate_idx + 1, "Invalid values length");
      std::this_thread::sleep_for(std::chrono::seconds(1));
    } // End of 'while' statement
    gps_device_factory::get_instance().get_gps(s).stop();
    TEST_THROWS_NOTHING(gps_device_factory::get_instance().remove_gps(s));
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
    ts.add(unique_ptr<Test::Suite>(new nmea2000_test_suite));
    ts.add(unique_ptr<Test::Suite>(new gps_test_suite));

    // Run the tests
    unique_ptr<Test::Output> output(cmdline(p_argc, p_argv));
    ts.run(*output, true);

    Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
    if (html) {
      html->generate(cout, true, "GPS test suite");
    }
  } catch (...) {
    cout << "unexpected exception encountered\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
