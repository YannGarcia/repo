/**
 * @file    istram.t.h
 * @brief   Main header file for input binary stream templatea.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include "ibstream.h"
#include "converter.h"

namespace helpers {
  
  template<typename T> uint32_t ibstream::read(T & p_value, const uint32_t p_bits_to_read) {
    //    std::clog << ">>> read(T)" << std::endl; 

    uint32_t result = 0;
    std::vector<unsigned char> value;
    if (read_bits(value, p_bits_to_read) == 0) {
      return 0;
    } else {
      if (std::is_floating_point<T>::value) { // 'float' and 'int' have the same size in bytes
	p_value = converter::get_instance().bytes_to_float(value);
      } else {
	switch (sizeof(T)) {
	case sizeof(int8_t):
	  p_value = static_cast<uint8_t>(value[0]);
	  break;
	case sizeof(int16_t):
	  p_value = converter::get_instance().bytes_to_short(value);
	  break;
	case sizeof(int32_t):
	  p_value = converter::get_instance().bytes_to_int(value);
	  break;
	case sizeof(int64_t):
	  p_value = converter::get_instance().bytes_to_long(value);
	  break;
	default:
	std::cerr << "converter::ibstream::read(T): Unsupported type, size=" << (uint32_t)sizeof(T) << std::endl; 	  
	  break;
	} // End of 'switch' statement
      }
    }
    
    //    std::clog << "<<< read(T):" << (uint32_t)result << " - value:" << (int64_t)p_value << std::endl; 
    return p_bits_to_read;
  }; // End of method read
  
} // End of namespace helpers
