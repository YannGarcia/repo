/*!
 * \file      logger_factory.h
 * \brief     Header file for the lightweight logger library factory.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <map>
#include <memory>

#include "logger.hh"

/*! \namespace logger
 *  \brief logger namespace
 */
namespace logger {

  /*!
   * \class logger_factory
   * \brief This class provide a set of methods for lightweight logging factory
   * \remark Singleton pattern
   */
  class logger_factory {
    /*!< Global instance of the logger class. 
     *   Singleton pattern
     */
    static std::unique_ptr<logger_factory> g_instance;
    
    /*!< Map of named logger */
    std::map<std::string, std::shared_ptr<logger> > _loggers;

    /**
     * \brief Private constructor
     * Singleton pattern
     */
    logger_factory() : _loggers() { };
    
  public:
    static inline logger_factory & get_instance() { return *g_instance; };
    
    /**
     * \brief Public destructor
     */
    ~logger_factory() { _loggers.clear(); };
    
    /**
     * \brief Retrieve a named logger instance
     * \param[in] p_logger_name The logger instance name
     * \return The logger instance on success, std::invalid_argument exception is raised otherwise
     * @throw std::invalid_argument if the named was not registered
     */
    inline logger & get_logger(const std::string & p_logger_name);

    /**
     * \brief Register a new named logger instance
     * \param[in] p_logger_name         The name of this instance of the logger class 
     * \param[in] p_file_name           The full path name to store logs
     * \param[in] p_logger_level_filter The authorised log level. Default: logger_levels_t::info | logger_levels_t::warning | logger_levels_t::error
     * \param[in] p_logger_time_format  The timestamp format to use. Default: logger_time_formats_t::datetime
     * \remark If the logger named already exists, nothing is done 
     */
    inline void add_logger(const std::string & p_logger_name, const std::string & p_file_name, const uint8_t p_logger_level_filter = logger_levels_t::info | logger_levels_t::warning | logger_levels_t::error, const logger_time_formats_t p_logger_time_format = logger_time_formats_t::datetime);
    
    /**
     * \brief Unregister an existing named logger instance
     * \param[in] p_logger_name The logger instance name
     * \remark If the logger named does not exist, nothing is done 
     */
    inline void remove_logger(const std::string & p_logger_name);
    
  }; // End of class logger_factory

  logger & logger_factory::get_logger(const std::string & p_logger_name) {
    std::map<std::string, std::shared_ptr<logger> >::iterator it = _loggers.find(p_logger_name);
    if (it == _loggers.end()) {
      throw std::invalid_argument("logger_factory::get_logger");
    }

    return *_loggers.at(it->first);
  };
  
  void logger_factory::add_logger(const std::string & p_logger_name, const std::string & p_file_name, const uint8_t p_logger_level_filter, const logger_time_formats_t p_logger_time_format) {
    std::map<std::string, std::shared_ptr<logger> >::iterator it = _loggers.find(p_logger_name);
    if (it == _loggers.end()) {
      _loggers.insert(std::pair<std::string, std::shared_ptr<logger> >(p_logger_name, std::shared_ptr<logger>(new logger(p_logger_name, p_file_name, p_logger_level_filter, p_logger_time_format))));
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
