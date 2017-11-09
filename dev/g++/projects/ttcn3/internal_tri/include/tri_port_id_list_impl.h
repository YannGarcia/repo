/**
 * \file    tri_port_id_list_impl.h
 * \brief   Header file for the tri port id list implementation.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 * \see ETSI ES 201 873-5 
 */
#pragma once

#include <vector>
#include <memory>

#include "tri_port_id_list.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_port_id_list_impl : public tri_port_id_list {
	std::vector<std::shared_ptr<tri_port_id> > _ports;
      public:
        tri_port_id_list_impl() : _ports() { };
        tri_port_id_list_impl(const std::vector<std::shared_ptr<tri_port_id> > & p_ports) : _ports(p_ports) { };
        tri_port_id_list_impl(const tri_port_id_list & p_tri_port_id_list) : _ports() { copy_(p_tri_port_id_list); };
	virtual ~tri_port_id_list_impl() { clear_(); };

	inline uint32_t size() const { return _ports.size(); };
	inline bool is_empty() const { return _ports.empty(); };
	inline const tri_port_id & get(const uint32_t p_index) const { return *_ports[p_index]; };
	inline void clear() { clear_(); };
	inline void add(const tri_port_id & p_port_id) { _ports.push_back(p_port_id.clone_port_id()); };
	inline std::unique_ptr<tri_port_id_list> clone_port_id_list () const { std::unique_ptr<tri_port_id_list> p(new tri_port_id_list_impl(*this)); return p; };
	bool operator== (const tri_port_id_list & p_port_id_list) const;
	bool operator< (const tri_port_id_list & p_port_id_list) const;

      private:
	void clear_();
	void copy_(const tri_port_id_list & p_tri_port_id_list);
      }; // End of class tri_port_id_list_impl

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;

