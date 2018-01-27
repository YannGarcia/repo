/*!
 * \file      sha_algorithms.hh
 * \brief     Header file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

/*! \namespace security
 *  \brief security namespace
 */
namespace security {

  /*!
   * \enum sha_algorithms_t
   * \brief List of supported sha algorithms
   */
  enum sha_algorithms_t : uint8_t { // TODO Add 224,226,384,512
    sha1 = 0x00, /*!< Secure Hash Algorithm SHA-1 */
    sha256 = 0x01,  /*!< Secure Hash Algorithm SHA-256 */
    sha384 = 0x02  /*!< Secure Hash Algorithm SHA-384 */
  }; // End of enum sha_algorithms_t
  
} // End of namespace security
