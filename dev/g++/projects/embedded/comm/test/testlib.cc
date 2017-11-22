/**
 * @File    testlib.cpp
 * @brief   Types converter test suite.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

#include <gtest.h>
#define ASSERT_TRUE_MSG(exp1, msg) ASSERT_TRUE(exp1) << msg

#include "socket_address.hh"
#include "channel_manager.hh"

#include "runnable.hh"

#define LOCAL_IPv4_ADDRESS "10.0.2.15"
#define LOCAL_IPv4_PORT    12345
#define PEER_IPv4_PORT     12346
#define ANY_IPv4_ADDRESS "0.0.0.0"
#define PEER_IPv4_ADDRESS "10.0.2.15"

#define LOCAL_IPv6_ADDRESS "::1"
#define PEER_IPv6_ADDRESS "::1"

using namespace std;

static int g_index = 0;
std::vector<int8_t> g_wait_cursor { '|', '/', '-', '\\', '|', '/', '-', '\\' };

/**
 * @class Socket address test suite implementation
 */
class socket_address_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
		
/**
 * @brief Test case for @see ipv4_address
 */
TEST(socket_address_test_suite, ipv4_address_1) {
  socket_address addr(std::string("localhost"), static_cast<const uint16_t>(12345));
  ASSERT_TRUE(addr.is_ipv4());
  ASSERT_TRUE(!addr.is_ipv6());
  std::vector<uint8_t> to_bytes = addr.to_numeric();
  ASSERT_TRUE(to_bytes.size() == 4);
  uint8_t expected_result[] = { 0x7f, 0x00, 0x00, 0x01 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  ASSERT_TRUE(addr.port() == 12345);
  ASSERT_TRUE(addr.to_string() == "127.0.0.1");
  ASSERT_TRUE(addr.addr() != NULL);
} // End of method test_ipv4_address_1
  
  /**
   * @brief Test case for @see ipv4_address
   */
TEST(socket_address_test_suite, ipv4_address_2) {
  socket_address addr(std::string("127.0.0.1"), static_cast<const uint16_t>(12347));
  ASSERT_TRUE(addr.is_ipv4());
  ASSERT_TRUE(!addr.is_ipv6());
  ASSERT_TRUE(addr.port() == 12347);
  ASSERT_TRUE(addr.to_string() == "127.0.0.1");
  ASSERT_TRUE(addr.addr() != NULL);
  ASSERT_TRUE(addr.is_localhost());
  ASSERT_TRUE(!addr.is_multicast());
  ASSERT_TRUE(!addr.is_broadcast());
} // End of method test_ipv4_address_2
  
  /**
   * @brief Test case for @see ipv4_address
   */
TEST(socket_address_test_suite, ipv4_address_3) {
  socket_address addr(std::string("www.etsi.org"), static_cast<const uint16_t>(443));
  ASSERT_TRUE(addr.is_ipv4());
  ASSERT_TRUE(!addr.is_ipv6());
  std::vector<uint8_t> to_bytes = addr.to_numeric();
  ASSERT_TRUE(to_bytes.size() == 4);
  uint8_t expected_result[] = { 195, 238, 226, 27 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  ASSERT_TRUE(addr.port() == 443);
  ASSERT_TRUE(addr.to_string() == "195.238.226.27");
  ASSERT_TRUE(addr.addr() != NULL);
  ASSERT_TRUE(!addr.is_localhost());
  ASSERT_TRUE(!addr.is_multicast());
  ASSERT_TRUE(!addr.is_broadcast());
} // End of method test_ipv4_address_3
  
  /**
   * @brief Test case for @see ipv4_address
   */
TEST(socket_address_test_suite, ipv4_address_4) {
  socket_address addr(std::string("195.238.226.27"), static_cast<const uint16_t>(443));
  std::vector<uint8_t> to_bytes = addr.to_numeric();
  ASSERT_TRUE(to_bytes.size() == 4);
  uint8_t expected_result[] = { 195, 238, 226, 27 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  ASSERT_TRUE(addr.to_string() == "195.238.226.27");
} // End of method test_ipv4_address_4
  
  /**
   * @brief Test case for @see ipv6_address::ctor
   */
TEST(socket_address_test_suite, ipv6_address_1) {
  socket_address addr(std::string("::1"), static_cast<const uint16_t>(12345));
  ASSERT_TRUE(!addr.is_ipv4());
  ASSERT_TRUE(addr.is_ipv6());
  ASSERT_TRUE(addr.is_localhost());
  ASSERT_TRUE(!addr.is_multicast());
  ASSERT_TRUE(!addr.is_broadcast());
  std::vector<uint8_t> to_bytes = addr.to_numeric();
  ASSERT_TRUE(to_bytes.size() == 16);
  uint8_t expected_result[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  ASSERT_TRUE(addr.to_string() == "::1");
} // End of method test_ipv6_address_1
  
  /**
   * @brief Test case for @see ipv6_address::ctor
   */
TEST(socket_address_test_suite, ipv6_address_2) {
  socket_address addr(std::string("par03s13-in-x10.1e100.net"), static_cast<const uint16_t>(12345));
  ASSERT_TRUE(!addr.is_ipv4());
  ASSERT_TRUE(addr.is_ipv6());
  ASSERT_TRUE(!addr.is_localhost());
  ASSERT_TRUE(!addr.is_multicast());
  ASSERT_TRUE(!addr.is_broadcast());
  std::vector<uint8_t> to_bytes = addr.to_numeric();
  ASSERT_TRUE(to_bytes.size() == 16);
  uint8_t expected_result[] = { 0x2a, 0x00, 0x14, 0x50, 0x40, 0x07, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
  ASSERT_TRUE(addr.to_string() == "2a00:1450:4007:806::1010");
} // End of method test_ipv6_address_2
  
/**
 * @class Channel manager/RAW test suite implementation
 */
class channel_manager_raw_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
  
/**
 * @brief Test case for @see channel_manager::create_channel
 * @see channel_manager::get_channel
 * @see channel_manager::remove_channel
 */
TEST(channel_manager_raw_test_suite, create_channel_raw_1) {
  // Create RAW channel
  socket_address addr(std::string("0.0.0.0"), 0);
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::raw, addr);
  ASSERT_TRUE(channel != -1);
  // Set interface
  channel_manager::get_instance().get_channel(channel).set_nic_name(std::string("enp0s3"));
  // Get channel
  ASSERT_NO_THROW(channel_manager::get_instance().get_channel(channel));
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
  ASSERT_THROW(channel_manager::get_instance().get_channel(channel), std::out_of_range);
} // End of method test_create_channel_raw_1
  
  /**
   * @brief Test case for @see channel_manager::create_channel
   * Use command netcat -vv -u -l -p12345 LOCAL_IPv4_ADDRESS to start an UDP listener on port 12345
   * Use command sudo tcpdump -vvv -ienp0s3 ether proto 0x8947
   * @see channel_manager::connect
   * @see channel_manager::write
   * @see channel_manager::disconnect
   */
TEST(channel_manager_raw_test_suite, create_channel_raw_2) {
  // Create RAW channel
  socket_address addr(std::string("0.0.0.0"), 0);
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::raw, addr);
  ASSERT_TRUE(channel != -1);
  // Get channel
  ASSERT_NO_THROW(channel_manager::get_instance().get_channel(channel));
    
  // Set interface
  int32_t result = channel_manager::get_instance().get_channel(channel).set_nic_name(std::string("enp0s3"));
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5c, 0x26, 0x0a, 0x83, 0x5C, 0xb4, 0x89, 0x47, 0x01, 0x00 };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(5));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_raw_2
  
  /**
   * @brief Test case for @see channel_manager::create_channel
   * Use command sudo tcpdump -vvv -ienp0s3 ether proto 0x8947
   * @see channel_manager::connect
   * @see channel_manager::write
   * @see channel_manager::read
   * @see channel_manager::disconnect
   */
TEST(channel_manager_raw_test_suite, create_channel_raw_3) {
  // Create RAW channel
  socket_address addr(std::string("0.0.0.0"), 0);
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::raw, addr);
  ASSERT_TRUE(channel != -1);
  // Get channel
  ASSERT_NO_THROW(channel_manager::get_instance().get_channel(channel));
    
  // Set interface
  int32_t result = channel_manager::get_instance().get_channel(channel).set_nic_name(std::string("enp0s3"));
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5c, 0x26, 0x0a, 0x83, 0x5C, 0xb4, 0x89, 0x47, 0x01, 0x00 };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(5));
    
  // Read data
  buffer.clear();
  buffer.assign(64, 0x00);
  result = channel_manager::get_instance().get_channel(channel).read(buffer);
  ASSERT_TRUE(result != -1);
  cout << "Buffer size: " << (int)buffer.size() << endl;
  //ASSERT_TRUE(buffer.size() == 6);
  //std::vector<uint8_t> expected_result = { 'H', 'i', ' ', 'm', 'a', 'n', 0x0a };
  //ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), expected_result.begin()));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_raw_3

