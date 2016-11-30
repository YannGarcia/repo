/**
 * @file    tri_qualified_name.cpp
 * @brief   Implementation file for the tri qualified name (e.g. module_name.component_name).
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */

#include "tri_qualified_name.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {
      
      bool tri_qualified_name::operator< (const tri_qualified_name & p_tri_qualified_name) const {
	if (_module_name < p_tri_qualified_name._module_name) {
	  return true;
	} else if (_module_name >= p_tri_qualified_name._module_name) {
	  return false;
	} else if (_object_name < p_tri_qualified_name._object_name) {
	  return true;
	}

	return  false;
      }
      
      std::string tri_qualified_name::to_string() const {
	std::string str(_module_name);
	str.append(".");
	str.append(_object_name);
	
	return str;
      }
      
    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

