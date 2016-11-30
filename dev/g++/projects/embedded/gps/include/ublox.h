/**
 * @File    ublox.h
 * @brief   Header file for the lightweight gps devices library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <memory>

#include "gps_device.h"

#include "rs232.h"

namespace gps {
  
  /**
   * @class ublox
   * @brief This class implements a set of methods for lightweight UBLOX GPS device
   */
  class ublox : public gps_device {
  protected:
    rs232 _rs232;
    
  public:
    ublox(const std::string & p_gps_device_name, std::shared_ptr<gps_parser> p_gps_parser);
    virtual ~ublox();
    
  protected:
    virtual void run();
  }; // End of class ublox
  
} // End of namespace gps

using namespace gps;
