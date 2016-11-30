/**
 * @file    tri_factory.cpp
 * @brief   Implementation file for the tri_factory class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include "tri_factory.h"

namespace internal {
  namespace ttcn3 {
    namespace tri {

      std::unique_ptr<tri_factory> tri_factory::g_instance = std::unique_ptr<tri_factory>(new tri_factory());

    } // End of namespace tri
  } // End of namespace ttcn3
} // End of namespace internal

