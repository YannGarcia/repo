/**
 * @file    testlib.cpp
 * @brief   HTTP server test suite.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <gtest.h>
#define ASSERT_TRUE_MSG(exp1, msg) ASSERT_TRUE(exp1) << msg

#include "gps_device_factory.hh"

using namespace std;

/**
 * @class NMEA 2000 test suite implementation
 */
class nmea2000_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
  /**
   * @brief Test case for @see gps_parser_factory::create
   */
  TEST(nmea2000_test_suite, nmea2000_1) {
    ASSERT_NO_THROW(gps_parser_factory::get_instance().create(parsers_t::nmea_2000)); 
  }  
  
  /**
   * @brief Test case for @see gps_parser::process
   * @see gps_parser_factory:::gps_parser::process_rmc
   */
  TEST(nmea2000_test_suite, nmea2000_2) {
    std::shared_ptr<gps_parser> p(gps_parser_factory::get_instance().create()); // NMEA 2000
    ASSERT_TRUE_MSG(p->process("$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57\r\n") == 0, "$GPRMC frame processing failed");
    std::map<uint8_t, std::string> & values = (std::map<uint8_t, std::string> &)p->get_result();
    ASSERT_TRUE_MSG(values.size() == (uint32_t)gps_parser::yawrate_idx + 1, "Invalid values length");
    ASSERT_TRUE(values[gps_parser::utc_time_idx] == "091202 083559.00");
    ASSERT_TRUE(values[gps_parser::latitude_idx] == "4717.11437 N");
    ASSERT_TRUE(values[gps_parser::longitude_idx] == "00833.91522 E");
    ASSERT_TRUE(values[gps_parser::speed_idx] == "0.004");
    ASSERT_TRUE(values[gps_parser::altitude_idx].empty());
    ASSERT_TRUE(values[gps_parser::heading_idx] == "77.52");
    ASSERT_TRUE(values[gps_parser::yawrate_idx].empty());
    ASSERT_TRUE_MSG(p->process("$GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62\r\n") != 0, "$GPRMC frame processing shall fail due to invalid UTC Time, Time of position fix format");
  }  
  
  /**
   * @brief Test case for @see gps_parser::process
   * @see gps_parser_factory:::gps_parser::process_gga
   */
  TEST(nmea2000_test_suite, nmea2000_3) {
    std::shared_ptr<gps_parser> p(gps_parser_factory::get_instance().create()); // NMEA 2000
    ASSERT_TRUE_MSG(p->process("$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B\r\n") == 0, "$GPGGA frame processing failed");
    std::map<uint8_t, std::string> & values = (std::map<uint8_t, std::string> &)p->get_result();
    ASSERT_TRUE_MSG(values.size() == (uint32_t)gps_parser::yawrate_idx + 1, "Invalid values length");
    ASSERT_TRUE(values[gps_parser::utc_time_idx].empty());
    ASSERT_TRUE(values[gps_parser::latitude_idx].empty());
    ASSERT_TRUE(values[gps_parser::longitude_idx].empty());
    ASSERT_TRUE(values[gps_parser::speed_idx].empty());
    ASSERT_TRUE(values[gps_parser::altitude_idx] == "499.6");
    ASSERT_TRUE(values[gps_parser::heading_idx].empty());
    ASSERT_TRUE(values[gps_parser::yawrate_idx].empty());
    ASSERT_TRUE_MSG(p->process("$GPGGA,092725.00,4717.11399,H,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B\r\n") != 0, "$GPGGA frame processing shall fail due to invalid Latitude direction");
  }  
  
/**
 * @class HTTP server test suite implementation
 */
class gps_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
  /**
   * @brief Test case for @see gps_device_factory
   * @see gps_device_factory:::gps_device_factory::add_gps
   * @see gps_device_factory:::gps_device_factory::get_gps
   * @see gps_device_factory:::gps_device_factory::remove_gps
   */
  TEST(gps_test_suite, gps_device_1) {
    ASSERT_NO_THROW(gps_device_factory::get_instance().add_gps("/dev/ttyO4")); // UBlox/NMEA 2000 
    ASSERT_NO_THROW(gps_device_factory::get_instance().get_gps("/dev/ttyO4"));
    ASSERT_NO_THROW(gps_device_factory::get_instance().remove_gps("/dev/ttyO4"));
    TEST_THROWS(gps_device_factory::get_instance().get_gps("/dev/ttyO4"), std::invalid_argument);
  }  
  
  /**
   * @brief Test case for @see gps_device_factory
   * @see gps_device_factory:::gps_device_factory::get_result
   */
  TEST(gps_test_suite, gps_device_2) {
    std::string s("/dev/ttyO4");
    gps_device_factory::get_instance().add_gps(s); // UBlox/NMEA 2000 
    gps_device_factory::get_instance().get_gps(s).start();
    int32_t counter = 0;
    while (counter++ < 60) {
      const std::map<uint8_t, std::string> & values = gps_device_factory::get_instance().get_gps(s).get_result();
      ASSERT_TRUE_MSG(values.size() == (uint32_t)gps_parser::yawrate_idx + 1, "Invalid values length");
      std::this_thread::sleep_for(std::chrono::seconds(1));
    } // End of 'while' statement
    gps_device_factory::get_instance().get_gps(s).stop();
    ASSERT_NO_THROW(gps_device_factory::get_instance().remove_gps(s));
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
