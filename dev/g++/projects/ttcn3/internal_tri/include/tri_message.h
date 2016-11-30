/**
 * @file    tri_message.h
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

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_message {
      public:
	virtual ~tri_message() { };
    
	virtual const uint8_t * get_data() const = 0;
	virtual void set_data(const uint8_t *p_data, const uint32_t p_length) = 0;
	virtual uint32_t get_bits_data_len() const = 0;
	virtual bool operator== (const tri_message & p_tri_message) const = 0;
	virtual std::unique_ptr<tri_message> clone_message() const = 0;
	virtual bool operator< (const tri_message & p_tri_message) const = 0;
      }; // End of class tri_message

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
