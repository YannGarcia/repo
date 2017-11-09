/**
 * \file    tci_type_impl.h
 * \brief   Main header file for the tci_type_impl class.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include "tci_type.h"

    class tci_type_impl : public tci_type {
    protected:
        tci_module_id _module_id;
        std::string _name;
        std::string _type_encoding;
        std::string  _type_encoding_variant;
        std::vector<std::string *> _type_encoding_extension;
    public:
        tci_type_impl()
        tci_type_impl(const tci_type_impl & p_tci_type_impl);
        virtual ~tci_type_impl();
        
        virtual const tci_module_id & get_defining_module () const { return _module_id; };
        virtual const std::string & get_name () const = 0;
        virtual const tci_type_t & get_type_class () const = 0;
        virtual const std::string & get_type_encoding () const = 0;
        virtual const std::string & get_type_encoding_variant() const = 0;
        virtual const std::vector<std::string *> & get_type_extension() const = 0;
        virtual tci_value * new_instance()const = 0;
        virtual bool operator==(const tci_type & p_tci_type) const = 0;
        virtual tci_type * clone() const { return new tci_type_impl(*this); };
        virtual bool operator<(const tci_type & p_tci_type) const = 0;
    }; // End of class tci_type_impl
    
}; // End of namespace internal::ttcn3::tci::impl

using namespace internal::ttcn3::tci::impl;
