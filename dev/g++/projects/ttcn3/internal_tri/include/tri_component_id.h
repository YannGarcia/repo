/**
 * \file    tri_component_id.h
 * \brief   Header file for the tri component id interface.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 * \see ETSI ES 201 873-5 
 */
#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "tri_qualified_name.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_component_id {
      public:
	virtual ~tri_component_id() { };
    
	virtual const tri_qualified_name & get_component_type_name() const = 0;
	virtual void set_component_type_name(const tri_qualified_name & p_component_type_name) = 0;
	virtual const std::string & get_component_name() const = 0;
	virtual void set_component_name(const std::string & p_component_name) = 0;
	virtual const uint32_t get_component_id() const = 0;
	virtual void set_component_id(const uint32_t p_component_id) = 0;
	virtual std::unique_ptr<tri_component_id> clone_component_id() const = 0;
	virtual bool operator== (const tri_component_id &p_component_id) const = 0;
	virtual bool operator< (const tri_component_id &p_component_id) const = 0;
	
      }; // End of class tri_component_id

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
