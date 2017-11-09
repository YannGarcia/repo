/**
 * \file    tci_value.h
 * \brief   Main header file for the tci_value class.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include "tci_module_id.h"
#include "tci_value.h"

namespace internal::ttcn3::tci {
    
    class tci_value {
    public:
        virtual ~tci_value ();

        virtual const tci_ype & getType () const = 0;
        virtual const std::string & get_value_encoding () const = 0;
        virtual const std::string & get_value_encoding_variant () const = 0;
        virtual bool notPresent () const = 0;
        virtual bool operator== (const tci_value & p_tci_value) const = 0;
        virtual tci_value * clone () const = 0;
        virtual bool operator< (const tci_value & p_tci_value) const = 0;
        virtual bool is_null() const { return false ; }
    }; // End of class tci_value
    
}; // End of namespace internal::ttcn3::tci

using namespace internal::ttcn3::tci;
