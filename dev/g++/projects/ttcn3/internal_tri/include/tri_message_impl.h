/**
 * \file    tri_message_impl.h
 * \brief   Header file for the tri port id interface.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 * \see ETSI ES 201 873-5 
 */
#pragma once

#include"tri_message.h"

#include "ibstream.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_message_impl : public tri_message {
	ibstream _ibs;
      public:
        tri_message_impl() : _ibs() { };
        tri_message_impl(const tri_message & p_tri_message) : _ibs() { set_data(p_tri_message.get_data(), p_tri_message.get_bits_data_len()); };
        tri_message_impl(const uint8_t * p_data, const uint32_t p_length) : _ibs() { set_data(p_data, p_length); };
        virtual ~tri_message_impl() { _ibs.close(); };
	
	inline const uint8_t * get_data() const { return _ibs.rdbuf(); };
	inline void set_data(const uint8_t * p_data, const uint32_t p_length) { _ibs.open(std::vector<uint8_t>(p_data, p_data + static_cast<size_t>(p_length / 8 + (p_length % 8 != 0) ? 1 : 0)), p_length); };
	inline uint32_t get_bits_data_len() const { return _ibs.gcount(); };
	inline bool operator== (const tri_message & p_tri_message) const { return _ibs.is_equal(p_tri_message.get_data(), p_tri_message.get_bits_data_len()); };
	inline std::unique_ptr<tri_message> clone_message() const { std::unique_ptr<tri_message> a(new tri_message_impl(*this)); return a; };
	inline bool operator< (const tri_message & p_tri_message) const { return !_ibs.is_equal(p_tri_message.get_data(), p_tri_message.get_bits_data_len()); };
	
      }; // End of class tri_message_impl

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
