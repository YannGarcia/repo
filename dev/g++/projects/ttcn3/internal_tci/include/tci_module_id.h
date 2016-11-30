/**
 * @file    tci_test_case_id.h
 * @brief   Main header file for the tci_test_case_id class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <string>

namespace internal::ttcn3::tci {
    
    class tci_module_id {
    public:
        virtual ~tci_module_id() { };

        virtual const std::string & get_object_name() const = 0;
        virtual void set_object_name (const std::string &p_module_name) = 0;
        virtual bool operator== (const tci_module_id & p_module_name) const = 0;
        virtual tci_module_id * clone () const = 0;
        virtual bool operator< (const tci_module_id & p_module_name) const = 0;
    }; // End of class tci_module_id
    
}; // End of namespace internal::ttcn3::tci

using namespace internal::ttcn3::tci;

