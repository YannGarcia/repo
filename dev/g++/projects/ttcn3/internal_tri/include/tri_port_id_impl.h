/**
 * \file    tri_port_id_impl.h
 * \brief   Header file for the tri port id interface.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 * \see ETSI ES 201 873-5 
 */
#pragma once

#include"tri_port_id.h"
#include"tri_qualified_name.h"
#include"tri_component_id_impl.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_port_id_impl : public tri_port_id {
	std::string _port_name;
	tri_component_id_impl _tri_component_id;
	uint64_t _port_index;
	tri_qualified_name _port_type;
	
      public:
        tri_port_id_impl() : _port_name(), _tri_component_id(), _port_index((uint64_t)-1), _port_type() { };
      tri_port_id_impl(const tri_port_id & p_tri_port_id) : _port_name(p_tri_port_id.get_port_name()), _tri_component_id((tri_component_id_impl)p_tri_port_id.get_component()), _port_index(p_tri_port_id.get_port_index()), _port_type(p_tri_port_id.get_port_type()) { };
      tri_port_id_impl(const std::string & p_port_name, const std::string & p_component_name, const uint32_t p_port_index) : _port_name(p_port_name), _tri_component_id(), _port_index(p_port_index), _port_type() { };
	virtual ~tri_port_id_impl() { _port_name.clear(); };
    
	inline const std::string & get_port_name() const { return _port_name; };
	inline void set_port_name(const std::string & p_port_name) { _port_name.assign(p_port_name); };
	inline const tri_component_id & get_component() const { return static_cast<const tri_component_id &>(_tri_component_id); };
	inline void set_component(const tri_component_id & p_component_id) { _tri_component_id.set_component_type_name(p_component_id.get_component_type_name()); _tri_component_id.set_component_name(p_component_id.get_component_name()); _tri_component_id.set_component_id(p_component_id.get_component_id()); };
	inline uint32_t get_port_index() const { return _port_index; };
	inline void set_port_index(const uint32_t p_port_index) { _port_index = p_port_index; };
	inline const tri_qualified_name & get_port_type() const { return _port_type; };
	inline void set_port_type(const tri_qualified_name & p_name) { _port_type.set_module_name(p_name.get_module_name()); _port_type.set_object_name(p_name.get_object_name()); };
	inline bool is_array() const { return _port_index != (uint64_t)-1; };
	inline std::unique_ptr<tri_port_id> clone_port_id() const { std::unique_ptr<tri_port_id> t(new tri_port_id_impl(*this)); return t; };
	inline bool operator== (const tri_port_id & p_port_id) const { return _port_index == p_port_id.get_port_index(); };
	inline bool operator< (const tri_port_id & p_port_id) const { return _port_index < p_port_id.get_port_index(); };
	
      }; // End of class tri_port_id_impl

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
