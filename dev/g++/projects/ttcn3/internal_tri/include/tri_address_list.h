/**
 * \file    tri_address_list.h
 * \brief   Header file for the tri port id list interface.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 * \see ETSI ES 201 873-5 
 */
#pragma once

#include <cstdint>

#include "tri_address.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_address_list {
      public:
	virtual ~tri_address_list() { };

	virtual uint32_t size() const = 0;
	virtual bool is_empty() const = 0;
	virtual const tri_address & get(const uint32_t p_index) const = 0;
	virtual void clear() = 0;
	virtual void add(const tri_address & p_address) = 0;
	virtual std::unique_ptr<tri_address_list> clone_address_list () const = 0;
	virtual bool operator== (const tri_address_list & p_address_list) const = 0;
	virtual bool operator< (const tri_address_list & p_address_list) const = 0;
      }; // End of class tri_address_list

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;

