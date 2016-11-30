 /**
 * @file    tci_test_case_id.h
 * @brief   Main header file for the tci_test_case_id class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include "tci_test_case_id.h"

namespace internal::ttcn3::tci {

    class tci_test_case_id_list {
    public:
        virtual ~tci_test_case_id_list();
        
        virtual size_t size () const = 0;
        virtual bool empty () const = 0;
        virtual const tci_test_case_id * get (Tsize p_index) const = 0;
        virtual void clear () = 0;
        virtual void push_back (const tci_test_case_id & p_tci_test_case_id)=0;
        virtual bool operator== (const tci_test_case_id_list & p_list) const = 0;
        virtual tci_test_case_id_list * clone () const = 0;
        virtual bool operator< (const tci_test_case_id_list & p_list) const = 0;
    }; // End of class tci_test_case_id
    
}; // End of namespace internal::ttcn3::tci

using namespace internal::ttcn3::tci;

