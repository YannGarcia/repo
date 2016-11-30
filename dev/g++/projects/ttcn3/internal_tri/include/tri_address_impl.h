/**
 * @file    tri_address_impl.h
 * @brief   Header file for the tri port id interface.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */
#pragma once

#include"tri_address.h"

#include "ibstream.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_address_impl : public tri_address {
	ibstream _ibs;
      public:
        tri_address_impl() : _ibs() { };
        tri_address_impl(const tri_address & p_tri_address) : _ibs() { set_encoded_data(p_tri_address.get_encoded_data(), p_tri_address.get_bits_data_len()); };
        tri_address_impl(const uint8_t * p_data, const uint32_t p_length) : _ibs() { set_encoded_data(p_data, p_length); };
        virtual ~tri_address_impl() { _ibs.close(); };
	
	inline const uint8_t * get_encoded_data() const { return _ibs.rdbuf(); };
	inline void set_encoded_data(const uint8_t * p_data, const uint32_t p_length) { _ibs.open(std::vector<uint8_t>(p_data, p_data + static_cast<size_t>(p_length / 8 + (p_length % 8 != 0) ? 1 : 0)), p_length); };
	inline uint32_t get_bits_data_len() const { return _ibs.gcount(); };
	inline bool operator== (const tri_address & p_tri_address) const { return _ibs.is_equal(p_tri_address.get_encoded_data(), p_tri_address.get_bits_data_len()); };
	inline std::unique_ptr<tri_address> clone_address () const { std::unique_ptr<tri_address> a(new tri_address_impl(*this)); return a; };
	inline bool operator< (const tri_address & p_tri_address) const { return !_ibs.is_equal(p_tri_address.get_encoded_data(), p_tri_address.get_bits_data_len()); };	
      }; // End of class tri_address_impl

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
