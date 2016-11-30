/**
 * @file    helper.t.h
 * @brief   Main header file for the template Helper library.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include "helper.h"

namespace helpers {
 
  template<typename T> std::vector<T> helper::concat(std::list<std::vector<T> > & p_buffer) {
    // Sanity check
    if (p_buffer.size() == 0) {
      std::vector<T> result;
      return result;
    }

    // Calculate the final vector size
    unsigned int length = 0;
    for (typename std::list<std::vector<T> >::iterator it = p_buffer.begin(); it != p_buffer.end(); ++it) {
      length += it->size();
    } // End of 'for' statement
    //std::clog << "helper::concat: length= " << length << std::endl;
    // Concatenate vectors
    std::vector<T> result;
    result.reserve(length);
    for (typename std::list<std::vector<T> >::iterator it = p_buffer.begin(); it != p_buffer.end(); ++it) {
      result.insert(result.end(), it->begin(), it->end());
    } // End of 'for' statement

    return result;
  }; // End of method helper::concat

  template<typename T> std::vector<T> helper::extract(const std::vector<T> p_value, const unsigned int p_offset, const unsigned int p_length) {
    //    std::clog << ">>> helper::extract: " << p_value.size() << ", " << p_offset << ", " << p_length << std::endl;

    // Sanity check
    if (
	(p_offset >= p_value.size()) || (
					 (p_length != static_cast<unsigned int>(-1)) && 
					 ((p_offset + p_length) >= p_value.size())
					 )) {
      std::vector<T> result;
      return result;
    }

    // Calculate size
    unsigned int length = (p_length ==  static_cast<unsigned int>(-1)) ? p_value.size() - p_offset: p_length;
    //    std::clog << "helper::extract: length=" << length << std::endl;
    std::vector<T> result;
    result.reserve(length);
    result.insert(result.end(), p_value.begin() + p_offset, p_value.begin() + p_offset + length);
    return result;
  }; // End of method helper::extract
  
} // End of namespace helpers
