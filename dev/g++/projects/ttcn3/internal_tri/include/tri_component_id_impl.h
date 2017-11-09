/**
 * \file    tri_component_id_impl.h
 * \brief   Header file for the tri component id interface.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 * \see ETSI ES 201 873-5 
 */
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <memory>

#include "tri_component_id.h"
#include "tri_qualified_name.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_component_id_impl : public tri_component_id {
	tri_qualified_name _component_type_name;
	std::string _component_name;
	uint32_t _component_id;
	
      public:
	tri_component_id_impl() : _component_type_name(), _component_name(), _component_id((uint32_t)-1) { };
        tri_component_id_impl(const tri_component_id & p_tri_component_id) : _component_type_name(p_tri_component_id.get_component_type_name()), _component_name(p_tri_component_id.get_component_name()), _component_id(p_tri_component_id.get_component_id()) { };
      tri_component_id_impl(std::string & p_module_name, std::string & p_component_type_name, std::string & p_component_name, const uint32_t p_component_id) : _component_type_name(p_module_name, p_component_type_name), _component_name(p_component_name), _component_id(_component_id) { };
	
	virtual ~tri_component_id_impl() { _component_name.clear(); };
    
	inline const tri_qualified_name & get_component_type_name() const {return _component_type_name; };
	inline void set_component_type_name(const tri_qualified_name & p_component_type_name) { _component_type_name.set_module_name(p_component_type_name.get_module_name()); _component_type_name.set_object_name(p_component_type_name.get_object_name()); };
	inline const std::string & get_component_name() const { return _component_name; };
	inline void set_component_name(const std::string & p_component_name) { _component_name.assign(p_component_name); };
	inline const uint32_t get_component_id() const { return _component_id; };
	inline void set_component_id(const uint32_t p_component_id) { _component_id = p_component_id; };
	inline std::unique_ptr<tri_component_id> clone_component_id() const { std::unique_ptr<tri_component_id> c(new tri_component_id_impl(*this)); return c; };
	inline bool operator== (const tri_component_id &p_component_id) const { return _component_id == p_component_id.get_component_id(); };
	inline bool operator< (const tri_component_id &p_component_id) const { return _component_id < p_component_id.get_component_id(); };

	inline std::string to_string() {
	  std::ostringstream os;
	  os <<"{" << _component_name << "} (" << _component_id << ")";
	  return os.str();
	}
	
      }; // End of class tri_component_id_impl

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
