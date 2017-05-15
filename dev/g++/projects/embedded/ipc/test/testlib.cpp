/**
 * @file    testlib.cpp
 * @brief   Types converter test suite.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <thread>
#include <chrono>

#include <cpptest.h>

#include "shared_memory.h"
#include "message_queue.h"
#include "ipc_manager.h"

using namespace std;

/**
 * @class shared_memory_test_suite
 * @brief test suite implementation for Shared Memrory
 */
class shared_memory_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  shared_memory_test_suite() {
    TEST_ADD(shared_memory_test_suite::test_1);
  }
	
private:
  /**
   * @brief Test case for @see ipc::shared_memory
   */
  void test_1() {
    shared_memory sm_server(0xcafedeca, 1024);
    TEST_ASSERT(sm_server.get_fd() != -1);
    TEST_ASSERT(sm_server.get_base_address() != NULL);
    
    shared_memory sm_client1(0xcafedeca, 1024, 128, ipc_rights::ro);
    TEST_ASSERT(sm_client1.get_fd() != -1);
    TEST_ASSERT(sm_client1.get_base_address() != NULL);
    
    shared_memory sm_client2(0xcafedeca, 1024, 256, ipc_rights::rw);
    TEST_ASSERT(sm_client2.get_fd() != -1);
    TEST_ASSERT(sm_client2.get_base_address() != NULL);

    TEST_ASSERT(sm_client1.get_base_address() != sm_client2.get_base_address());

    {    
      vector<unsigned char> data;
      data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
      TEST_ASSERT_MSG(sm_client2.write(data) == 0, "test_1 failure, invalid return code");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      data.assign(2, 0x00);
      TEST_ASSERT_MSG(sm_client2.read(data) == 0, "test_1 failure, invalid return code");
      TEST_ASSERT(data.size() == 2);
      TEST_ASSERT_MSG(data[0] == 0xca, "test_1 failure, expected value is 0xca");
      TEST_ASSERT_MSG(data[1] == 0xfe, "test_1 failure, expected value is 0xfe");
    }

    {
      vector<unsigned char> data;
      data.push_back(0xba); data.push_back(0xbe); data.push_back(0x01); data.push_back(0x02); 
      TEST_ASSERT_MSG(sm_client1.write(data) == 0, "test_1 failure, invalid return code");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      data.assign(2, 0x00);
      TEST_ASSERT_MSG(sm_client1.read(data) == 0, "test_1 failure, invalid return code");
      TEST_ASSERT(data.size() == 2);
      TEST_ASSERT_MSG(data[0] == 0xba, "test_1 failure, expected value is 0xba");
      TEST_ASSERT_MSG(data[1] == 0xbe, "test_1 failure, expected value is 0xbe");
      TEST_ASSERT_MSG(data[2] == 0x01, "test_1 failure, expected value is 0x01");
      TEST_ASSERT_MSG(data[3] == 0x02, "test_1 failure, expected value is 0x02");
    }
  }
  
};

/**
 * @class message_queue_test_suite
 * @brief test suite implementation for Message Queue
 */
class message_queue_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  message_queue_test_suite() {
    TEST_ADD(message_queue_test_suite::test_1);
  }
	
private:
  /**
   * @brief Test case for @see ipc::message_queue
   */
  void test_1() {
    message_queue mq_server(0xcafedeca, static_cast<uint64_t>(1));
    TEST_ASSERT(mq_server.get_fd() != -1);
    
    message_queue mq_client(0xcafedeca, static_cast<uint64_t>(1), ipc_rights::ro);
    TEST_ASSERT(mq_client.get_fd() != -1);
    
    vector<unsigned char> data;
    data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); data.push_back(0xbe); data.push_back(0xef);
    TEST_ASSERT_MSG(mq_client.write(data) == 0, "test_1 failure, invalid return code");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    data.assign(6, 0x00);
    TEST_ASSERT_MSG(mq_client.read(data) == 0, "test_1 failure, invalid return code");
    TEST_ASSERT(data.size() == 6);
    TEST_ASSERT_MSG(data[0] == 0xca, "test_1 failure, expected value is 0xca");
    TEST_ASSERT_MSG(data[1] == 0xfe, "test_1 failure, expected value is 0xfe");
    TEST_ASSERT_MSG(data[2] == 0xde, "test_1 failure, expected value is 0xde");
    TEST_ASSERT_MSG(data[3] == 0xca, "test_1 failure, expected value is 0xca");
    TEST_ASSERT_MSG(data[4] == 0xbe, "test_1 failure, expected value is 0xbe");
    TEST_ASSERT_MSG(data[5] == 0xef, "test_1 failure, expected value is 0xef");
  }
  
};

/**
 * @class ipc_manager_test_suite
 * @brief test suite implementation for ipc_manager
 */
class ipc_manager_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  ipc_manager_test_suite() {
    TEST_ADD(ipc_manager_test_suite::test_1);
    TEST_ADD(ipc_manager_test_suite::test_2);
    TEST_ADD(ipc_manager_test_suite::test_3);
    TEST_ADD(ipc_manager_test_suite::test_4);
  }
	
