/**
 * \file    tri_log.h
 * \brief   Header file for the tri_log class.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <memory>
#include <cstdarg>      // std::va_list

#include "logger_factory.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      class tri_log {
	std::string _logger_name;
	static std::unique_ptr<tri_log> g_instance;
	
	tri_log() : _logger_name("internal_tri") {
	  logger_factory::get_instance().add_logger(_logger_name, "/tmp/internal_tri.log", logger_levels_t::all);
	};

      public:
	virtual ~tri_log() { logger_factory::get_instance().remove_logger(_logger_name); _logger_name.clear(); };
	
	static inline tri_log & get_instance() { return *g_instance; };
	
	inline void debug(const std::string & p_string) { if (logger_factory::get_instance().get_logger(_logger_name).is_debug_set()) logger_factory::get_instance().get_logger(_logger_name).debug(p_string); };
	inline void debug(const char * p_fmt, ...);
	
      }; // End of class tri_factory

      std::unique_ptr<tri_log> tri_log::g_instance(new tri_log());
      
      void tri_log::debug(const char * p_fmt, ...) {
	if (logger_factory::get_instance().get_logger(_logger_name).is_debug_set()) {
	  va_list args;
	  ::va_start(args, p_fmt);
	  char * buffer = new char[1024];
	  ::vsnprintf(buffer, 1023, p_fmt, args);
	  logger_factory::get_instance().get_logger(_logger_name).debug(std::string(buffer));
	  delete [] buffer;
	  ::va_end(args);
	}
      }
  
    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

using namespace internal::ttcn3::tri;
