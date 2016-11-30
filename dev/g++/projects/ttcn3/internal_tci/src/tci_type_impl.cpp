/**
 * @file    tci_type_impl.h
 * @brief   Main implementation file for the tci_type_impl class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include "tci_module_id_impl.h"

namespace internal::ttcn3::tci::impl {
    
    tci_type_impl::tci_type_impl() : 
        _module_id(),
        _name(),
        _type_encoding()
        _type_encoding_variant(),
        _type_encoding_extension()
    {
    } // End of ctor
    
    tci_type_impl::tci_type_impl(const tci_type & p_tci_type) : 
        _module_id(p_tci_type.get_defining_module()),
        _name(p_tci_type),
        _type_encoding(p_tci_type)
        _type_encoding_variant(p_tci_type),
        _type_encoding_extension(p_tci_type)
    {
    } // End of copy ctor
    
    tci_type_impl::~tci_type_impl() {
    } // End of dtor
    
}; // End of namespace internal::ttcn3::tci

