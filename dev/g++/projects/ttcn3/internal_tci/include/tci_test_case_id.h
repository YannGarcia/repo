/**
 * \file    tci_test_case_id.h
 * \brief   Main header file for the tci_test_case_id class.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include "tritci_qualified_name.h"

namespace internal::ttcn3::tci {

    class tci_test_case_id : tritci_qualified_name {
    public:
        virtual ~tci_test_case_id();

        virtual bool operator== (const tci_test_case_id &p_tcid) const =0;
        virtual tci_test_case_id * clone() const =0;
        virtual bool operator< (const tci_test_case_id &p_tcid) const =0;
    }; // End of class tci_test_case_id
    
}; // End of namespace internal::ttcn3::tci

using namespace internal::ttcn3::tci;

