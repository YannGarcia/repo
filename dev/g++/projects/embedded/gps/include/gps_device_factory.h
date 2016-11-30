/**
 * @File    gps_device_factory.h
 * @brief   Header file for the lightweight gps library factory.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <map>
#include <memory>

#include "gps_parser_factory.h"

#include "devices.h"
#include "ublox.h"

namespace gps {

  /**
   * @class gps_device_factory
   * @brief This class provide a set of methods for lightweight GPS factory
   * @remark Singleton pattern
   */
  class gps_device_factory {
    static std::unique_ptr<gps_device_factory> g_instance;
    
    std::map<std::string, std::shared_ptr<gps_device> > _gpss;

    /**
     * @brief Private constructor
     */
    gps_device_factory() : _gpss() { };
    
  public:
    static inline gps_device_factory & get_instance() { return *g_instance; };
    
    ~gps_device_factory() { _gpss.clear(); };
    
    /**
     * @brief Retrieve an instance of GPS
     * @param[in] p_gps_device_name The system device name of the GPS (e.g. /dev/ttyO1)
     * @return A reference to the instance of the GPS on success, an exception is raised otherwise
     * @exception std::invalid_argument
     */
    inline gps_device & get_gps(const std::string & p_gps_device_name);

    /**
     * @brief Add a new instance of GPS
     * @param[in] p_gps_device_name The system device name of the GPS (e.g. /dev/ttyO1)
     * @param[in] p_device The device type
     * @param[in] p_parser The GPS protocol parser
     * @exception std::invalid_argument
     */
    inline void add_gps(const std::string & p_gps_device_name, const devices_t p_device = devices_t::ublox_device, const parsers_t p_parser = parsers_t::nmea_2000);
    
    /**
     * @brief Remove an existing  instance of GPS
     * @param[in] p_gps_device_name The system device name of the GPS (e.g. /dev/ttyO1)
     * @exception std::invalid_argument
     */
    inline void remove_gps(const std::string & p_gps_device_name);
    
  }; // End of class gps_device_factory

  gps_device & gps_device_factory::get_gps(const std::string & p_gps_device_name) {
    std::map<std::string, std::shared_ptr<gps_device> >::iterator it = _gpss.find(p_gps_device_name);
    if (it == _gpss.end()) {
      throw std::invalid_argument("gps_device_factory::get_gps");
    }

    return *_gpss.at(it->first);
  };
  
  void gps_device_factory::add_gps(const std::string & p_gps_device_name, const devices_t p_device, const parsers_t p_parser) {
    std::map<std::string, std::shared_ptr<gps_device> >::iterator it = _gpss.find(p_gps_device_name);
    if (it == _gpss.end()) {
      switch (p_device) {
      case devices_t::ublox_device:
	_gpss.insert(
		     std::pair<std::string, std::shared_ptr<gps_device> >(
									  p_gps_device_name, 
									  std::shared_ptr<gps_device>(
												      new ublox(
												      		p_gps_device_name,
														gps_parser_factory::get_instance().create(p_parser)
														)
												      )
									  )
		     );
	break;
      } // End of 'switch' statement
    }
    
    // Nothing to do
  };
  
  void gps_device_factory::remove_gps(const std::string & p_gps_device_name) {
    std::map<std::string, std::shared_ptr<gps_device> >::iterator it = _gpss.find(p_gps_device_name);
    if (it != _gpss.end()) {
      it->second.reset(); // Reset GPS protocol parser reference
      _gpss.erase(it);
    }
  };
  
  std::unique_ptr<gps_device_factory> gps_device_factory::g_instance(new gps_device_factory());
  
} // End of namespace gps

using namespace gps;
