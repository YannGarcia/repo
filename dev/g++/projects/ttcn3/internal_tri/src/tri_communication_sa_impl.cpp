/**
 * @file    tri_communication_sa_impl.cpp
 * @brief   Implementation file for the tri communication SA interface
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */

#include "tri_communication_sa_impl.h"

#include "tri_log.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      tri_communication_sa_impl::tri_communication_sa_impl() /*: _sockects()*/ {

      }

      tri_communication_sa_impl::~tri_communication_sa_impl() {
	// _sockets.clear();
      }

      const tri_status tri_communication_sa_impl::tri_sa_reset() {
	tri_log::get_instance().debug(std::string(">>> tri_sa_reset"));
					  
	// _sockets.clear();
	
	return tri_ok; // TODO
      }

      const tri_status tri_communication_sa_impl::tri_execute_test_case(const tri_test_case_id & p_tri_test_case_id, const tri_port_id_list & p_tri_port_id_list) {
	tri_log::get_instance().debug(std::string(">>> tri_execute_test_case"));
					  
	return tri_ok; // TODO
      }

      const tri_status tri_communication_sa_impl::tri_end_test_case() {
	tri_log::get_instance().debug(std::string(">>> tri_end_test_case"));
					  
	return tri_ok; // TODO
      }

      const tri_status tri_communication_sa_impl::tri_map(const tri_port_id & p_tri_component_id, const tri_port_id & p_tri_system_port_id) {
	tri_log::get_instance().debug(">>> tri_map: %s:%s -> %s:%s", p_tri_component_id.get_component().get_component_name().c_str(), p_tri_component_id.get_port_name().c_str(), p_tri_system_port_id.get_component().get_component_name().c_str(), p_tri_system_port_id.get_port_name().c_str());
					  
	return tri_ok; // TODO
      }
      
      const tri_status tri_communication_sa_impl::tri_unmap(const tri_port_id & p_tri_component_id, const tri_port_id & p_tri_system_port_id) {
	tri_log::get_instance().debug(">>> tri_unmap: %s:%s -> %s:%s", p_tri_component_id.get_component().get_component_name().c_str(), p_tri_component_id.get_port_name().c_str(), p_tri_system_port_id.get_component().get_component_name().c_str(), p_tri_system_port_id.get_port_name().c_str());
					  
	return tri_ok; // TODO
      }
      
      const tri_status tri_communication_sa_impl::tri_send(const tri_component_id & p_component_id, const tri_port_id & p_tsi_port_id, const tri_address & p_sut_address, const tri_message & p_send_message) {
	tri_log::get_instance().debug(">>> tri_send: %s:%s", p_component_id.get_component_name().c_str(), p_tsi_port_id.get_port_name().c_str());
					  
	return tri_ok; // TODO
      }
	
      const tri_status tri_communication_sa_impl::tri_send_bc(const tri_component_id & p_component_id, const tri_port_id & p_tsi_port_id, const tri_message & p_send_message) {
	tri_log::get_instance().debug(">>> tri_send_bc: %s:%s", p_component_id.get_component_name().c_str(), p_tsi_port_id.get_port_name().c_str());
					  
	return tri_ok; // TODO
      }
	
      const tri_status tri_communication_sa_impl::tri_send_mc(const tri_component_id & p_component_id, const tri_port_id & p_tsi_port_id, const tri_address_list & p_sut_addresses, const tri_message & p_send_message) {
	tri_log::get_instance().debug(">>> tri_send_mc: %s:%s", p_component_id.get_component_name().c_str(), p_tsi_port_id.get_port_name().c_str());
					  
	return tri_ok; // TODO
      }

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal
