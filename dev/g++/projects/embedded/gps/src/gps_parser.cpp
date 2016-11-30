/**
 * @File    gps_parser.cpp
 * @brief   Implementation file for the lightweight gps_parser library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */

#include "gps_parser.h"

namespace gps {

  namespace parsers {

    const uint8_t gps_parser::utc_time_idx = 0;
    const uint8_t gps_parser::latitude_idx = 1;
    const uint8_t gps_parser::longitude_idx = 2;
    const uint8_t gps_parser::speed_idx = 3;
    const uint8_t gps_parser::altitude_idx = 4;
    const uint8_t gps_parser::heading_idx = 5;
    const uint8_t gps_parser::yawrate_idx = 6;

    gps_parser::gps_parser() : _values() {
      // Setup default values
      _values.insert(std::pair<uint8_t, std::string>(utc_time_idx, std::string(""))); // GPS UTC of position
      _values.insert(std::pair<uint8_t, std::string>(latitude_idx, std::string(""))); // Latitude
      _values.insert(std::pair<uint8_t, std::string>(longitude_idx, std::string(""))); // Longitude
      _values.insert(std::pair<uint8_t, std::string>(speed_idx, std::string(""))); // Speed
      _values.insert(std::pair<uint8_t, std::string>(altitude_idx, std::string(""))); // Elevation
      _values.insert(std::pair<uint8_t, std::string>(heading_idx, std::string(""))); // Heading
      _values.insert(std::pair<uint8_t, std::string>(yawrate_idx, std::string(""))); // Yaw rate
      
    }

  } // End of namespace parsers

} // End of namespace gps
