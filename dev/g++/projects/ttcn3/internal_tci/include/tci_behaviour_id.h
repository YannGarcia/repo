/**
 * @file    tci_behaviour_id.h
 * @brief   Main header file for the tci_behaviour_id class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

namespace internal::ttcn3::tci {

    class tci_behaviour_id : pulic tri_qualified_name {
    public:
        tci_behaviour() { };
        virtual ~tci_behaviour() { };
        
        virtual bool operator == (const TciBehaviourId & p_bid) const = 0;

        virtual tci_behaviour_id * clone () const = 0;

        virtual bool operator< (const tci_behaviour_id & p_id) const = 0;
        
    }; // End of call tci_behaviour_id
    
}; // End of namespace internal::ttcn3::tci

using namespace internal::ttcn3::tci;
