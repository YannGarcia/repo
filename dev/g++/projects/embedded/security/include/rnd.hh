/*!
 * \file      rnd.hh
 * \brief     Header file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2020 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <memory>
#include <cstdint>

#include <cryptopp/osrng.h>

namespace security {

  /*!
   * \class rnd
   * \brief Random class to generatate randm vector of specified size
   */
  class rnd {
    CryptoPP::AutoSeededRandomPool _rng;

    static std::unique_ptr<rnd> _instance; /*!< Unique instance of rnd class*/

    /*!
     * \brief Private constructor
     */
    rnd() { };

  public: /*!< \publicsection */

    /*!
     * \brief Destructor
     */
    virtual ~rnd() { } ;

    static rnd& get_instance() { return *_instance; };

    int32_t generate(const size_t p_length, std::vector<uint8_t>& p_vector);

  }; // End of class rnd

} // End of namespace

