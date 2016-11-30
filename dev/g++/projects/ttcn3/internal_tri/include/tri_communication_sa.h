/**
 * @file    tri_communication_sa.h
 * @brief   Header file for the tri communication SA interface.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 * @see ETSI ES 201 873-5 
 */
#pragma once

#include <memory>

#include "tri_status.h"
#include "tri_test_case_id.h"
#include "tri_port_id_list.h"
#include "tri_address.h"
#include "tri_address_list.h"
#include "tri_message.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_communication_sa {
      public:
	tri_communication_sa() { };
	virtual ~tri_communication_sa() { };
    
	/** 
	 * @brief The SA shall reset all communication means that it is maintaining
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.1 triSAReset
	 */
	virtual const tri_status tri_sa_reset() = 0;
    
	/** 
	 * @brief The SA sets up any static connections to the SUT and initialize any communication means for TSI ports
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.2.1 triExecuteTestCase (TE -> SA)
	 */
	virtual const tri_status tri_execute_test_case(const tri_test_case_id & p_tri_test_case_id, const tri_port_id_list & p_tri_port_id_list) = 0;
	
	/** 
	 * @brief The SA free resources, cease communication at system ports and to test components
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.2.6 triEndTestCase (TE -> SA)
	 */
	virtual const tri_status tri_end_test_case() = 0;
    
	/** 
	 * @brief The SA establish a dynamic connection to the SUT for the referenced TSI port
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.2.2 triMap (TE -> SA)
	 */
	virtual const tri_status tri_map(const tri_port_id & p_tri_component_id, const tri_port_id & p_tri_system_port_id) = 0;
    
	/** 
	 * @brief The SA shall close a dynamic connection to the SUT for the referenced TSI port
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.2.4 triUnmap (TE -> SA)
	 */
	virtual const tri_status tri_unmap(const tri_port_id & p_tri_component_id, const tri_port_id & p_tri_system_port_id) = 0;

	/**
	 * @brief The SA shall executes a TTCN-3 unicast send operation on a component port
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.3.1 triSend (TE -> SA)
	 */
	virtual const tri_status tri_send(const tri_component_id & p_component_id, const tri_port_id & p_tsi_port_id, const tri_address & p_sut_address, const tri_message & p_send_message) = 0;
	
	/**
	 * @brief The SA shall executes a TTCN-3 broadcast send operation on a component port
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.3.2 triSendBC (TE -> SA)
	 */
	virtual const tri_status tri_send_bc(const tri_component_id & p_component_id, const tri_port_id & p_tsi_port_id, const tri_message & p_send_message) = 0;
	
	/**
	 * @brief The SA shall executes a TTCN-3 multicast send operation on a component port
	 * @return tri_ok on success, tr_error otherwise
	 * @see ETSI ES 201 873-5 Clause 5.5.3.3 triSendMC (TE -> SA)
	 */
	virtual const tri_status tri_send_mc(const tri_component_id & p_component_id, const tri_port_id & p_tsi_port_id, const tri_address_list & p_sut_addresses, const tri_message & p_send_message) = 0;

      }; // End of class tri_communication_sa

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