/**
 * @class Channel manager/UDP test suite implementation
 */
class channel_manager_udp_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
/**
 * @brief Test case for @see channel_manager::create_channel
 * @see channel_manager::get_channel
 * @see channel_manager::remove_channel
 */
TEST(channel_manager_udp_test_suite, create_channel_udp_1) {
  // Create UDP channel
  socket_address addr(std::string("localhost"), static_cast<const uint16_t>(LOCAL_IPv4_PORT));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::udp, addr);
  ASSERT_TRUE(channel != -1);
    
  // Get channel
  ASSERT_NO_THROW(channel_manager::get_instance().get_channel(channel));
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
  ASSERT_THROW(channel_manager::get_instance().get_channel(channel), std::out_of_range);
} // End of method test_create_channel_udp_1
  
  /**
   * @brief Test case for @see channel_manager::create_channel
   * Use command netcat -vv -u -l -p12345 LOCAL_IPv4_ADDRESS to start an UDP listener on port 12345
   * @see channel_manager::connect
   * @see channel_manager::write
   * @see channel_manager::disconnect
   */
TEST(channel_manager_udp_test_suite, create_channel_udp_2) {
  // Create UDP channel
  socket_address addr(std::string(LOCAL_IPv4_ADDRESS), static_cast<const uint16_t>(LOCAL_IPv4_PORT));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::udp, addr);
    
  // Connect it to host
  int32_t result = channel_manager::get_instance().get_channel(channel).connect();
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0x0d, 0x0a };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(5));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_udp_2
  
  /**
   * @brief Test case for @see channel_manager::create_channel
   * Use command echo "Hi man" | netcat -vv -u -l -p12346 LOCAL_IPv4_ADDRESS to start an UDP listener on port 12346
   * @see channel_manager::connect
   * @see channel_manager::send
   * @see channel_manager::disconnect
   */
