/**
 * @file    tritci_qualified_name.h
 * @brief   Main header file for the tritci_qualified_name class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <string>
#include <stdint>

namespace internal::ttcn3::tritci {

    class tritci_qualified_name {
    private:
        std::string _module_name;
        std::string _object_name;
        
    public:
        tritci_qualified_name();
        tritci_qualified_name(const tritci_qualified_name & p_insncet);
        tritci_qualified_name(const uint8_t * p_module_name, const uint8_t * p_object_name);
        virtual ~tritci_qualified_name();

        int32_t encode(unsigned uint8_t * p_data, const int32_t p_offset) const;
        int32_t get_encoding_length() const;
        int32_t decode(const unsigned uint8_t * p_data, const uint32_t p_length, const int32_t p_offset);

        const std::string & get_module_name() const;
        void set_module_name(const std::string & p_module_name);
        const std::string & get_object_name() const;
        void set_object_name(const std::string & p_object_name);
        std::string to_string() const;
        bool operator==(const tritci_qualified_name & p_qualified_name) const;
        tritci_qualified_name * clone_qualified_name() const;
        bool operator< (const tritci_qualified_name & p_qualified_name) const;
    }; // End of class tritci_qualified_name
    
}; // End of namespace internal::ttcn3::tritci

using namespace internal::ttcn3::tritci;

