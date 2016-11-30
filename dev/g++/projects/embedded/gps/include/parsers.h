/**
 * @File    parsers.h
 * @brief   Header file for the lightweight gps library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

namespace gps {

  namespace parsers {

    /**
     * @enum parsers_t
     * @brief List of authorized GPS protocol parsers
     */
    enum parsers_t : uint8_t { 
      nmea_2000 = 0x01    /** NMEA 2000 GPS protocol parser */
    }; // End of enum parsers_t
  
  } // End of namespace parsers

} // End of namespace gps

using namespace gps::parsers;
