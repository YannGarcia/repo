/**
 * @file      obstream.h
 * @brief     Header file for output binary stream.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

#include <ios>
#include <vector>
#include <cstdint>

namespace helpers {

  /**
   * @class Output binary stream
   */
  class obstream {
    /*!< Bit masks */
    static uint8_t g_masks[];
    
  public:
    /**
     * @brief Default ctor
     * @param[in] The default size of the output data in bytes
     */
    obstream(const uint32_t p_capacity = 128);

    /**
     * @brief Default dtor
     */
    virtual ~obstream();

    /**
     * @brief Create a new binary output data stream
     * @param[in] The default size of the output data in bytes
     */
    void open(const uint32_t p_capacity = 128);

    /**
     * @brief Close the binary output data stream
     */
    void close();

    /**
     * @brief Write one or more bits from the current stream
     * @param[in] p_buffer The buffer to write
     * @param[in] p_bits_to_write The number of bits to write
     * @return The number of bits write
     */
    //inline uint32_t write<std::vector<uint8_t>& >(const std::vector<uint8_t> & p_buffer, const uint32_t p_bits_to_write) { return write_bits(p_buffer, p_bits_to_write); };

    /**
     * @brief Write one or more bits from the current stream
     * @param[in] p_value The value to write
     * @param[in] p_bits_to_write The number of bits to write
     * @return The number of bits write
     */
    template<typename T> uint32_t write(const T & p_value, const uint32_t p_bits_to_write = 8 * sizeof(uint8_t) * sizeof(T));

    /**
     * @brief Write one or more bits from the current stream
     * @param[in] p_buffer The buffer to write
     * @param[in] p_bits_to_write The number of bits to write
     * @return The number of bits write
     */
    uint32_t write_bits(const std::vector<uint8_t> & p_buffer, const uint32_t p_bits_to_write);

    /**
     * @brief Write one or more bits from the current stream
     * @param[in] p_buffer The buffer write
     * @param[in] p_bits_to_write The number of bits to write
     * @return The number of bits write
     */
    uint32_t write_bits(const uint8_t p_bits, const uint32_t p_bits_to_write);
    
    /**
     * @brief Changing the current read position
     * @param[in] p_position New absolute position within the stream
     * @return 0 on success, -1 otherwise 
     */
    uint32_t seekp(const uint32_t p_position);

    /**
     * @brief Changing the current read position
     * @param[in] p_offset Offset value, relative to the p_direction parameter
     * @param[in] p_direction The way parameter
     * @return 0 on success, -1 otherwise 
     */
    uint32_t seekp(const int32_t p_offset, const std::ios_base::seekdir p_direction);

    /**
     * @brief Compare bit streams
     * @param[in] p_buffer The pointer to the bit buffer to compare to
     * @param[in] p_length The length in bits of the buffer to compare to
     * @return true on success, false otherwise
     */
    const bool is_equal(const uint8_t * p_buffer, const uint32_t p_length) const;

    /**
     * @brief Returns a pointer to the internal bit buffer
     * @return The pointer to the internal bit buffer
     */
    inline const uint8_t * rdbuf() const { return _buffer.data(); };

    /**
     * @brief Compare bit streams
     * @param[in] p_obstream The bit stream to compare to
     * @return true if streams are equals, false otherwise
     */
    inline const bool operator== (const obstream & p_obstream) const { return is_equal(p_obstream.rdbuf(), p_obstream.gcount()); };

    /**
     * @brief Compare bit streams
     * @param[in] p_obstream The bit stream to compare to
     * @return true if streams are not equals, false otherwise
     */
    inline const bool operator!= (const obstream & p_obstream) const { return !is_equal(p_obstream.rdbuf(), p_obstream.gcount()); };

    /**
     * @brief Returns the position of the current character in the output stream
     * @return The position of the current character in the output stream
     */
    inline uint32_t tellp() const { return _bits_index; };

    /**
     * @brief Indicate the size of the buffer in bits
     * @return The size of the buffer in bits
     */
    inline uint32_t gcount() const { return _bits_index; };

    /**
     * @brief Indicate the size of the buffer in bits
     * @return The size of the buffer in bits
     */
    inline void reset() { _buffer.clear(); _bits_index = 0; };

    /**
     * @brief Retrieve the output bits stream
     * @return The output bits stream as a bytes vector
     */
    inline const std::vector<uint8_t> & to_bytes() const { return _buffer; };

  protected:
    /**
     * @brief The index of the next bits to process
     */
    uint32_t _bits_index;
    /**
     * @brief The buffer of bits
     */
    std::vector<uint8_t> _buffer;
    std::ios_base::iostate _state;
  }; // End of class obstream

} // End of namespace helpers

#include "obstream.t.h"

using namespace helpers;
