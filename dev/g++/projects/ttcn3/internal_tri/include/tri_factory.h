/**
 * @file    tri_factory.h
 * @brief   Header file for the tri_factory class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include "tri_communication_sa_impl.h"
#include "tri_test_case_id_impl.h"
#include "tri_port_id_impl.h"
#include "tri_port_id_list_impl.h"
#include "tri_address_impl.h"
#include "tri_address_list_impl.h"
#include "tri_message_impl.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_factory {
      public:
	static std::unique_ptr<tri_factory> g_instance;
      
      private:
      tri_factory() : _sa(static_cast<tri_communication_sa *>(new tri_communication_sa_impl())) { };
      
	std::unique_ptr<tri_communication_sa> _sa;
      
      public:
        virtual ~tri_factory() { _sa.reset(); };

	static inline tri_factory & get_instance() { return *g_instance; };
	
	static inline tri_communication_sa & get_instance_sa() { return *g_instance->_sa; };
	/* tri_factory & get_instance_pa() { return *g_instance; }; */

	inline tri_test_case_id_impl tri_create_testcase_id(const std::string & p_module_name, const std::string & p_test_case_name) {
	  tri_test_case_id_impl t(p_module_name, p_test_case_name);
	  return t;
	};

	inline tri_port_id_list_impl tri_create_port_id_list(const std::vector<std::shared_ptr<tri_port_id> > & p_ports) {
	  tri_port_id_list_impl p(p_ports);
	  return p;
	};
	
	inline std::shared_ptr<tri_port_id> tri_component_id(const std::string & p_port_name, const std::string & p_component_name, const uint32_t p_port_index) {
	  std::shared_ptr<tri_component> p(new tri_component_impl(p_port_name, p_component_name, p_port_index));
	  return p;
	};

	inline std::shared_ptr<tri_port_id> tri_create_port_id(const std::string & p_port_name, const std::string & p_component_name, const uint32_t p_port_index) {
	  std::shared_ptr<tri_port_id> p(new tri_port_id_impl(p_port_name, p_component_name, p_port_index));
	  return p;
	};

	inline std::shared_ptr<tri_address> tri_create_address(const uint8_t * p_data, const uint32_t p_length) {
	  std::shared_ptr<tri_address> p(new tri_address_impl(p_data, p_length));
	  return p;
	};

	inline std::shared_ptr<tri_message> tri_create_message(const uint8_t * p_data, const uint32_t p_length) {
	  std::shared_ptr<tri_message> p(new tri_message_impl(p_data, p_length));
	  return p;
	};

      }; // End of class tri_factory
    
    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
