/**
 * @File      logger.cpp
 * @brief     Implementation header file for the lightweight logger library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include "logger.h"

namespace logger {

  /* TODO To be continued
  template<typename T> 
  logger & operator <<(logger &p_logger, const logger_levels_t p_logger_level, T const & p_value) { 
    if (is_set(p_logger_level)) { 
      _os << p_value; 
    } 
    return p_logger; 
  }*/
  
  void logger::set_start_time() {
    ::gettimeofday(&_start_time_val, NULL);
    _time = _start_time_val.tv_sec;
    _start_time = ::localtime(&_time);
  }
  
  const std::string & logger::get_timestamp() {
    static const char mon_name[][4] = {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    static char _buffer[32];
    ::gettimeofday(&_time_val, NULL);
    _time = _time_val.tv_sec;
    _tm = ::localtime(&_time);
    if (_time_format == logger_time_formats_t::seconds) {
      struct timeval diff = {0};
      if (_time_val.tv_usec < _start_time_val.tv_usec) {
	diff.tv_sec = _tm->tm_sec - _start_time->tm_sec - 1;
	diff.tv_usec = _time_val.tv_usec + (1000000L - _start_time_val.tv_usec);
      } else {
	diff.tv_sec = _tm->tm_sec - _start_time->tm_sec;
	diff.tv_usec = _time_val.tv_usec - _start_time_val.tv_usec;
      }
      ::sprintf(
		_buffer,
		"%ld.%06ld",
		diff.tv_sec,
		diff.tv_usec
		);
    } else if (_time_format == logger_time_formats_t::time) {
      ::sprintf(
		_buffer,
		"%02d:%02d:%02d.%06ld",
		_tm->tm_hour,
		_tm->tm_min,
		_tm->tm_sec,
		_time_val.tv_usec
		);
    } else {
      ::sprintf(
		_buffer,
		"%4d/%.3s/%02d %02d:%02d:%02d.%06ld",
		_tm->tm_year + 1900,
		mon_name[_tm->tm_mon],
		_tm->tm_mday,
		_tm->tm_hour,
		_tm->tm_min,
		_tm->tm_sec,
		_time_val.tv_usec
		);
    }
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
