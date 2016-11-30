/**
 * @File    testlib.cpp
 * @brief   Types converter test suite.
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

#include "tri_factory.h"
#include "tri_test_case_id_impl.h"
#include "tri_port_id_impl.h"
#include "tri_port_id_list.h"
#include "tri_component_id_impl.h"

using namespace std;

/**
 * @class ttcn3_tri_test_suite
 * @brief internal::ttcn3::tri_* test suite implementation
 */
class ttcn3_tri_test_suite : public Test::Suite {
public:
  /**
   * @brief Default ctor
   */
  ttcn3_tri_test_suite() {
    TEST_ADD(ttcn3_tri_test_suite::test_tri_sa_instance);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_sa_reset);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_execute_test_case_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_qualified_name_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_qualified_name_2);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_qualified_name_3);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_qualified_name_4);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_test_case_id_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_port_id_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_component_id_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_port_id_list_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_port_id_list_2);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_port_id_list_3);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_port_id_list_4);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_address_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_address_2);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_message_1);
    TEST_ADD(ttcn3_tri_test_suite::test_tri_message_2);
  }
	
private:
  /**
   * @brief Test case for @see tri_factory::get_instance_sa
   * Check tri_communication_sa::tri_sa_reset()
   */
  void test_tri_sa_instance() {
    TEST_ASSERT(tri_factory::get_instance_sa().tri_sa_reset() == tri_ok);
  }
  
  /**
   * @brief Test case for @see tri_factory::tri_sa_reset()
   */
  void test_tri_sa_reset() {
    TEST_ASSERT(tri_factory::get_instance_sa().tri_sa_reset() == tri_ok);
  }
  
  /**
   * @brief Test case for @see tri_factory::tri_sa_reset()
   */
  void test_tri_execute_test_case_1() {
    tri_port_id_impl p1(std::string("p1"), std::string("1"), 1), p2(std::string("p2"), std::string("2"), 2), p3(std::string("p3"), std::string("3"), 3);
    std::vector<shared_ptr<tri_port_id> > v = { 
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p1"), std::string("1"), 1)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p2"), std::string("2"), 2)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p3"), std::string("3"), 3))
    };
    tri_port_id_list_impl l(v);
    
    TEST_ASSERT(
		internal::ttcn3::tri::tri_factory::get_instance_sa().tri_execute_test_case(
											   internal::ttcn3::tri::tri_factory::get_instance().tri_create_testcase_id("AtsGtpv2C_TestCases", "TC_S11_MME_PM_01"),
											   internal::ttcn3::tri::tri_factory::get_instance().tri_create_port_id_list(v)
											   )
		== tri_ok
		);
  }

  void test_tri_send_1() {
    std::vector<uint8_t> address = { 0xca, 0xfe, 0xde, 0xca };
    std::vector<uint8_t> message = { 0xca, 0xfe, 0xde, 0xc0 };
    TEST_ASSERT(
		internal::ttcn3::tri::tri_factory::get_instance_sa().tri_send(
									      *internal::ttcn3::tri::tri_factory::tri_create_port_id(std::string("p1"), std::string("1"), 1),
									      *internal::ttcn3::tri::tri_factory::tri_create_component_id(std::string("module"), std::string("type_name"), std::string("name"), 10),						      *internal::ttcn3::tri::tri_factory::tri_create_address(address.data(), address.size() * 8),
									      *internal::ttcn3::tri::tri_factory::tri_create_message(message.data(), 28)	      
									      )
		== tri_ok
		);      
  }
  
  /**
   * @brief Test case for @see tri_qualified_name
   */
  void test_tri_qualified_name_1() {
    tri_qualified_name q;
    TEST_ASSERT(q.get_module_name().empty());
    TEST_ASSERT(q.get_object_name().empty());

    q.set_module_name(std::string("Module"));
    q.set_object_name(std::string("Component"));
    TEST_ASSERT(q.get_module_name().compare("Module") == 0);
    TEST_ASSERT(q.get_object_name().compare("Component") == 0);
    
  }
  
  /**
   * @brief Test case for @see tri_qualified_name
   */
  void test_tri_qualified_name_2() {
    tri_qualified_name q("Module", "Component");
    TEST_ASSERT(q.get_module_name().compare("Module") == 0);
    TEST_ASSERT(q.get_object_name().compare("Component") == 0);
  }
  
  /**
   * @brief Test case for @see tri_qualified_name
   */
  void test_tri_qualified_name_3() {
    tri_qualified_name q(std::string("Module"), std::string("Component"));
    std::unique_ptr<tri_qualified_name> c = q.clone_tri_qualified_name();
    TEST_ASSERT(c->get_module_name().compare("Module") == 0);
    TEST_ASSERT(c->get_object_name().compare("Component") == 0);
    TEST_ASSERT(q == *c);
    TEST_ASSERT(!(q < *c));    
    TEST_ASSERT(!(*c < q));    

    c.reset();
  }
  
  /**
   * @brief Test case for @see tri_qualified_name
   */
  void test_tri_qualified_name_4() {
    tri_qualified_name q1(std::string("Module"), std::string("Component"));
    tri_qualified_name q2(std::string("Modul"), std::string("Component"));
    TEST_ASSERT(!(q1 == q2));
    TEST_ASSERT(q2 < q1);    
    TEST_ASSERT(!(q1 < q2));    
    tri_qualified_name q3(std::string("Module"), std::string("Componen"));
    TEST_ASSERT(!(q1 == q3));
    TEST_ASSERT(!(q3 < q1));    
    TEST_ASSERT(!(q1 < q3));    
    tri_qualified_name q4(std::string("Modul"), std::string("Componen"));
    TEST_ASSERT(!(q1 == q4));
    TEST_ASSERT(q4 < q1);    
    TEST_ASSERT(!(q1 < q4));    
    tri_qualified_name q5(std::string("Modules"), std::string("Components"));
    TEST_ASSERT(!(q1 == q5));
    TEST_ASSERT(q1 < q5);    
    TEST_ASSERT(!(q5 < q1));    
  }
  
  /**
   * @brief Test case for @see _tri_test_case_id
   */
  void test_tri_test_case_id_1() {
    tri_test_case_id_impl t(std::string("Module"), std::string("Component"));
    TEST_ASSERT(t.get_module_name().compare("Module") == 0);
    TEST_ASSERT(t.get_object_name().compare("Component") == 0);

    std::unique_ptr<tri_test_case_id> c = t.clone_test_case_id();
    TEST_ASSERT(c->to_string().compare("Module.Component") == 0);

    TEST_ASSERT(t == *c);
    TEST_ASSERT(!(t < *c));
    TEST_ASSERT(!(*c < t));

    c.reset();
  }
  
  /**
   * @brief Test case for @see _tri_port_id
   */
  void test_tri_port_id_1() {
    tri_port_id_impl p;
    TEST_ASSERT(p.get_port_name().empty());
    TEST_ASSERT(p.get_port_index() == (uint32_t)-1);
    TEST_ASSERT(!p.is_array());
    
    p.set_port_index(1);
    TEST_ASSERT(p.get_port_index() == 1);
    TEST_ASSERT(p.is_array());
    
    p.set_port_name(std::string("MyPort"));
    TEST_ASSERT(p.get_port_name().compare("MyPort") == 0);
    
    tri_qualified_name q(std::string("Module"), std::string("Component"));
    p.set_port_type(q);
    TEST_ASSERT(p.get_port_type().get_module_name().compare("Module") == 0);
    TEST_ASSERT(p.get_port_type().get_object_name().compare("Component") == 0);
    
    std::unique_ptr<tri_port_id> c = p.clone_port_id();
    TEST_ASSERT(p == *c);
    TEST_ASSERT(!(p < *c));
    TEST_ASSERT(!(*c < p));
    TEST_ASSERT(c->get_port_type() == q);

    c.reset();
  }
  
  /**
   * @brief Test case for @see _tri_component_id
   */
  void test_tri_component_id_1() {
    tri_component_id_impl c;
    TEST_ASSERT(c.get_component_name().empty());
    TEST_ASSERT(c.get_component_id() == (uint32_t)-1);
    
    c.set_component_id(1);
    TEST_ASSERT(c.get_component_id() == 1);
    
    c.set_component_name(std::string("MyComponent"));
    TEST_ASSERT(c.get_component_name().compare("MyComponent") == 0);
    
    tri_qualified_name q(std::string("Module"), std::string("Component"));
    c.set_component_type_name(q);
    TEST_ASSERT(c.get_component_type_name().get_module_name().compare("Module") == 0);
    TEST_ASSERT(c.get_component_type_name().get_object_name().compare("Component") == 0);
    
    std::unique_ptr<tri_component_id> cl = c.clone_component_id();
    TEST_ASSERT(static_cast<tri_component_id_impl &>(*cl).to_string().compare("{MyComponent} (1)") == 0);
    TEST_ASSERT(c == *cl);
    TEST_ASSERT(!(c < *cl));
    TEST_ASSERT(!(*cl < c));
    TEST_ASSERT(cl->get_component_type_name() == q);

    cl.reset();
  }
  
  /**
   * @brief Test case for @see _tri_port_id_list
   */
  void test_tri_port_id_list_1() {
    tri_port_id_impl p1(std::string("p1"), std::string("1"), 1), p2(std::string("p2"), std::string("2"), 2), p3(std::string("p3"), std::string("3"), 3);
    tri_port_id_list_impl l;
    TEST_ASSERT(l.size() == 0);
    l.add(p1);
    l.add(p2);
    l.add(p3);
    TEST_ASSERT(l.size() == 3);
    
    tri_port_id_list_impl l1(l);
    TEST_ASSERT(l1.size() == 3);
  }
  
  /**
   * @brief Test case for @see _tri_port_id_list
   */
  void test_tri_port_id_list_2() {
    tri_port_id_impl p1(std::string("p1"), std::string("1"), 1), p2(std::string("p2"), std::string("2"), 2), p3(std::string("p3"), std::string("3"), 3);
    std::vector<shared_ptr<tri_port_id> > v = { 
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p1"), std::string("1"), 1)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p2"), std::string("2"), 2)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p3"), std::string("3"), 3))
    };
    tri_port_id_list_impl l(v);
    TEST_ASSERT(l.size() == 3);
    TEST_ASSERT(l.get(0) == p1);
    TEST_ASSERT(l.get(1) == p2);
    TEST_ASSERT(l.get(2) == p3);
  }
  
  /**
   * @brief Test case for @see _tri_port_id_list
   */
  void test_tri_port_id_list_3() {
    tri_port_id_impl p1(std::string("p1"), std::string("1"), 1), p2(std::string("p2"), std::string("2"), 2), p3(std::string("p3"), std::string("3"), 3);
    std::vector<shared_ptr<tri_port_id> > v = { 
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p1"), std::string("1"), 1)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p2"), std::string("2"), 2)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p3"), std::string("3"), 3))
    };
    tri_port_id_list_impl l(v);
    
    TEST_ASSERT(l.get(0) == p1);
    TEST_ASSERT(l.get(1) == p2);
    TEST_ASSERT(l.get(2) == p3);
  }
  
  /**
   * @brief Test case for @see _tri_port_id_list
   */
  void test_tri_port_id_list_4() {
    tri_port_id_impl p1(std::string("p1"), std::string("1"), 1), p2(std::string("p2"), std::string("2"), 2), p3(std::string("p3"), std::string("3"), 3);
    std::vector<shared_ptr<tri_port_id> > v = { 
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p1"), std::string("1"), 1)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p2"), std::string("2"), 2)),
      std::shared_ptr<tri_port_id>(new tri_port_id_impl(std::string("p3"), std::string("3"), 3))
    };
    tri_port_id_list_impl l(v);
    
    std::unique_ptr<tri_port_id_list> c = l.clone_port_id_list();
    TEST_ASSERT(c->size() == 3);
    TEST_ASSERT(c->get(0) == p1);
    TEST_ASSERT(c->get(1) == p2);
    TEST_ASSERT(c->get(2) == p3);
    
    c.reset();
  }
  
  /**
   * @brief Test case for @see _tri_address
   */
  void test_tri_address_1() {
    tri_address_impl p;
    TEST_ASSERT(p.get_bits_data_len() == 0);
    TEST_ASSERT(p.get_encoded_data() == NULL);
    uint8_t data[] = { 0xca, 0xfe, 0xd0 };
    p.set_encoded_data(data, 20); // Sise of data in bits
    TEST_ASSERT(p.get_bits_data_len() == 20);
    TEST_ASSERT(p.get_encoded_data() != NULL);
    
    std::unique_ptr<tri_address> c = p.clone_address();
    TEST_ASSERT(p == *c);
    TEST_ASSERT(!(p < *c));
    TEST_ASSERT(!(*c < p));

    c.reset();
  }
  
  /**
   * @brief Test case for @see _tri_address
   */
  void test_tri_address_2() {
    uint8_t data[] = { 0xca, 0xfe, 0xde };
    tri_address_impl p(data, 24);
    TEST_ASSERT(p.get_bits_data_len() == 24);
    TEST_ASSERT(p.get_encoded_data() != NULL);    
  }
  
  /**
   * @brief Test case for @see _tri_message
   */
  void test_tri_message_1() {
    tri_message_impl p;
    TEST_ASSERT(p.get_bits_data_len() == 0);
    TEST_ASSERT(p.get_data() == NULL);
    uint8_t data[] = { 0xca, 0xfe, 0xd0 };
    p.set_data(data, 20); // Sise of data in bits
    TEST_ASSERT(p.get_bits_data_len() == 20);
    TEST_ASSERT(p.get_data() != NULL);
    
    std::unique_ptr<tri_message> c = p.clone_message();
    TEST_ASSERT(p == *c);
    TEST_ASSERT(!(p < *c));
    TEST_ASSERT(!(*c < p));

    c.reset();
  }
  
  /**
   * @brief Test case for @see _tri_message
   */
  void test_tri_message_2() {
    uint8_t data[] = { 0xca, 0xfe, 0xde };
    tri_message_impl p(data, 24);
    TEST_ASSERT(p.get_bits_data_len() == 24);
    TEST_ASSERT(p.get_data() != NULL);    
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
    ts.add(unique_ptr<Test::Suite>(new ttcn3_tri_test_suite));

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
