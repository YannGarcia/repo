/**
 * @file    tri_qualified_name.h
 * @brief   Header file for the tri qualified name (e.g. module_name.component_name).
 * @author garciay.yann@gmail.com
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

      class tri_qualified_name {
      private:
	std::string _module_name;
	std::string _object_name;
	
      public:
        tri_qualified_name() : _module_name(), _object_name() { };
        tri_qualified_name(const tri_qualified_name & p_tri_qualified_name) : _module_name(p_tri_qualified_name.get_module_name()), _object_name(p_tri_qualified_name.get_object_name()) { };
        tri_qualified_name(const std::string & p_module_name, const std::string & p_object_name) : _module_name(p_module_name), _object_name(p_object_name) { };
	virtual ~tri_qualified_name() {
	  _module_name.clear();
	  _object_name.clear();
	};

	inline const std::string & get_module_name() const { return _module_name; };
	inline void set_module_name(const std::string & p_name) { _module_name.assign(p_name); };
	inline const std::string & get_object_name() const { return _object_name; };
	inline void set_object_name(const std::string & p_name) { _object_name.assign(p_name); };
	inline std::unique_ptr<tri_qualified_name> clone_tri_qualified_name() const { std::unique_ptr<tri_qualified_name> q(new tri_qualified_name(*this)); return q; };
	inline bool operator== (const tri_qualified_name & p_tri_qualified_name) const { return (p_tri_qualified_name.get_module_name().compare(_module_name) == 0) && (p_tri_qualified_name.get_object_name().compare(_object_name) == 0); };
	bool operator< (const tri_qualified_name & p_tri_qualified_name) const;
	std::string to_string() const;
      }; // End of class tri_qualified_name

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
