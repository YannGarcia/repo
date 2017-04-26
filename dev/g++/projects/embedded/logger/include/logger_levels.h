/**
 * @File      logger_levels.h
 * @brief     Header file for the lightweight logger library.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

namespace logger {

  /**
   * @enum logger_levels_t
   * @brief List of authorized log levels
   */
  enum logger_levels_t : uint8_t { 
    trace   = 0x01,    /** Verbose level */
    debug   = 0x02,    /** Debug level */
    info    = 0x04,    /** Information level */
    warning = 0x08,    /** warning levels */
    error   = 0x10,    /** error level */
    all     = 0xff     /** All levels */
  }; // End of enum logger_levels_t
  
} // End of namespace logger
