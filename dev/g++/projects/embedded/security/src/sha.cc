/*!
 * \file      sha.cc
 * \brief     Implementation file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <cryptopp/sha.h>

#include "sha.hh"

#include "logger.hh"

namespace security {

  sha::sha(const sha_algorithms_t p_sha_algorithms): _algorithms(p_sha_algorithms), _sha(nullptr) {
    switch (_algorithms) {
    case sha1:
      _sha.reset(new CryptoPP::SHA1, [p_sha_algorithms](const void* p_sha){sha::shared_ptr_deleter(p_sha, p_sha_algorithms);});
      break;
    case sha256:
      _sha.reset(new CryptoPP::SHA256, [p_sha_algorithms](const void* p_sha){sha::shared_ptr_deleter(p_sha, p_sha_algorithms);});
      break;
    } // End of 'switch' statement
  } // End of contructor

  sha::~sha() {
    _sha.reset();
  } // End of destructor

  void sha::shared_ptr_deleter(const void* p_sha, const sha_algorithms_t p_sha_algorithms) {
    switch (p_sha_algorithms) {
    case sha1:
      delete (CryptoPP::SHA1*)p_sha;
      break;
    case sha256:
      delete (CryptoPP::SHA256*)p_sha;
      break;
    } // End of 'switch' statement
  } // End of shared_ptr_deleter

  int32_t sha::hash(const std::vector<uint8_t> p_data, std::vector<uint8_t>& p_hash) const {
    // Sanity check
    if (_sha.get() == nullptr) {
      return -1;
    }
    
    switch (_algorithms) {
    case sha1:
      p_hash.resize(CryptoPP::SHA1::DIGESTSIZE);
      ((CryptoPP::SHA1*)_sha.get())->CalculateDigest(static_cast<uint8_t *>(p_hash.data()), p_data.data(), p_data.size());
      break;
    case sha256:
      p_hash.resize(CryptoPP::SHA256::DIGESTSIZE);
      ((CryptoPP::SHA256*)_sha.get())->CalculateDigest(static_cast<uint8_t *>(p_hash.data()), p_data.data(), p_data.size());
      break;
    } // End of 'switch' statement

    return 0;
  }
  
} // End of namespace security

