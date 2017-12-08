/*!
 * \file      keys_pair.hh
 * \brief     Header file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <memory>
#include <cstdint>

#include "sha_algorithms.hh"

namespace security {

  /*!
   * \class keys_pair
   * \brief KEYS_PAIR-x class to be used to produce message digest based on KEYS_PAIR-x algorithm
   */
  class keys_pair {
    sha_algorithms_t _algorithms; /*!< SHA algorithm used to create the sha object */
    std::shared_ptr<void> _private_key;    /*!< */
    std::shared_ptr<void> _public_key;     /*!< */
    std::vector<uint8_t> _private_key_v;   /*!< */
    std::vector<uint8_t> _public_key_v_x; /*!< */
    std::vector<uint8_t> _public_key_v_y; /*!< */

    static void private_key_ptr_deleter(const void* p_key, const sha_algorithms_t p_sha_algorithms);
    static void public_key_ptr_deleter(const void* p_key, const sha_algorithms_t p_sha_algorithms);
    
  public: /*!< \publicsection */

    /*!
     * \brief Constructor
     * \param[in] p_sha_algorithms The SHA algorithm to use
     */
    keys_pair(const sha_algorithms_t p_sha_algorithms);
    
    /*!
     * \brief Destructor
     */
    virtual ~keys_pair();

    int32_t generate();

    inline std::vector<uint8_t>& get_private_key() { return _private_key_v; };
    inline std::vector<uint8_t>& get_public_key_x() { return _public_key_v_x; };
    inline std::vector<uint8_t>& get_public_key_y() { return _public_key_v_x; };

    std::string to_string();
    
  }; // End of class keys_pair

} // End of namespace security

