/*!
 * \file      ecdsa_signature.hh
 * \brief     Header file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <vector>
#include <cstdint>

#include "keys_pair.hh"

namespace security {

  /*!
   * \class ecdsa_signature
   * \brief ECDSA_SIGNATURE-x class to be used to produce message digest based on ECDSA_SIGNATURE-x algorithm
   */
  class ecdsa_signature {
    
  public: /*!< \publicsection */

    /*!
     * \brief Constructor
     * \param[in] p_sha_algorithms The SHA algorithm to use
     */
    ecdsa_signature(const keys_pair& p_keys);
    
    /*!
     * \brief Destructor
     */
    virtual ~ecdsa_signature();

    int sign(const std::vector<uint8_t> p_message, std::vector<uint8_t> p_signature);
    bool verify_sign(const std::vector<uint8_t> p_message, const std::vector<uint8_t> p_signature);


    std::string to_string();
    
  }; // End of class ecdsa_signature

} // End of namespace security