TEST(channel_manager_udp_test_suite, create_channel_udp_3) {
  // Create UDP channel
  socket_address addr(std::string(LOCAL_IPv4_ADDRESS), static_cast<const uint16_t>(LOCAL_IPv4_PORT));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::udp, addr);
    
  // Connect it to host
  int32_t result = channel_manager::get_instance().get_channel(channel).connect();
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0x0d, 0x0a };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(5));
    
  // Read data
  buffer.clear();
  buffer.assign(6, 0x00);
  result = channel_manager::get_instance().get_channel(channel).read(buffer);
  ASSERT_TRUE(result != -1);
  cout << "Buffer size: " << (int)buffer.size() << endl;
  ASSERT_TRUE(buffer.size() == 6);
  std::vector<uint8_t> expected_result = { 'H', 'i', ' ', 'm', 'a', 'n', 0x0a };
  ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), expected_result.begin()));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_udp_3
  
  /**
   * @brief Test case for @see channel_manager::create_channel - IPv6
   * @see channel_manager::get_channel
   * @see channel_manager::remove_channel
   */
TEST(channel_manager_udp_test_suite, create_channel_udp_4) {
  // Create UDP channel
  socket_address addr(std::string("::1"), static_cast<const uint16_t>(12345));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::udp, addr);
  ASSERT_TRUE(channel != -1);
    
  // Get channel channel
  ASSERT_NO_THROW(channel_manager::get_instance().get_channel(channel));
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
  ASSERT_THROW(channel_manager::get_instance().get_channel(channel), std::out_of_range);
} // End of method test_create_channel_udp_4
  
  /**
   * @brief Test case for @see channel_manager::create_channel
   * Use command netcat -6 -vv -u -l -p12345 LOCAL_IPv6_ADDRESS to start an UDP listener on port 12345
   * @see channel_manager::connect
   * @see channel_manager::write
   * @see channel_manager::disconnect
   */
