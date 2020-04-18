/*!
 * \file      logger.h
 * \brief     Header file for the lightweight logger library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
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
#include <vector>

#include "logger_levels.hh"
#include "logger_time_formats.hh"

/*! \namespace logger
 *  \brief logger namespace
 */
namespace logger {

  /*!
   * \class logger
   * \brief This class provide a set of methods for lightweight logging
   */
  class logger {
    /*!< Logger name */
    std::string _name;
    /*!< Output stream instance */
    std::ofstream _os;
    /*!< Authorised logger levels */
    uint8_t _levels;
    /*!< Choosen date/time format */
    uint8_t _time_format;
    /*!< Date/time value convertion */
    std::string _timestamp;
    /*!< Start date/time value used by gettimeofday() (required for very long time execution) */
    struct timeval _start_time_val;
    /*!< Start date/time value (required for very long time execution */
    struct tm *_start_time;
    /*!< Current date/time value used by gettimeofday() */
    struct timeval _time_val;
    /*!< Start date/time value (required for very long time execution */
    struct tm * _tm;
    /*!< Local time convertion */
    time_t _time;
    
  public: /*! \publicsection */
    /*!
     * \brief Constructor
     *        Create a new instance of the logger class
     * \param[in] p_logger_name         The name of this instance of the logger class 
     * \param[in] p_file_name           The full path name to store logs
     * \param[in] p_logger_level_filter The authorised log level. Default: logger_levels_t::error
     * \param[in] p_logger_time_format  The timestamp format to use. Default: logger_time_formats_t::datetime
     * @throws std::runtime_error in case of IO error
     */
    logger(const std::string & p_logger_name, const std::string & p_file_name, const uint8_t p_logger_level_filter = logger_levels_t::error, const logger_time_formats_t p_logger_time_format = logger_time_formats_t::datetime) : _name(p_logger_name), _os(p_file_name, std::ofstream::out | std::ofstream::trunc), _levels(p_logger_level_filter), _time_format(p_logger_time_format), _timestamp(), _start_time(NULL), _tm(NULL) { if (!_os.is_open()) throw std::runtime_error("logger::logger: Failed to open log file"); };

    /*!
     * \brief Destructor
     */
    virtual ~logger() { _os << std::endl; _os.close(); };

    /*!
     * \fn bool is_set(const logger_levels_t p_logger_level);
     * \brief Indicates if the specified logger level is set
     * \param[in] p_logger_level The logger level
     * \return true if the specified logger level is set, false otherwise
     * \sa logger::logger_levels_t
     * \inline
     */
    inline bool is_set(const logger_levels_t p_logger_level) { return ((_levels & p_logger_level) != 0x00); };
    /*!
     * \fn bool is_trace_set();
     * \brief Indicates if the trace level is set
     * \return true if the trace level is set, false otherwise
     * \sa logger::logger_levels_t
     * \inline
     */
    inline bool is_trace_set() { return ((_levels & logger_levels_t::trace) != 0x00); };
    /*!
     * \fn bool is_debug_set();
     * \brief Indicates if the debug level is set
     * \return true if the debug level is set, false otherwise
     * \sa logger::logger_levels_t
     * \inline
     */
    inline bool is_debug_set() { return ((_levels & logger_levels_t::debug) != 0x00); };
    /*!
     * \fn bool is_info_set();
     * \brief Indicates if the info level is set
     * \return true if the info level is set, false otherwise
     * \sa logger::logger_levels_t
     * \inline
     */
    inline bool is_info_set() { return ((_levels & logger_levels_t::info) != 0x00); };
    /*!
     * \fn bool is_warning_set();
     * \brief Indicates if the warning level is set
     * \return true if the warning level is set, false otherwise
     * \sa logger::logger_levels_t
     * \inline
     */
    inline bool is_warning_set() { return ((_levels & logger_levels_t::warning) != 0x00); };
    /*!
     * \fn bool is_error_set();
     * \brief Indicates if the error level is set
     * \return true if the error level is set, false otherwise
     * \sa logger::logger_levels_t
     * \inline
     */
    inline bool is_error_set() { return ((_levels & logger_levels_t::error) != 0x00); };
    