private:
  /**
   * @brief Test case for @see ipc::ipc_manager
   * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
   */
  void test_1() {
    // Create share memories
    int32_t ipc_server1 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xcafedeca, 1024);
    TEST_ASSERT(ipc_server1 != -1);
    int32_t ipc_server2 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xbabebabe, 256);
    TEST_ASSERT(ipc_server2 != -1);
    TEST_ASSERT(ipc_server1 != ipc_server2);

    // Get ipcs
    TEST_THROWS_NOTHING(ipc_manager::get_instance().get_ipc(ipc_server1));
    TEST_THROWS_NOTHING(ipc_manager::get_instance().get_ipc(ipc_server2));
    
    // Remove ipcs
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_server1) != -1);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_server2) != -1);
  }
  
  /**
   * @brief Test case for @see ipc::ipc_manager
   * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
   */
  void test_2() {
    // Create share memories
    int32_t ipc_server = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xcafedeca, 1024);
    TEST_ASSERT(ipc_server != -1);
    int32_t ipc_client1 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 128);
    TEST_ASSERT(ipc_client1 != -1);
    int32_t ipc_client2 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 256);
    TEST_ASSERT(ipc_client1 != -1);

    // Get ipcs
    TEST_THROWS_NOTHING(ipc_manager::get_instance().get_ipc(ipc_server));
    TEST_THROWS_NOTHING(ipc_manager::get_instance().get_ipc(ipc_client1));
    TEST_THROWS_NOTHING(ipc_manager::get_instance().get_ipc(ipc_client2));
    
    // Remove ipcs
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_client1) == 0);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_client1) == -1);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_client2) == 0);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_client2) == -1);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_server) == 0);
  }
  
  /**
   * @brief Test case for @see ipc::ipc_manager/share_memory
   * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
   */
  void test_3() {
    // Create share memories
    int32_t ipc_server = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, 0xcafedeca, 1024);
    TEST_ASSERT(ipc_server != -1);
    int32_t ipc_client1 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 128);
    TEST_ASSERT(ipc_client1 != -1);
    int32_t ipc_client2 = ipc_manager::get_instance().create_ipc(ipc_type::shared_memory, ipc_rights::ro, 0xcafedeca, 1024, 256);
    TEST_ASSERT(ipc_client2 != -1);
    
    vector<unsigned char> data;
    data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
    TEST_ASSERT_MSG(ipc_manager::get_instance().get_ipc(ipc_client2).write(data) == 0, "test_3 failure, invalid return code");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    data.assign(2, 0x00);
    TEST_ASSERT_MSG(ipc_manager::get_instance().get_ipc(ipc_client2).read(data) == 0, "test_3 failure, invalid return code");
    TEST_ASSERT(data.size() == 2);
    TEST_ASSERT_MSG(data[0] == 0xca, "test_3 failure, expected value is 0xca");
    TEST_ASSERT_MSG(data[1] == 0xfe, "test_3 failure, expected value is 0xfe");
    
    // Remove ipcs
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_client2) == 0);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_client1) == 0);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_server) == 0 );
  }
  
  /**
   * @brief Test case for @see ipc::ipc_manager/message_queue
   * Check ipc::ipc_manager::create_ipc, ::ipc::ipc_managerget_ipc and ipc::ipc_manager::remove_ipc
   */
  void test_4() {
    // Create share memories
    int32_t ipc_server = ipc_manager::get_instance().create_ipc(ipc_type::message_queue, 0xcafedeca, 1);
    TEST_ASSERT(ipc_server != -1);
    int32_t ipc_client = ipc_manager::get_instance().create_ipc(ipc_type::message_queue, ipc_rights::ro, 0xcafedeca, 1);
    TEST_ASSERT(ipc_client != -1);

    vector<unsigned char> data;
    data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
    TEST_ASSERT_MSG(ipc_manager::get_instance().get_ipc(ipc_client).write(data) == 0, "test_4 failure, invalid return code");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    data.assign(4, 0x00);
    TEST_ASSERT_MSG(ipc_manager::get_instance().get_ipc(ipc_client).read(data) == 0, "test_4 failure, invalid return code");
    TEST_ASSERT(data.size() == 4);
    TEST_ASSERT_MSG(data[0] == 0xca, "test_4 failure, expected value is 0xca");
    TEST_ASSERT_MSG(data[1] == 0xfe, "test_4 failure, expected value is 0xfe");
    TEST_ASSERT_MSG(data[2] == 0xde, "test_4 failure, expected value is 0xde");
    TEST_ASSERT_MSG(data[3] == 0xca, "test_4 failure, expected value is 0xca");
    
    // Remove ipcs
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_client) == 0);
    TEST_ASSERT(ipc_manager::get_instance().remove_ipc(ipc_server) == 0 );
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
    ts.add(unique_ptr<Test::Suite>(new shared_memory_test_suite));
    ts.add(unique_ptr<Test::Suite>(new message_queue_test_suite));
    ts.add(unique_ptr<Test::Suite>(new ipc_manager_test_suite));

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