TEST(channel_manager_udp_test_suite, create_channel_udp_5) {
  // Create UDP channel
  socket_address addr(std::string(LOCAL_IPv6_ADDRESS), static_cast<const uint16_t>(12345));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::udp, addr);
    
  // Connect it to host
  int32_t result = channel_manager::get_instance().get_channel(channel).connect();
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0x0d, 0x0a };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(5));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_udp_5
  
class thread_ : public runnable {
  socket_address _host_address;
  socket_address _peer_address;
  uint32_t _udp;
  bool _result;
    
public:
  thread_(const socket_address & p_host_address, const socket_address p_peer_address) : _host_address(p_host_address), _peer_address(p_peer_address), _udp(-1), _result(false) {
    _udp = channel_manager::get_instance().create_channel(channel_type::udp, _host_address, _peer_address);
    // channel_manager::get_instance().get_channel(_udp).connect();
  }; // End of ctor
  virtual ~thread_() { };

  void stop() {
    runnable::stop();
    channel_manager::get_instance().get_channel(_udp).disconnect();
    channel_manager::get_instance().remove_channel(_udp);
    _udp = -1;
  };
    
  void run() {
    std::vector<uint8_t> expected_result = { 'H', 'i', ' ', 'm', 'a', 'n', 0x0a };
    std::vector<uint32_t> fds;
    _running = true;
    while(_running) {
      channel_manager::get_instance().poll_channels(1000, fds); // 1000 ms
      std::vector<uint32_t>::iterator it = std::find(fds.begin(), fds.end(), _udp);
      if (it != fds.end()) { // Some data are available for _udp channel
        // Read data
        std::vector<uint8_t> buffer(16, 0xff);
        channel_manager::get_instance().get_channel(*it).read(buffer);
        fds.clear();
	  
        _result = !std::equal(buffer.begin(), buffer.end(), expected_result.begin()); // FIXME I don't understand why to negate std::equal() returns true when buffer content are not equal !!!
        std::clog << "thread_::run: _result = " << _result << std::endl;
        fds.clear();

        // Echo
        channel_manager::get_instance().get_channel(_udp).write(buffer);
      }
    } // End of 'while' statement
  }; // End of method run
    
  const bool get_result() const { return _result; };
}; // End of class thread_
  
/**
 * @class Channel manager/UDP/Thread test suite implementation
 */
class channel_manager_t_udp_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
/**
 * @brief Test case for @see channel_manager::create_channel
 * Use commands:
 * -  netcat -vv -u LOCAL_IPv4_ADDRESS LOCAL_IPv4_PORT
 * -  netcat -vv -u -l -p PEER_IPv4_PORT PEER_IPv4_ADDRESS
 * @see channel_manager::accept_connection
 * @see channel_manager::write
 * @see channel_manager::disconnect
 */
TEST(channel_manager_t_udp_test_suite, udp_thread_channel_1) {
  socket_address host_address(std::string(LOCAL_IPv4_ADDRESS), static_cast<const uint16_t>(LOCAL_IPv4_PORT));
  socket_address peer_address(std::string(PEER_IPv4_ADDRESS), static_cast<const uint16_t>(PEER_IPv4_PORT));
    
  // Start server
  unique_ptr<thread_> udp_manager;
  udp_manager.reset(new thread_(host_address, peer_address));
  udp_manager->start();
    
  // Wait for stop keyword
  while (!udp_manager->get_result()) {
    std::clog << g_wait_cursor[g_index] << '\b';
    g_index = (g_index + 1) % g_wait_cursor.size();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  };
    
  // Stop server
  udp_manager->stop();
  udp_manager.reset();
} // End of method test_udp_thread_channel_1
  
/**
 * @class Channel manager test suite implementation
 */
class channel_manager_tcp_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
/**
 * @brief Test case for @see channel_manager::create_channel
 * @see channel_manager::get_channel
 * @see channel_manager::remove_channel
 */
