/**
 * @file    tri_address_list_impl.h
 * @brief   Header file for the tri port id list implementation.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */
#pragma once

#include <vector>
#include <memory>

#include "tri_address_list.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_address_list_impl : public tri_address_list {
	std::vector<std::shared_ptr<tri_address> > _ports;
      public:
        tri_address_list_impl() : _ports() { };
        tri_address_list_impl(const std::vector<std::shared_ptr<tri_address> > & p_ports) : _ports(p_ports) { };
        tri_address_list_impl(const tri_address_list & p_tri_address_list) : _ports() { copy_(p_tri_address_list); };
	virtual ~tri_address_list_impl() { clear_(); };

	inline uint32_t size() const { return _ports.size(); };
	inline bool is_empty() const { return _ports.empty(); };
	inline const tri_address & get(const uint32_t p_index) const { return *_ports[p_index]; };
	inline void clear() { clear_(); };
	inline void add(const tri_address & p_address) { _ports.push_back(p_address.clone_address()); };
	inline std::unique_ptr<tri_address_list> clone_address_list () const { std::unique_ptr<tri_address_list> p(new tri_address_list_impl(*this)); return p; };
	bool operator== (const tri_address_list & p_address_list) const;
	bool operator< (const tri_address_list & p_address_list) const;

      private:
	void clear_();
	void copy_(const tri_address_list & p_tri_address_list);
      }; // End of class tri_address_list_impl

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;

