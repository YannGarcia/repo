/**
 * \file    ipc_rights.h
 * \brief   Header file for IPC rights access enumerated.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <cstdint>

namespace ipc {
  
  /**
   * \enum ipc_rights_t
   * \brief List of authorized IPCs
   */
  enum class ipc_rights : uint8_t { // TODO Rename ipc_rights into ipc_tirghts_t
    ro = 0x00,
    rw = 0x01
  }; // End of enum class ipc_rights_t

} // End of namespace ipc

using namespace ipc;
