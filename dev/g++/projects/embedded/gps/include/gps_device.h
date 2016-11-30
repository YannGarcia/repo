/**
 * @File    gps_device.h
 * @brief   Header file for the lightweight gps devices library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <memory>
#include <stdexcept>

#include "devices.h"
#include "parsers.h"
#include "gps_parser.h"

#include "runnable.h"

namespace gps {

  /**
   * @class gps_device
   * @brief This class provides a set of methods for lightweight GPS devices
   */
  class gps_device : public runnable {
  protected:
    std::string _gps_device_name;
    std::shared_ptr<gps_parser> _gps_parser;

  public:
    /**
     * @brief Default constructor shall not be used
     */
    gps_device() : _gps_device_name(), _gps_parser() { throw new std::runtime_error("Specialized ctor shall be implemented"); };
    /**
     * @brief Specialized constructor
     * @param[in] p_gps_device_name The GPS device name
     * @param[in] p_gps_parser The GPS protocol parser to be used for GPS frame processing
     */
    gps_device(const std::string & p_gps_device_name, std::shared_ptr<gps_parser> & p_gps_parser) : _gps_device_name(p_gps_device_name), _gps_parser(p_gps_parser) { };
    virtual ~gps_device() { };

    virtual inline const std::map<uint8_t, std::string> & get_result() const { return _gps_parser->get_result(); };
    virtual inline void stop() { runnable::stop(); };

  protected:
    virtual inline void run() { throw new std::runtime_error("Method shall be overwritten"); };

  }; // End of class gps_device

} // End of namespace gps

using namespace gps;
