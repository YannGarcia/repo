/**
 * \file      helper.h
 * \brief     Header file for the Helper library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2020 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>

#include <vector>
#include <list>
#include <algorithm>

namespace helpers {

  /**
   * \class helper
   * \brief Helper class to manipulate vectors/buffers of type T
   * \remark Pattern singleton
   */
  class helper {

    /**
     * \brief Unique instance of this class
     */
    static helper * instance;

    /**
     * \brief Default constructor
     */
    helper() {};
    /**
     * \brief Default destructor
     */
    ~helper() {
      if (instance != NULL) {
        delete instance;
        instance = NULL;
      }
    };

  public:
    /**
     * \brief Unique instance accessor
     */
    inline static helper & get_instance() {
      if (instance == NULL) instance = new helper();
      return *instance;
    };

  public:
    /**
     * \brief Concatenate two or more vectors
     * \param[in] p_buffer The list of the vectors to concatenate
     * \return The concatenated vectors
     */
    template<typename T> std::vector<T> concat(std::list<std::vector<T> > & p_buffer);

    /**
     * \brief Extract part of a vector
     * \param[in] p_buffer The vector to extract
     * \param[in] p_offset The offset to start. Default: 0
     * \param[in] p_length The number of bytes to extract. Default: -1 for all remaining bytes
     * \return The extracted vectors
     */
    template<typename T> std::vector<T> extract(const std::vector<T> p_buffer, const unsigned int p_offset = 0, const unsigned int p_length = -1);

    /**
     * \brief Generate an hexadecimal dump of the provided bytes array. The output is sent to the consile
     * \param[in] p_buffer The vector to extract
     * \param[in] p_offset The offset to start. Default: 0
     * \param[in] p_length The number of bytes to dump. Default: -1 for all remaining bytes
     * \return The extracted vectors
     */
    void hexa_dump(const std::vector<unsigned char> p_buffer, const unsigned int p_offset = 0, const unsigned int p_length = -1);

    /**
     * \brief Generate an hexadecimal dump of the provided bytes array. The output is sent to the consile
     * \param[in] p_output The output stream
     * \param[in] p_buffer The vector to extract
     * \param[in] p_offset The offset to start. Default: 0
     * \param[in] p_length The number of bytes to dump. Default: -1 for all remaining bytes
     * \return The extracted vectors
     */
    void hexa_dump(std::ostringstream & p_output, const unsigned char * p_buffer, const unsigned int p_offset = 0, const unsigned int p_length = -1);

    /**
     * \brief Generate an hexadecimal dump of the provided bytes array. The output is sent to the consile
     * \param[in] p_buffer The vector to extract
     * \param[in] p_offset The offset to start. Default: 0
     * \param[in] p_length The number of bytes to dump. Default: -1 for all remaining bytes
     * \return The extracted vectors
     */
    void hexa_dump(const unsigned char * p_buffer, const unsigned int p_offset, const unsigned int p_length);

    /**
     * \brief Retrieve the size of a file
     * \param[in] p_file_name The full file name
     * \return The size of a file on success, -1 otherwise
     */
    int file_size(const std::string& p_file_name);

    /**
     * \brief Load a file in binary format
     * \param[in] p_file_name The full file name
     * \param[out] p_buffer The file content
     * \return The number of bytes read on success, -1 otherwise
     */
    int file_load(const std::string& p_file_name, std::vector<unsigned char>& p_buffer);

  private:
    /**
     * \brief Convert the specified digit into hexadecimal number (0x30..0x39 (0..9), 0x47..x4c (A..F))
     * \param[in] p_digit The digit to convert
     * \return An hexadecimal digit (0..9-A..F)
     */
    static inline unsigned char to_hex_digit(const unsigned char p_digit) { return ((p_digit < 10) ? (p_digit + 0x30) : (p_digit + 0x37)); };

    /** Convert the specified hexadecimal digit into a character if it is printable, or replace by a '.' otherwise
     * \param[in] p_digit The hexadecimal digit to convert
     * \return A character is it's printable, '.' otherwise
     */
    static inline char to_char_digit(const unsigned char p_digit) { return (((p_digit < 0x20) || (p_digit > 0x80)) ? '.' : (char)p_digit); };

  }; // End of class helper

} // End of namespace helpers

#include "helper.t.h"

using namespace helpers;
