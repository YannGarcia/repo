/**
 * @file    testlib.cpp
 * @brief   Types converter test suite.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-1017 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <thread>
#include <chrono>

#include <gtest.h>
#define ASSERT_TRUE_MSG(exp1, msg) ASSERT_TRUE(exp1) << msg

#include "shared_memory.h"
#include "message_queue.h"
#include "ipc_manager.h"

using namespace std;

/**
 * @class shared_memory_test_suite
 * @brief test suite implementation for Shared Memrory
 */
class shared_memory_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case for @see ipc::shared_memory
 */
TEST(shared_memory_test_suite, sm) {
  shared_memory sm_server(0xcafedeca, 1024);
  ASSERT_TRUE(sm_server.get_fd() != -1);
  ASSERT_TRUE(sm_server.get_base_address() != NULL);
    
  shared_memory sm_client1(0xcafedeca, 1024, 128, ipc_rights::ro);
  ASSERT_TRUE(sm_client1.get_fd() != -1);
  ASSERT_TRUE(sm_client1.get_base_address() != NULL);
    
  shared_memory sm_client2(0xcafedeca, 1024, 256, ipc_rights::rw);
  ASSERT_TRUE(sm_client2.get_fd() != -1);
  ASSERT_TRUE(sm_client2.get_base_address() != NULL);

  ASSERT_TRUE(sm_client1.get_base_address() != sm_client2.get_base_address());

  {    
    vector<unsigned char> data;
    data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
    ASSERT_TRUE_MSG(sm_client2.write(data) == 0, "test_1 failure, invalid return code");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    data.assign(2, 0x00);
    ASSERT_TRUE_MSG(sm_client2.read(data) == 0, "test_1 failure, invalid return code");
    ASSERT_TRUE(data.size() == 2);
    ASSERT_TRUE_MSG(data[0] == 0xca, "test_1 failure, expected value is 0xca");
    ASSERT_TRUE_MSG(data[1] == 0xfe, "test_1 failure, expected value is 0xfe");
  }

  {
    vector<unsigned char> data;
    data.push_back(0xba); data.push_back(0xbe); data.push_back(0x01); data.push_back(0x02); 
    ASSERT_TRUE_MSG(sm_client1.write(data) == 0, "test_1 failure, invalid return code");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    data.assign(2, 0x00);
    ASSERT_TRUE_MSG(sm_client1.read(data) == 0, "test_1 failure, invalid return code");
    ASSERT_TRUE(data.size() == 2);
    ASSERT_TRUE_MSG(data[0] == 0xba, "test_1 failure, expected value is 0xba");
    ASSERT_TRUE_MSG(data[1] == 0xbe, "test_1 failure, expected value is 0xbe");
    ASSERT_TRUE_MSG(data[2] == 0x01, "test_1 failure, expected value is 0x01");
    ASSERT_TRUE_MSG(data[3] == 0x02, "test_1 failure, expected value is 0x02");
  }
}
  
/**
 * @class message_queue_test_suite
 * @brief test suite implementation for Message Queue
 */
class message_queue_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
/**
 * @brief Test case for @see ipc::message_queue
 */
TEST(message_queue_test_suite, mq) {
  message_queue mq_server(0xcafedeca, static_cast<uint64_t>(1));
  ASSERT_TRUE(mq_server.get_fd() != -1);
    
  message_queue mq_client(0xcafedeca, static_cast<uint64_t>(1), ipc_rights::ro);
  ASSERT_TRUE(mq_client.get_fd() != -1);
    
  vector<unsigned char> data;
  data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); data.push_back(0xbe); data.push_back(0xef);
  ASSERT_TRUE_MSG(mq_client.write(data) == 0, "test_1 failure, invalid return code");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  data.assign(6, 0x00);
  ASSERT_TRUE_MSG(mq_client.read(data) == 0, "test_1 failure, invalid return code");
  ASSERT_TRUE(data.size() == 6);
  ASSERT_TRUE_MSG(data[0] == 0xca, "test_1 failure, expected value is 0xca");
  ASSERT_TRUE_MSG(data[1] == 0xfe, "test_1 failure, expected value is 0xfe");
  ASSERT_TRUE_MSG(data[2] == 0xde, "test_1 failure, expected value is 0xde");
  ASSERT_TRUE_MSG(data[3] == 0xca, "test_1 failure, expected value is 0xca");
  ASSERT_TRUE_MSG(data[4] == 0xbe, "test_1 failure, expected value is 0xbe");
  ASSERT_TRUE_MSG(data[5] == 0xef, "test_1 failure, expected value is 0xef");
}
  
/**
 * @class ipc_manager_test_suite
 * @brief test suite implementation for ipc_manager
 */
