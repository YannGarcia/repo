/*!
 * \file      logger_time_formats.h
 * \brief     Header file for the lightweight logger library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

/*! \namespace logger
 *  \brief logger namespace
 */
namespace logger {

  /*!
   * \enum logger_time_formats_t
   * \brief List of authorized log levels
   */
  enum logger_time_formats_t : uint8_t { 
    time     = 0x01,    /*!< Time format */
    datetime = 0x02,    /*!< Date and Time format */
    seconds  = 0x04     /*!< Seconds format */
  }; // End of enum logger_time_formats_t
  
} // End of namespace logger
