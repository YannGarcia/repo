/**
 * @File    logger_factory.h
 * @brief   Header file for the lightweight logger library factory.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <map>
#include <memory>
#include "logger.h"

namespace logger {

  /**
   * @class logger_factory
   * @brief This class provide a set of methods for lightweight logging factory
   * @remark Singleton pattern
   */
  class logger_factory {
    static std::unique_ptr<logger_factory> g_instance;
    
    std::map<std::string, std::shared_ptr<logger> > _loggers;

    /**
     * @brief Private constructor
     */
    logger_factory() : _loggers() { };
    
  public:
    static inline logger_factory & get_instance() { return *g_instance; };
    
    ~logger_factory() { _loggers.clear(); };
    
    inline logger & get_logger(const std::string & p_logger_name);

    inline void add_logger(const std::string & p_logger_name, const std::string & p_file_name, const uint8_t p_logger_level_filter = logger_levels_t::info | logger_levels_t::warning | logger_levels_t::error);
    
    inline void remove_logger(const std::string & p_logger_name);
    
  }; // End of class logger_factory

  logger & logger_factory::get_logger(const std::string & p_logger_name) {
    std::map<std::string, std::shared_ptr<logger> >::iterator it = _loggers.find(p_logger_name);
    if (it == _loggers.end()) {
      throw std::invalid_argument("logger_factory::get_logger");
    }

    return *_loggers.at(it->first);
  };
  
  void logger_factory::add_logger(const std::string & p_logger_name, const std::string & p_file_name, const uint8_t p_logger_level_filter) {
    std::map<std::string, std::shared_ptr<logger> >::iterator it = _loggers.find(p_logger_name);
    if (it == _loggers.end()) {
      _loggers.insert(std::pair<std::string, std::shared_ptr<logger> >(p_logger_name, std::shared_ptr<logger>(new logger(p_logger_name, p_file_name, p_logger_level_filter))));
    }
  };
  
  void logger_factory::remove_logger(const std::string & p_logger_name) {
    std::map<std::string, std::shared_ptr<logger> >::iterator it = _loggers.find(p_logger_name);
    if (it != _loggers.end()) {
      it->second.reset();
      _loggers.erase(it);
    }
  };
  
  std::unique_ptr<logger_factory> logger_factory::g_instance(new logger_factory());
  
} // End of namespace logger

using namespace logger;
