/*!
 * \file    ipc_manager.h
 * \brief   Header file for IPC factory.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <stdexcept> // std::out_of_range
#include <memory> // Used for unique_ptr

#include <poll.h>

#include "abstract_ipc.hh"
#include "ipc_type.hh"
#include "ipc_rights.hh"

namespace ipc {
  
  /**
   * \class ipc_manager
   * \brief 
   */
  class ipc_manager {
    
    static uint32_t _counter;                               /*!< Created channel counter */
    std::map<const uint32_t, abstract_ipc *> _ipcs;         /*!< abstract_ipc instances */
    std::map<const uint32_t, struct pollfd> _polls;         /*!< Polling map on created channels */
    bool _polling_in_progress;                              /*!< Polling progress flag */
    static std::unique_ptr<ipc_manager> _instance;          /*!< Unique instance of this class (singleton) */

  public:
    inline static ipc_manager & get_instance() { return *_instance; };

  private:
    ipc_manager();

  public:
    ~ipc_manager();

    const int32_t create_ipc(const ipc_type p_ipc_type, const ipc_rights p_ipc_rights, const int32_t p_key, const uint64_t p_value, const uint32_t p_offset = 0);
    const int32_t create_ipc(const ipc_type p_ipc_type, const int32_t p_key, const uint64_t p_value, const uint32_t p_offset = 0);

    const int32_t remove_ipc(const uint32_t p_ipc);
    const int32_t poll_ipcs(const uint32_t p_timeout, std::vector<uint32_t> & p_ipcs);
    const int32_t poll_ipcs(const uint32_t p_timeout, std::vector<uint32_t> & p_ipcsToPoll, std::vector<uint32_t> & p_ipcs);

    inline abstract_ipc & get_ipc(const uint32_t p_ipc) const { if (_ipcs.find(p_ipc) == _ipcs.end()) throw std::out_of_range("Wrong channel identifier" ); return *_ipcs.at(p_ipc); };
    
  private:
    const uint32_t initialise_ipc(abstract_ipc * p_ipc);
    
  }; // End of class ipc_manager

} // End of namespace ipc

using namespace ipc;
