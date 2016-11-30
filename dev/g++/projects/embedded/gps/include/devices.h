/**
 * @File    devices.h
 * @brief   Header file for the lightweight gps library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

namespace gps {

  /**
   * @enum devices_t
   * @brief List of authorized log levels
   */
  enum devices_t : uint8_t { 
    ublox_device   = 0x01    /** UBLOX GPS devices */
  }; // End of enum devices_t
  
} // End of namespace gps

using namespace gps;
