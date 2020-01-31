/*!
 * \file      rnd.cc
 * \brief     Implementation file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2020 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */

#include <cryptopp/secblock.h>

#include "rnd.hh"

namespace security {

  std::unique_ptr<rnd> rnd::_instance(new rnd);

  int32_t rnd::generate(const size_t p_length, std::vector<uint8_t>& p_vector) {
    CryptoPP::SecBlock<uint8_t> block(p_length);
    p_vector.resize(p_length);
    _rng.GenerateBlock(p_vector.data(), p_vector.size());
    return p_vector.size();
  }

} // End of namespace security
