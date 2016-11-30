/**
 * @file    ipc_type.h
 * @brief   Header file for IPC types enumerated.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <cstdint>

namespace ipc {
  
  /**
   * @enum ipc_t
   * @brief List of authorized IPCs
   */
  enum class ipc_type : uint8_t { // TODO Rename ipc_type into ipc_t
    shared_memory = 0x00,
    message_queue = 0x01
  }; // End of enum class ipc_t

} // End of namespace ipc

using namespace ipc;
