/**
 * \file      sha256.hpp
 * \brief     Header file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <vector>
#include <cstdint>

namespace security {

class sha256 {
  std::vector<uint8_t> _vector;

public:
  sha256(): _vector(0x00, 32) {};
  virtual ~sha256() { _vector.clear(); };
} // End of class sha256

} // End of namespace security

