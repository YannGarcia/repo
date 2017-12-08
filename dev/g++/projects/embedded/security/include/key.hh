/*!
 * \file      key.hh
 * \brief     Header file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <memory>
#include <cstdint>

//#include <cryptopp/eccrypto.h>
namespace CryptoPP {
  template <class EC, class H> struct ECDSA;
  class AutoSeededRandomPool;
  class PrivateKey;
  namespace ASN1 {
  };
};

#include "sha_algorithms.hh"

namespace security {

  /*!
   * \class key
   * \brief KEY-x class to be used to produce message digest based on KEY-x algorithm
   */
  template<class T1, class T2> /* e.g. T1=CryptPP::ECP, T2=CryptoPP::SHA256 */
  class key {
    sha_algorithms_t _algorithms; /*!< SHA algorithm used to create the sha object */
    typename CryptoPP::ECDSA<T1,T2>::PrivateKey _key;    /*!< */
    std::vector<uint8_t> _key_vector; /*!< */

  public: /*!< \publicsection */

    /*!
     * \brief Constructor
     * \param[in] p_sha_algorithms The SHA algorithm to use
     */
    key(const sha_algorithms_t p_sha_algorithms);
    
    /*!
     * \brief Destructor
     */
    virtual ~key();
    
    int32_t generate();
    
    inline std::vector<uint8_t>& get_key() { return _key_vector; };
    
    std::string to_string();
    
  }; // End of class key

} // End of namespace

#include "key.t.h"

