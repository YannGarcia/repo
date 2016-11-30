/**
 * @File    gps_parser.h
 * @brief   Header file for the lightweight gps_parser library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <map>
#include <vector>
#include <functional> // Used for std::cref

namespace gps {

  namespace parsers {

    /**
     * @class gps_parser
     * @brief This class provide a set of methods for lightweight GPS protocol parser
     */
    class gps_parser {
    public:
      static const uint8_t utc_time_idx;
      static const uint8_t latitude_idx;
      static const uint8_t longitude_idx;
      static const uint8_t speed_idx;
      static const uint8_t altitude_idx;
      static const uint8_t heading_idx;
      static const uint8_t yawrate_idx;

    protected:
      std::map<uint8_t, std::string> _values;
      
    public:
      gps_parser();
      virtual ~gps_parser() { _values.clear(); };
      
      virtual const int32_t process(const std::vector<uint8_t> & p_gps_frame) = 0;
      virtual const int32_t process(const std::string & p_gps_frame) = 0;
      virtual inline const std::map<uint8_t, std::string> & get_result() const { return std::ref(_values); };
      
    }; // End of class gps_parser
    
  } // End of namespace parsers
  
} // End of namespace gps

using namespace gps::parsers;
