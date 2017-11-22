/**
 * \file      ibstream.h
 * \brief     Header file for input binary stream.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <ios>
#include <vector>
#include <cstdint>

namespace helpers {

  /**
   * \class ibstream
   * \brief Input binary stream
   */
  class ibstream {
    /*!< Bit masks */
    static uint8_t g_masks[];

  public:
    /**
     * \brief Default ctor
     */
    ibstream();

    /**
     * \brief Creation ctor
     * \param[in] p_input_buffer The binary input data of the stream
     * \param[in] p_size The size of the input data in bits
     */
    ibstream(const std::vector<uint8_t> & p_input_buffer, const uint32_t p_size);

    /**
     * \brief Default dtor
     */
    virtual ~ibstream();
    
    /**
     * \brief Create a new binary input data stream
     * \param[in] p_input_buffer The binary input data of the stream
     * \param[in] p_size The size of the input data in bits
     */
    void open(const std::vector<uint8_t> & p_input_buffer, const uint32_t p_size);

    /**
     * \brief Close the binary input data stream
     */
    void close();

    /**
     * \brief Formated read of one or more bits from the current stream
     * \param[out] p_value The value to read 
     * \param[in] p_bits_to_read The number of bits to read
     * \return The number of bits read
     */
    template<typename T> uint32_t read(T & p_value, const uint32_t p_bits_to_read = 8 * sizeof(uint8_t) * sizeof(T));

    /**
     * \brief Read one or more bits from the current stream
     * \param[out] p_buffer The buffer to store the read bits
     * \param[in] p_bits_to_read The number of bits to read
     * \return The number of bits read
     */
    uint32_t read_bits(std::vector<uint8_t> & p_buffer, const uint32_t p_bits_to_read);

    /**
     * \brief Skip one or more bits from the current stream
     * \param[in] p_bits_to_ignore The number of bits to ignore
     * \return The number of bits ignored
     */
    inline uint32_t ignore(const uint32_t p_bits_to_ignore) { _bits_index += p_bits_to_ignore; return _bits_index; };

    /**
     * \brief Changing the current read position
     * \param[in] p_position New absolute position within the stream
     */
    uint32_t seekg(const uint32_t p_position);

    /**
     * \brief Changing the current read position
     * \param[in] p_offset Offset value, relative to the p_direction parameter
     * \param[in] p_direction The way parameter
     * \return 0 on success, -1 otherwise
     */
    uint32_t seekg(const int32_t p_offset, const std::ios_base::seekdir p_direction);
    
    /**
     * \brief Peek next bits
     * \param[in] p_bits_to_read The number of bits to peek
     * \return The vector containing the bits on success, an empty vector otherwise
     */
    std::vector<uint8_t> peek(const uint32_t p_bits_to_read = 8);
    
    /**
     * \brief Compare bit streams
     * \param[in] p_buffer The pointer to the bit buffer to compare to
     * \param[in] p_length The length in bits of the buffer to compare to
     * \return true on success, false otherwise
     */
    const bool is_equal(const uint8_t * p_buffer, const uint32_t p_length) const;

    /**
     * \brief Returns a pointer to the internal bit buffer
     * \return The pointer to the internal bit buffer
     */
    inline const uint8_t * rdbuf() const { return _buffer.data(); };

    /**
     * \brief Compare bit streams
     * \param[in] p_ibstream The bit stream to compare to
     * \return true if streams are equals, false otherwise
     */
    inline const bool operator== (const ibstream & p_ibstream) const { return is_equal(p_ibstream.rdbuf(), p_ibstream.gcount()); };

    /**
     * \brief Compare bit streams
     * \param[in] p_ibstream The bit stream to compare to
     * \return true streams are not equal, false otherwise
     */
    inline const bool operator!= (const ibstream & p_ibstream) const { return !is_equal(p_ibstream.rdbuf(), p_ibstream.gcount()); };

    /**
     * \brief Returns the position of the current bit in the input stream
     * \return The position of the current bit in the input stream
     */
    inline uint32_t tellg() const { return _bits_index; };

    /**
     * \brief Indicate the size of the buffer in bits
     * \return The size of the buffer in bits
     */
    inline uint32_t gcount() const { return _size; };

    /**
     * \brief Indicate the size of the remaining bits to read from the stream
     * \return The size of the remaining bits to read
     */
    inline uint32_t gremaining() const { return _size - _bits_index; };
    
  protected:
    /**
     * \brief The index of the next bits to process
     */
    uint32_t _bits_index;
    /**
     * \brief The size in bits on the buffer
     */
    uint32_t _size;
    /**
     * \brief The buffer of bits
     */
    std::vector<uint8_t> _buffer;
    std::ios_base::iostate _state;
  }; // End of class ibstream

} // End of namespace helpers

#include "ibstream.t.h"

using namespace helpers;
