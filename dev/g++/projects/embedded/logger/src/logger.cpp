/**
 * @File    logger.cpp
 * @brief   Implementation header file for the lightweight logger library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include "logger.h"

namespace logger {

  
  const std::string & logger::get_timestamp() {
    static const char mon_name[][4] = {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    static char _buffer[26];
    ::gettimeofday(&_timeval, NULL);
    _time = _timeval.tv_sec;
    _tm = ::localtime(&_time);
    ::sprintf(
	      _buffer,
	      "%4d/%.3s/%02d %02d:%02d:%02d.%06ld",
	      _tm->tm_year + 1900,
	      mon_name[_tm->tm_mon],
	      _tm->tm_mday,
	      _tm->tm_hour,
	      _tm->tm_min,
	      _tm->tm_sec,
	      _timeval.tv_usec
	      );
    _timestamp.assign(_buffer);
    
    return _timestamp;
  }
  
  void logger::log(const logger_levels_t p_level, const std::string & p_fmt, ::va_list p_args) {
    char * buffer = new char[1024];
    ::vsnprintf(buffer, 1023, p_fmt.c_str(), p_args);
    log(p_level, std::string(buffer));
    delete [] buffer;
  }

  void logger::trace(const char * p_fmt, ...) {
    if ((_levels & logger_levels_t::trace) != 0x00) {
      va_list args;
      ::va_start(args, p_fmt);
      log(logger_levels_t::error, std::string(p_fmt), args);
      ::va_end(args);
    }
  }
  
  void logger::debug(const char * p_fmt, ...) {
    if ((_levels & logger_levels_t::debug) != 0x00) {
      va_list args;
      ::va_start(args, p_fmt);
      log(logger_levels_t::error, std::string(p_fmt), args);
      ::va_end(args);
    }
  }
  
  void logger::info(const char * p_fmt, ...) {
    if ((_levels & logger_levels_t::info) != 0x00) {
      va_list args;
      ::va_start(args, p_fmt);
      log(logger_levels_t::error, std::string(p_fmt), args);
      ::va_end(args);
    }
  }
  
  void logger::warning(const char * p_fmt, ...) {
    if ((_levels & logger_levels_t::warning) != 0x00) {
      va_list args;
      ::va_start(args, p_fmt);
      log(logger_levels_t::error, std::string(p_fmt), args);
      ::va_end(args);
    }
  }
  
  void logger::error(const char * p_fmt, ...) {
    if ((_levels & logger_levels_t::error) != 0x00) {
      va_list args;
      ::va_start(args, p_fmt);
      log(logger_levels_t::error, std::string(p_fmt), args);
      ::va_end(args);
    }
  }
  
} // End of namespace logger
