/**
 * @File    ublox.cpp
 * @brief   Implementation file for the lightweight UBLOX GPS device library.
 * @author  garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <chrono>

#include "ublox.h"

namespace gps {
  
  ublox::ublox(const std::string & p_gps_device_name, std::shared_ptr<gps_parser> p_gps_parser) : gps_device(p_gps_device_name, p_gps_parser), _rs232(p_gps_device_name, 9600) { 
    //reset_decoder_ubx();
    // Configure the device
    // configure();
  }

  ublox::~ublox() {
  }

  void ublox::run() {
    _running = true;
    while (_running) {
      // Read available data if any
      if (_rs232.data_available()) {
	std::vector<uint8_t> frame(_rs232.data_available());
	// Read data
	_rs232.read(frame);
	// Parse them
	if (_gps_parser->process(frame) != 0) { // An error occurs
	  std::this_thread::sleep_for(std::chrono::milliseconds(300));
	} // else, nothing to do, application shall call get_result method 
      } else {
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
      }
      
    } // end of 'while' statement
  }

} // End of namespace gps
