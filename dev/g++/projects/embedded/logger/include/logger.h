/**
 * @File    logger.h
 * @brief   Header file for the lightweight logger library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>      // std::filebuf
#include <cstdarg>      // std::va_list
#include <stdexcept>
#include <string>
#include <cstdint>
#include <ctime>
#include <sys/time.h>

#include "logger_levels.h"
#include "logger_time_formats.h"

namespace logger {

  /**
   * @class logger
   * @brief This class provide a set of methods for lightweight logging
   */
  class logger {
    std::string _name;
    std::ofstream _os;
    uint8_t _levels;
    uint8_t _time_format;
    std::string _timestamp;
    struct timeval _start_time_val;
    struct tm *_start_time;
    time_t _time;
    struct tm * _tm;
    struct timeval _time_val;
    
  public:
    /**
     * @brief Constructor
     *        Create a new instance of the logger class
     * @param[in] p_logger_name         The name of this instance of the logger class 
     * @param[in] p_file_name           The full path name to store logs
     * @param[in] p_logger_level_filter The authorised log level. Default: logger_levels_t::error
     * @param[in] p_logger_time_format  The timestamp format to use. Default: logger_time_formats_t::datetime
     */
  logger(const std::string & p_logger_name, const std::string & p_file_name, const uint8_t p_logger_level_filter = logger_levels_t::error, const logger_time_formats_t p_logger_time_format = logger_time_formats_t::datetime) : _name(p_logger_name), _os(p_file_name, std::ofstream::out | std::ofstream::trunc), _levels(p_logger_level_filter), _time_format(p_logger_time_format), _timestamp(), _start_time(NULL), _tm(NULL) { if (!_os.is_open()) throw std::runtime_error("logger::logger: Failed to open log file"); };
    /**
     * @brief Destructor
     */
    virtual ~logger() { _os.close(); };

    inline bool is_trace_set() { return ((_levels & logger_levels_t::trace) != 0x00); };
    inline bool is_debug_set() { return ((_levels & logger_levels_t::debug) != 0x00); };
    inline bool is_info_set() { return ((_levels & logger_levels_t::info) != 0x00); };
    inline bool is_warning_set() { return ((_levels & logger_levels_t::warning) != 0x00); };
    inline bool is_error_set() { return ((_levels & logger_levels_t::error) != 0x00); };
    
    inline void trace(const std::string & p_string) { if ((_levels & logger_levels_t::trace) != 0x00) log(logger_levels_t::trace, p_string); };
    inline void debug(const std::string & p_string) { if ((_levels & logger_levels_t::debug) != 0x00) log(logger_levels_t::debug, p_string); };
    inline void info(const std::string & p_string) { if ((_levels & logger_levels_t::info) != 0x00) log(logger_levels_t::info, p_string); };
    inline void warning(const std::string & p_string) { if ((_levels & logger_levels_t::warning) != 0x00) log(logger_levels_t::warning, p_string); };
    inline void error(const std::string & p_string) { if ((_levels & logger_levels_t::error) != 0x00) log(logger_levels_t::error, p_string); };
    
    void trace(const char * p_fmt, ...);
    void debug(const char * p_fmt, ...);
    void info(const char * p_fmt, ...);
    void warning(const char * p_fmt, ...);
    void error(const char * p_fmt, ...);

    void set_start_time();
    
  private:
    inline void log(const logger_levels_t p_level, const std::string & p_string) { _os << get_timestamp() << " logger " << p_string << std::endl; };
    void log(const logger_levels_t p_level, const std::string & p_fmt, ::va_list p_args);
    const std::string & get_timestamp();
    
  }; // End of class logger

} // End of namespace logger
