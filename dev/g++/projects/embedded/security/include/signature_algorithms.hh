/*!
 * \file      signature_algorithms.hh
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
   * \enum signature_algorithms_t
   * \brief List of supported signature algorithms
   */
  enum signature_algorithms_t : uint8_t { 
    ecdsa   = 0x01    /*!< Elliptic curve digital signature algorithm */
  }; // End of enum signature_algorithms_t
  
} // End of namespace security