TEST(channel_manager_tcp_test_suite, create_channel_tcp_1) {
  // Create TCP channel
  socket_address addr(std::string("localhost"), static_cast<const uint16_t>(12348));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::tcp, addr);
  ASSERT_TRUE(channel != -1);
    
  // Get channel channel
  ASSERT_NO_THROW(channel_manager::get_instance().get_channel(channel));
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
  ASSERT_THROW(channel_manager::get_instance().get_channel(channel), std::out_of_range);
} // End of method test_create_channel_tcp_1
  
  /**
   * @brief Test case for @see channel_manager::create_channel
   * Use command netcat -vv -l -p12348 LOCAL_IPv4_ADDRESS to start an TCP listener on port 12348
   * @see channel_manager::connect
   * @see channel_manager::write
   * @see channel_manager::disconnect
   */
TEST(channel_manager_tcp_test_suite, create_channel_tcp_2) {
  // Create TCP channel
  socket_address addr(std::string(LOCAL_IPv4_ADDRESS), static_cast<const uint16_t>(12348));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::tcp, addr);
    
  // Connect it to host
  int32_t result = channel_manager::get_instance().get_channel(channel).connect();
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0x0a };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(1));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_tcp_2
  
  /**
   * @brief Test case for @see channel_manager::create_channel
   * Use command ech "Hi man" | netcat -vv -l -p12348 LOCAL_IPv4_ADDRESS to start an TCP listener on port 12348
   * @see channel_manager::connect
   * @see channel_manager::write
   * @see channel_manager::read
   * @see channel_manager::disconnect
   */
TEST(channel_manager_tcp_test_suite, create_channel_tcp_3) {
  // Create UDP channel
  socket_address addr(std::string(LOCAL_IPv4_ADDRESS), static_cast<const uint16_t>(12348));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::tcp, addr);
    
  // Connect it to host
  int32_t result = channel_manager::get_instance().get_channel(channel).connect();
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0x0a };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(5));
    
  // Read data
  buffer.assign(16, 0xff);
  result = channel_manager::get_instance().get_channel(channel).read(buffer);
  ASSERT_TRUE(result != -1);
  cout << "Buffer size: " << (int)buffer.size() << endl;
  ASSERT_TRUE(buffer.size() == 7);
  uint8_t expected_result[] = { 'H', 'i', ' ', 'm', 'a', 'n', 0x0a };
  ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), expected_result));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_tcp_3
  
  /**
   * @brief Test case for @see channel_manager::create_channel - IPv6
   * @see channel_manager::get_channel
   * @see channel_manager::remove_channel
   */
TEST(channel_manager_tcp_test_suite, create_channel_tcp_4) {
  // Create TCP channel
  socket_address addr(std::string("::1"), static_cast<const uint16_t>(12348));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::tcp, addr);
  ASSERT_TRUE(channel != -1);
    
  // Get channel channel
  ASSERT_NO_THROW(channel_manager::get_instance().get_channel(channel));
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
  ASSERT_THROW(channel_manager::get_instance().get_channel(channel), std::out_of_range);
} // End of method test_create_channel_tcp_4
  
  /**
   * @brief Test case for @see channel_manager::create_channel - IPv6
   * Use command netcat -vv -l -p12348 LOCAL_IPv6_ADDRESS to start an TCP listener on port 12348
   * @see channel_manager::connect
   * @see channel_manager::write
   * @see channel_manager::disconnect
   */
TEST(channel_manager_tcp_test_suite, create_channel_tcp_5) {
  // Create TCP channel
  socket_address addr(std::string(LOCAL_IPv6_ADDRESS), static_cast<const uint16_t>(12348));
  int32_t channel = channel_manager::get_instance().create_channel(channel_type::tcp, addr);
    
  // Connect it to host
  int32_t result = channel_manager::get_instance().get_channel(channel).connect();
  ASSERT_TRUE(result != -1);
    
  // Send data
  std::vector<uint8_t> buffer = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0x0a };
  result = channel_manager::get_instance().get_channel(channel).write(buffer);
  ASSERT_TRUE(result != -1);
    
  std::this_thread::sleep_for(std::chrono::seconds(1));
    
  // Close connection
  result = channel_manager::get_instance().get_channel(channel).disconnect();
  ASSERT_TRUE(result != -1);
    
  // Remove channel
  ASSERT_TRUE(channel_manager::get_instance().remove_channel(channel) != -1);
} // End of method test_create_channel_tcp_5
  
/**
 * @brief Main test program
 * @param[in] p_argc Number of argumrnt
 * @param[in] p_argv List of the arguments
 */
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
