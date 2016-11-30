/**
 * @file    tri_test_case_id_impl.h
 * @brief   Header file for the tri test case id implementation
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */
#pragma once

#include "tri_test_case_id.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_test_case_id_impl : public tri_test_case_id {
      public:
	tri_test_case_id_impl() { };
      tri_test_case_id_impl(const std::string & p_module_name, const std::string & p_object_name) : tri_test_case_id(p_module_name, p_object_name) { };
        tri_test_case_id_impl(const tri_test_case_id & p_tri_test_case_id) : tri_test_case_id(p_tri_test_case_id) { };
	virtual ~tri_test_case_id_impl() { };
	
	inline std::unique_ptr<tri_test_case_id> clone_test_case_id() const { std::unique_ptr<tri_test_case_id> t(new tri_test_case_id_impl(*this)); return t; };
	inline bool operator== (const tri_test_case_id & p_test_case_id) const { return static_cast<const tri_qualified_name &>(*this) == static_cast<const tri_qualified_name &>(p_test_case_id); };
	inline bool operator< (const tri_test_case_id & p_test_case_id) const { return static_cast<const tri_qualified_name &>(*this) < static_cast<const tri_qualified_name &>(p_test_case_id); };
  
      }; // End of class tri_test_case_id_impl

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
