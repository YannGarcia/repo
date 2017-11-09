/**
 * \file    tci_test_case_id_impl.h
 * \brief   Main header file for the tci_test_case_id_impl class.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <string>

namespace internal::ttcn3::tci::impl {
    
    /** \class tci_module_id_impl
     * \brief Implementation of abstract class tci_test_case_id
     * \see tci_test_case_id
     */
    class tci_module_id_impl : public tci_module_id {
    protected:
        std::string _module_name;
        uint32_t _type;
        
    public:
        tci_module_id_impl() : _module_name(), _type(0) { };
        tci_module_id_impl(const tci_module_id & p_tci_module_id) : _module_name(p_tci_module_id.get_object_name()), _type(0) { };
        virtual ~tci_module_id_impl() { };
        
        inline virtual const std::string & get_object_name() const { return _module_name; };
        inline virtual void set_object_name (const std::string & p_module_name) { _module_name = p_module_name; };
        inline virtual bool operator== (const tci_module_id & p_module_name) const { return _module_name.compare(p_module_name) == 0; };
        inline virtual tci_module_id * clone () const { return new tci_module_id_impl(*this); };
        inline virtual bool operator< (const tci_module_id & p_module_name) const { return _module_name.compare(p_module_name) < 0; };
        
    public:
        inline virtual int compute_length() const { return sizeof(uint32_t) + _module_name.length(); };
        virtual uint32_t encode(const uint8_t * p_data, const uint32_t p_offset) const;
        virtual uint32_t decode(const uint8_t * p_data, const uint32_t p_length, const uint32_t p_offset);
    }; // End of class tci_module_id_impl
    
}; // End of namespace internal::ttcn3::tci::impl

using namespace internal::ttcn3::tci::impl;

