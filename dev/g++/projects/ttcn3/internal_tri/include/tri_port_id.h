/**
 * @file    tri_port_id.h
 * @brief   Header file for the tri port id interface.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */
#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "tri_component_id.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_port_id {
      public:
	virtual ~tri_port_id() { };
    
	virtual const std::string & get_port_name() const = 0;
	virtual void set_port_name(const std::string & p_port_name) = 0;
	virtual const tri_component_id & get_component() const = 0;
	virtual void set_component(const tri_component_id & p_component_id) = 0;
	virtual uint32_t get_port_index() const = 0;
	virtual void set_port_index(const uint32_t p_port_index) = 0;
	virtual const tri_qualified_name & get_port_type() const = 0;
	virtual void set_port_type(const tri_qualified_name & p_port_type) = 0;
	virtual bool is_array() const = 0;
	virtual std::unique_ptr<tri_port_id> clone_port_id() const = 0;
	virtual bool operator== (const tri_port_id & p_port_id) const = 0;
	virtual bool operator< (const tri_port_id & p_port_id) const = 0;
	
      }; // End of class tri_port_id

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
