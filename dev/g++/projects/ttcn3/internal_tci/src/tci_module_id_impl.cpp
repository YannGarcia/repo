/**
 * @file    tci_test_case_id_impl.h
 * @brief   Main implementation file for the tci_test_case_id_impl class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */

#include "tci_module_id_impl.h"

namespace internal::ttcn3::tci::impl {
    
    uint32_t tci_module_id_impl::encode(const uint8_t * p_data, const uint32_t  p_offset) const {
        size_t length = _module_name.length();
        memcpy(&p_data[p_offset], &length, sizeof(size_t));
        p_offset += sizeof(size_t);
        memcpy((void *)&p_data[p_offset], (const void *)_module_name.c_str(), length);
        return p_offset + length;
    } // End of method encode
    
    uint32_t tci_module_id_impl::decode(const uint8_t * p_data, const uint32_t p_length, const int32_t p_offset) {
        size_t length;
        uint32_t offset = p_offset;
        memcpy((void *)&length, (const void *)&p_data[offset], sizeof(size_t));
        offset += sizeof(size_t);
        _module_name.clear();
        if ((offset + length) > p_length)
            return -1;
        } else if (length > 0) {
            _module_name.assign((char *)&p_data[offset], length);
            offset += length;
        }
        
        return offset;
    } // End of method decode
    
}; // End of namespace internal::ttcn3::tci

