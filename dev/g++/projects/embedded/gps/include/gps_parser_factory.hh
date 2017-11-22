/**
 * \file    gps_parser_factory.h
 * \brief   Header file for the lightweight gps library factory.
 * \author  garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#pragma once

#include <map>
#include <memory>

#include "parsers.hh"

#include "nmea2000.hh"

namespace gps {

  namespace parsers {

    /**
     * \class gps_parser_factory
     * \brief This class provide a set of methods for lightweight GPS protocol parsers factory
     * \remark Singleton pattern
     */
    class gps_parser_factory {
      static std::unique_ptr<gps_parser_factory> g_instance;
      
      /**
       * \brief Private constructor
       */
    gps_parser_factory() { };
    
    public:
      static inline gps_parser_factory & get_instance() { return *g_instance; };
    
      ~gps_parser_factory() { };
    
      inline std::shared_ptr<gps_parser> create(const parsers_t p_parser = parsers_t::nmea_2000);
    
    }; // End of class gps_parser_factory

    std::unique_ptr<gps_parser_factory> gps_parser_factory::g_instance(new gps_parser_factory());
  
    std::shared_ptr<gps_parser> gps_parser_factory::create(const parsers_t p_parser) { 
      std::shared_ptr<gps_parser> p;
      switch (p_parser) {
        case parsers_t::nmea_2000:
          p.reset(new nmea2000()); 
          break;
      } // End of 'switch' statement

      return p;
    }

  } // End of namespace parsers

} // End of namespace gps

using namespace gps::parsers;
