/**
 * @file      obstream.t.h
 * @brief     Template header file for output binary stream template.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#include <type_traits>

#include "obstream.h"
#include "converter.h"

namespace helpers {
  
  template<typename T> uint32_t obstream::write(const T & p_value, const uint32_t p_bits_to_write) {
    //std::clog << ">>> write(T):" << (uint32_t)sizeof(T) << " - isFloat:" << std::is_floating_point<T>::value << std::endl; 

    uint32_t result = 0;
    if (std::is_floating_point<T>::value) { // 'float' and 'int' have the same size in bytes
      std::vector<uint8_t> convert = converter::get_instance().float_to_bytes(static_cast<const float>(p_value));
      result = write_bits(
			  std::vector<uint8_t>(
					       convert.cbegin() + static_cast<uint32_t>(
											(
											 sizeof(T) - 
											 static_cast<uint32_t>(
													       p_bits_to_write / 8
													       )
											 )
											), 
					       convert.cend()
					       ), 
			  p_bits_to_write
			  );
    } else {
      switch (sizeof(T)) {
      case sizeof(int8_t):
	result = write_bits(static_cast<uint8_t>(static_cast<const uint8_t>(p_value)), p_bits_to_write);
	break;
      case sizeof(int16_t): 
	{
	  std::vector<uint8_t> convert = converter::get_instance().short_to_bytes(static_cast<const int16_t>(p_value));
	  result = write_bits(
			      std::vector<uint8_t>(
						   convert.cbegin() + static_cast<uint32_t>(
											    (
											     sizeof(T) - 
											     static_cast<uint32_t>(
														   p_bits_to_write / 8
														   )
											     )
											    ), 
						   convert.cend()
						   ), 
			      p_bits_to_write
			      );
	}
	break;
      case sizeof(int32_t): 
	{
	  std::vector<uint8_t> convert = converter::get_instance().int_to_bytes(static_cast<const int32_t>(p_value));
	  result = write_bits(
			      std::vector<uint8_t>(
						   convert.cbegin() + static_cast<uint32_t>(
											    (
											     sizeof(T) - 
											     static_cast<uint32_t>(
														   p_bits_to_write / 8
														   )
											     )
											    ), 
						   convert.cend()
						   ), 
			      p_bits_to_write
			      );
	}
	break;
      case sizeof(int64_t): 
	{
	  std::vector<uint8_t> convert = converter::get_instance().long_to_bytes(static_cast<const int64_t>(p_value));
	  result = write_bits(
			      std::vector<uint8_t>(
						   convert.cbegin() + static_cast<uint32_t>(
											    (
											     sizeof(T) - 
											     static_cast<uint32_t>(
														   p_bits_to_write / 8
														   )
											     )
											    ), 
						   convert.cend()
						   ), 
			      p_bits_to_write
			      );
	}
	break;
      default:
	//result = write_bits(p_value, p_bits_to_write);
	std::cerr << "converter::obstream::write(T): Unsupported type, size=" << (uint32_t)sizeof(T) << std::endl; 
	break;
      } // End of 'switch' statement
    }
    
    //std::clog << "<<< write(T):" << (uint32_t)result << std::endl; 
    return result;
  }; // End of method write
  
} // End of namespace helpers