class ipc_manager_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
/**
 * @brief Test case for @see ipc::ipc_manager
 * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
 */
TEST(ipc_manager_test_suite, ipc_manager_1) {
  // Create share memories
  int32_t ipc_server1 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xcafedeca, 1024);
  ASSERT_TRUE(ipc_server1 != -1);
  int32_t ipc_server2 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xbabebabe, 256);
  ASSERT_TRUE(ipc_server2 != -1);
  ASSERT_TRUE(ipc_server1 != ipc_server2);

  // Get ipcs
  ASSERT_NO_THROW(ipc_manager::get_instance().get_ipc(ipc_server1));
  ASSERT_NO_THROW(ipc_manager::get_instance().get_ipc(ipc_server2));
    
  // Remove ipcs
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_server1) != -1);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_server2) != -1);
}
  
/**
 * @brief Test case for @see ipc::ipc_manager
 * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
 */
TEST(ipc_manager_test_suite, ipc_manager_2) {
  // Create share memories
  int32_t ipc_server = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xcafedeca, 1024);
  ASSERT_TRUE(ipc_server != -1);
  int32_t ipc_client1 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 128);
  ASSERT_TRUE(ipc_client1 != -1);
  int32_t ipc_client2 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 256);
  ASSERT_TRUE(ipc_client1 != -1);

  // Get ipcs
  ASSERT_NO_THROW(ipc_manager::get_instance().get_ipc(ipc_server));
  ASSERT_NO_THROW(ipc_manager::get_instance().get_ipc(ipc_client1));
  ASSERT_NO_THROW(ipc_manager::get_instance().get_ipc(ipc_client2));
    
  // Remove ipcs
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_client1) == 0);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_client1) == -1);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_client2) == 0);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_client2) == -1);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_server) == 0);
}
  
/**
 * @brief Test case for @see ipc::ipc_manager/share_memory
 * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
 */
TEST(ipc_manager_test_suite, ipc_manager_3) {
  // Create share memories
  int32_t ipc_server = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xcafedeca, 1024);
  ASSERT_TRUE(ipc_server != -1);
  int32_t ipc_client1 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 128);
  ASSERT_TRUE(ipc_client1 != -1);
  int32_t ipc_client2 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 256);
  ASSERT_TRUE(ipc_client2 != -1);
    
  vector<unsigned char> data;
  data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
  ASSERT_TRUE_MSG(ipc_manager::get_instance().get_ipc(ipc_client2).write(data) == 0, "test_3 failure, invalid return code");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  data.assign(2, 0x00);
  ASSERT_TRUE_MSG(ipc_manager::get_instance().get_ipc(ipc_client2).read(data) == 0, "test_3 failure, invalid return code");
  ASSERT_TRUE(data.size() == 2);
  ASSERT_TRUE_MSG(data[0] == 0xca, "test_3 failure, expected value is 0xca");
  ASSERT_TRUE_MSG(data[1] == 0xfe, "test_3 failure, expected value is 0xfe");
    
  // Remove ipcs
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_client2) == 0);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_client1) == 0);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_server) == 0 );
}
  
/**
 * @brief Test case for @see ipc::ipc_manager/message_queue
 * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
 */
TEST(ipc_manager_test_suite, ipc_manager_4) {
  // Create share memories
  int32_t ipc_server = ipc_manager::get_instance().create_ipc(ipc_type::message_queue, 0xcafedeca, 1);
  ASSERT_TRUE(ipc_server != -1);
  int32_t ipc_client = ipc_manager::get_instance().create_ipc(ipc_type::message_queue, ipc_rights::ro, 0xcafedeca, 1);
  ASSERT_TRUE(ipc_client != -1);

  vector<unsigned char> data;
  data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
  ASSERT_TRUE_MSG(ipc_manager::get_instance().get_ipc(ipc_client).write(data) == 0, "test_4 failure, invalid return code");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  data.assign(4, 0x00);
  ASSERT_TRUE_MSG(ipc_manager::get_instance().get_ipc(ipc_client).read(data) == 0, "test_4 failure, invalid return code");
  ASSERT_TRUE(data.size() == 4);
  ASSERT_TRUE_MSG(data[0] == 0xca, "test_4 failure, expected value is 0xca");
  ASSERT_TRUE_MSG(data[1] == 0xfe, "test_4 failure, expected value is 0xfe");
  ASSERT_TRUE_MSG(data[2] == 0xde, "test_4 failure, expected value is 0xde");
  ASSERT_TRUE_MSG(data[3] == 0xca, "test_4 failure, expected value is 0xca");
    
  // Remove ipcs
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_client) == 0);
  ASSERT_TRUE(ipc_manager::get_instance().remove_ipc(ipc_server) == 0 );
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
