/**
 * @file    tri_port_id_list.h
 * @brief   Header file for the tri port id list interface.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */
#pragma once

#include <cstdint>

#include "tri_port_id.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_port_id_list {
      public:
	virtual ~tri_port_id_list() { };

	virtual uint32_t size() const = 0;
	virtual bool is_empty() const = 0;
	virtual const tri_port_id & get(const uint32_t p_index) const = 0;
	virtual void clear() = 0;
	virtual void add(const tri_port_id & p_port_id) = 0;
	virtual std::unique_ptr<tri_port_id_list> clone_port_id_list () const = 0;
	virtual bool operator== (const tri_port_id_list & p_port_id_list) const = 0;
	virtual bool operator< (const tri_port_id_list & p_port_id_list) const = 0;
      }; // End of class tri_port_id_list

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;

