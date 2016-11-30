/**
 * @file    tri_port_id_list_impl.cpp
 * @brief   Source file for the tri port id list implementation.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */

#include "tri_port_id_list_impl.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

/*      tri_port_id_list_impl::tri_port_id_list_impl(const std::vector<std::unique_ptr<tri_port_id> > & p_ports) : _ports() {
	for (uint32_t i = 0; i < p_ports.size(); i++) {
	  _ports.push_back(p_ports[i]->clone_port_id());			   
	} // End of 'for' statement
      }*/
      
      bool tri_port_id_list_impl::operator== (const tri_port_id_list & p_port_id_list) const {
	if (_ports.size() == p_port_id_list.size()) {
	  return false; // TODO
	}

	return false;
      }
      
      bool tri_port_id_list_impl::operator< (const tri_port_id_list & p_port_id_list) const {
	if (_ports.size() < p_port_id_list.size()) {
	  return true;
	} else if (_ports.size() == p_port_id_list.size()) {
	  return false; // TODO
	}

	return false;
      }

      void tri_port_id_list_impl::clear_() {
	for (std::vector<std::shared_ptr<tri_port_id> >::iterator it = _ports.begin(); it != _ports.end(); ++it) {
	  it->reset();
	} // End of 'for' statement
	_ports.clear();
      }
      
      void tri_port_id_list_impl::copy_(const tri_port_id_list & p_tri_port_id_list) {
	for (uint32_t i = 0; i < p_tri_port_id_list.size(); i++) {
	  _ports.push_back(p_tri_port_id_list.get(i).clone_port_id());			   
	} // End of 'for' statement
      }
      
    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

