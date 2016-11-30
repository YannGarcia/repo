/**
 * @File    nmea2000.h
 * @brief   Header file for the lightweight nmea2000 library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <regex.h> /* Online regex evaluation: http://www.regexplanet.com/advanced/perl/index.html
                      NOTE: Replace \\ by \
		   */

#include "gps_parser.h"

namespace gps {

  namespace parsers {

    /**
     * @class nmea2000
     * @brief This class implements gps_parser interface for NME 2000 GPS protocol
     */
    class nmea2000 : public gps_parser {
    protected:
      /** Buffer containing the NMEA frames to be processed
       */
      std::string _buffer;
      /** ::regex result code required by ::regerror
       */
      int32_t _result_code;
      /** Required by ::regerror to store error message
       */
      char _error_msg[256];

      /** NMEA commands
       */
      static std::string cmd_pattern;
      /** Global Positioning System Fix Data
       */
      static std::string gga_pattern;
      /** Recommended minimum specific GPS/Transit data
       */
      static std::string rmc_pattern;
      
      regex_t _regex_command;
      regmatch_t *_matches;
      regex_t _regex_gga;
      regex_t _regex_rmc;

    public:
      nmea2000();
      virtual ~nmea2000() { };

      virtual const int32_t process(const std::vector<uint8_t> & p_gps_frame);
      virtual const int32_t process(const std::string & p_gps_frame);

    protected:
      virtual const int32_t process_buffer();
      virtual const int32_t process_gga(const std::string & p_gps_frame);
      virtual const int32_t process_rmc(const std::string & p_gps_frame);
      
    }; // End of class nmea2000

  } // End of namespace parsers

} // End of namespace gps

using namespace gps::parsers;