    /* template<typename T>  */
    /*   friend logger & operator <<(logger &p_logger, const logger_levels_t p_logger_level, T const & p_value); */

    /*!
     * \fn void trace(const std::string & p_string);
     * \brief Write the string to the IO if the trace level is set
     * \param[in] p_string The string to write
     * \inline
     */
    inline void trace(const std::string & p_string) { if ((_levels & logger_levels_t::trace) != 0x00) log(logger_levels_t::trace, p_string); };
    /*!
     * \fn void debug(const std::string & p_string);
     * \brief Write the string to the IO if the debug level is set
     * \param[in] p_string The string to write
     * \inline
     */
    inline void debug(const std::string & p_string) { if ((_levels & logger_levels_t::debug) != 0x00) log(logger_levels_t::debug, p_string); };
    /*!
     * \fn void info(const std::string & p_string);
     * \brief Write the string to the IO if the info level is set
     * \param[in] p_string The string to write
     * \inline
     */
    inline void info(const std::string & p_string) { if ((_levels & logger_levels_t::info) != 0x00) log(logger_levels_t::info, p_string); };
    /*!
     * \fn void trace(const std::string & p_string);
     * \brief Write the string to the IO if the trace level is set
     * \param[in] p_string The string to write
     * \inline
     */
    inline void warning(const std::string & p_string) { if ((_levels & logger_levels_t::warning) != 0x00) log(logger_levels_t::warning, p_string); };
    /*!
     * \fn void error(const std::string & p_string);
     * \brief Write the string to the IO if the error level is set
     * \param[in] p_string The string to write
     * \inline
     */
    inline void error(const std::string & p_string) { if ((_levels & logger_levels_t::error) != 0x00) log(logger_levels_t::error, p_string); };
    
    /*!
     * \fn void trace(const char * p_fmt, ...);
     * \brief Write a formated trace string to the IO
     * \param[in] p_fmt The message formating, such as C printf function
     */
    void trace(const char * p_fmt, ...);
    /*!
     * \fn void debug(const char * p_fmt, ...);
     * \brief Write a formated debug string to the IO
     * \param[in] p_fmt The message formating, such as C printf function
     */
    void debug(const char * p_fmt, ...);
    /*!
     * \fn void hexa_dump(const char *p_prompt, const std::vector<unsigned char>& p_buffer);
     * \brief Write a formated hexadecimal dump string to the IO
     * \param[in] p_prompt Message label
     * \remark This methos is active at \see logger_levels_t::trace level
     */
    void hexa_dump(const char *p_prompt, const std::vector<unsigned char>& p_buffer);
    /*!
     * \fn void info(const char * p_fmt, ...);
     * \brief Write a formated info string to the IO
     * \param[in] p_fmt The message formating, such as C printf function
     */
    void info(const char * p_fmt, ...);
    /*!
     * \fn void warning(const char * p_fmt, ...);
     * \brief Write a formated warning string to the IO
     * \param[in] p_fmt The message formating, such as C printf function
     */
    void warning(const char * p_fmt, ...);
    /*!
     * \fn void error(const char * p_fmt, ...);
     * \brief Write a formated error string to the IO
     * \param[in] p_fmt The message formating, such as C printf function
     */
    void error(const char * p_fmt, ...);

    /*!
     * \fn void set_start_time();
     * \brief Initialise a timer to measure duration (in ms) of a section of code
     * \bug To achieve
     */
    void set_start_time();
    
  private: /*! \privatesection */
    inline void log(const logger_levels_t p_level, const std::string & p_string) { _os << get_timestamp() << " logger " << p_string << std::endl; };
    void log(const logger_levels_t p_level, const std::string & p_fmt, ::va_list p_args);
    const std::string & get_timestamp();
    
  }; // End of class logger

} // End of namespace logger
