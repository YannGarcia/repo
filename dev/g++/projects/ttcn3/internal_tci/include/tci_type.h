/**
 * @file    tci_type.h
 * @brief   Main header file for the tci_type class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include "tci_module_id.h"
#include "tci_value.h"

namespace internal::ttcn3::tci {
    
    typedef enum { 
        tci_address = 0,
        tci_anytype = 1,
        tci_bitstring = 2,
        tci_boolean = 3,
        tci_char = 4,
        tci_charstring = 5,
        tci_component = 6,
        tci_enumerated = 7,
        tci_float = 8,
        tci_hexstring = 9,
        tci_integer = 10,
        tci_objid = 11,
        tci_octetstring = 12,
        tci_record = 13,
        tci_record_of = 14, 
        tci_array = 15,
        tci_set = 16,
        tci_set_of = 17,
        tci_union = 18,
        tci_universal_char = 19,
        tci_universal_charstring = 20,
        tci_verdict = 21
    } tci_type_t; 
    
    class tci_type {
    public:
        virtual ~tci_type ();
        
        virtual const tci_module_id & get_defining_module () const = 0;
        virtual const std::string & get_name () const = 0;
        virtual const tci_type_t & get_type_class () const = 0;
        virtual const std::string & get_type_encoding () const = 0;
        virtual const std::string & get_type_encoding_variant() const = 0;
        virtual const std::vector<std::string *> & get_type_extension() const = 0;
        virtual tci_value * new_instance()const = 0;
        virtual bool operator==(const tci_type & p_tci_type) const = 0;
        virtual tci_type * clone() const = 0;
        virtual bool operator<(const tci_type & p_tci_type) const = 0;
    }; // End of class tci_type
    
}; // End of namespace internal::ttcn3::tci

using namespace internal::ttcn3::tci;
