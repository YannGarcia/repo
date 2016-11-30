/**
 * @file    keyboard.h
 * @brief   Main header file for the keyboard key stricked helper class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <cstdint>

namespace helpers {

  /**
   * @class keyboard
   * @brief Helper class for key stricked interception
   */
  class keyboard {
      
  public:
    /**
     * @brief Default constructor
     */
    keyboard() { };
    /**
     * @brief Default destructor
     */
    ~keyboard() { };
      
    /**
     * @brief Check if key stricked
     * @return 1 on success, 0 otherwise
     */
    static uint32_t kbhit();
      
  }; // End of class keyboard

} // End of namespace helpers

using namespace helpers;
