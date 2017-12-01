/*!
 * \file      sha.hh
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
   * \class sha
   * \brief SHA-x class to be used to produce message digest based on SHA-x algorithm
   */
  class sha {
    sha_algorithms_t _algorithms; /*!< SHA algorithm used to create the sha object */
    
    std::shared_ptr<void> _sha; /*!< The pointer to the sha object*/

    /*!
     \fn void sha::shared_ptr_deleter(const void* p_sha, const sha_algorithms_t p_sha_algorithms);
     * \brief Custom deleter for the _sha shared pointer
     * \param[inout] p_sha The pointer to delete
     * \param[in] p_sha_algorithms The SHA algorithm used to create the sha object
     * \static
     */
    static void shared_ptr_deleter(const void* p_sha, const sha_algorithms_t p_sha_algorithms);
    
  public: /*!< \publicsection */

    /*!
     * \brief Constructor
     * \param[in] p_sha_algorithms The SHA algorithm to use
     */
    sha(const sha_algorithms_t p_sha_algorithms);
    
    /*!
     * \brief Destructor
     */
    virtual ~sha();

    int32_t hash(const std::vector<uint8_t> p_data, std::vector<uint8_t>& p_hash) const;
    
  }; // End of class sha

} // End of namespace security

