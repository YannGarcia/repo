/**
 * @file    tri_test_case_id.h
 * @brief   Header file for the tri test case id interface.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */
#pragma once

#include "tri_qualified_name.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_test_case_id : public tri_qualified_name {
      public:
	tri_test_case_id() { };
        tri_test_case_id(const std::string & p_module_name, const std::string & p_object_name): tri_qualified_name(p_module_name, p_object_name) { };
	virtual ~tri_test_case_id() { };
	
	virtual std::unique_ptr<tri_test_case_id> clone_test_case_id() const = 0;
	virtual bool operator== (const tri_test_case_id & p_test_case_id) const = 0;
	virtual bool operator< (const tri_test_case_id & p_test_case_id) const = 0;
  
      }; // End of class tri_test_case_id

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
