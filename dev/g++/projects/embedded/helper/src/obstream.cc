/*!
 * \file      obstream.cpp
 * \brief     Implementation file for output binary stream.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "obstream.hh"

namespace helpers {
  
  uint8_t obstream::g_masks[] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
    
  obstream::obstream(const uint32_t p_capacity) : _bits_index(0), _buffer(p_capacity, 0x00), _state(std::ios_base::iostate::_S_goodbit) {
  } // End of default ctor
  
  obstream::~obstream() {
    close();
  } // End of dtor
  
  void obstream::open(const uint32_t p_capacity) {
    _bits_index = 0;
    _buffer.clear();
    _buffer.assign(p_capacity, 0x00);
    _state = std::ios_base::iostate::_S_goodbit;
  } // End of default ctor
  
  void obstream::close() {
    _buffer.clear();
    _bits_index = 0;
    _state = std::ios_base::iostate::_S_eofbit;
  } // End of dtor
  
  uint32_t obstream::write_bits(const std::vector<uint8_t> & p_buffer, const uint32_t p_bits_to_write) {
    //std::clog << std::dec << ">>> write_bits: " << p_bits_to_write << std::endl;

    // Sanity check
    if (p_bits_to_write == 0) { // No bits were write
      //std::clog << "write_bits: No bits to write" << std::endl;
      return 0;
    } 
    
    // Calculate the number of bits to write
    uint32_t bits_to_write = (p_bits_to_write > (p_buffer.size() * (8 * sizeof(uint8_t)))) ? p_buffer.size() * (8 * sizeof(uint8_t)) : p_bits_to_write;
    //std::clog << "bits_to_write= " << bits_to_write << std::endl;
    
    // Calculate the index of the next bits to write in bytes and bits
    //std::clog << "_bits_index=" << _bits_index << std::endl;
    uint32_t bits_index_in_bytes = _bits_index / (8 * sizeof(uint8_t));
    uint8_t bits_index_in_bits = _bits_index % (8 * sizeof(uint8_t));

    // Calculate the number of bytes and bits to write
    uint32_t bytes_to_write = bits_to_write / (8 * sizeof(uint8_t));
    uint8_t bits_remaining = bits_to_write % (8 * sizeof(uint8_t));
    
    //std::clog << "bits_index_in_bytes=" << bits_index_in_bytes << std::endl << "bits_index_in_bits=" << (uint32_t)bits_index_in_bits << std::endl << "bytes_to_write=" << bytes_to_write << std::endl << "bits_remaining=" << (uint32_t)bits_remaining << std::endl << "Capacity:" << _buffer.capacity() << std::endl;

    // Allocate memory
    if (_buffer.capacity() < (bits_index_in_bytes + bytes_to_write + 1)) {
      _buffer.reserve((bits_index_in_bytes + bytes_to_write + 2));
      //std::clog << "Increased capacity:" << _buffer.capacity() << std::endl;
    }
    
    if (bytes_to_write != 0) {
      if (bits_index_in_bits == 0) { // There is no left bits to write first
        // Copy the bytes
	//std::clog << "# bytes to copy(1)=" << bytes_to_write << std::endl;
	std::copy(
                  p_buffer.cbegin(),
                  p_buffer.cend(),
                  _buffer.begin() + bits_index_in_bytes
                  );
	//std::clog << std::dec << "_buffer size(1)=" << _buffer.size() << std::endl;	

        // Update current bits index
        _bits_index += bytes_to_write * (8 * sizeof(uint8_t));
	//std::clog << std::dec << "_bits_index(1)=" << _bits_index << std::endl;	
	// Re-alculate the index of the next bit to read in bytes and bits
	bits_index_in_bytes = _bits_index / (8 * sizeof(uint8_t));
	bits_index_in_bits = _bits_index % (8 * sizeof(uint8_t));
	//std::clog << "bits_index_in_bytes(1)=" << (uint32_t)bits_index_in_bytes << std::endl << "bits_index_in_bits(3)=" << (uint32_t)bits_index_in_bits << std::endl;
      } else { // Some bits shall be write first
	//std::clog << "### Some bits shall be write first" << std::endl;
      }
    }
    
    if (bits_remaining != 0) {
      //std::clog << "g_masks[" << (uint32_t)bits_remaining << "](3)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)g_masks[bits_remaining - 1] << std::dec << std::endl << "Shift(3)=" << ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_remaining) << std::endl << "mask(3):" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)g_masks[bits_remaining - 1] << std::endl << "value(3):" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[p_buffer.size() - 1] << std::dec << std::endl;

      uint8_t bits_to_shift = (8 * sizeof(uint8_t)) - bits_index_in_bits;
      //std::clog << "bits_to_shift(3.1)=" << (uint32_t)bits_to_shift << std::endl;
      if (bits_to_shift < bits_remaining) {
	// Update first _bit_index
	_bits_index += bits_remaining;
	//std::clog << "_bits_index(3.1)=" << (uint32_t)_bits_index << std::endl;
	// Write the left bits
	_buffer[bits_index_in_bytes] |= (p_buffer[p_buffer.size() - 1] & g_masks[bits_remaining - 1]) >> (bits_remaining - bits_to_shift);
	//std::clog << "_buffer[" << bits_index_in_bytes << "](3.1)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)_buffer[bits_index_in_bytes] << std::dec << std::endl;
	bits_remaining -= bits_to_shift;
	//std::clog << "bits_remaining(3)=" << (uint32_t)bits_remaining << std::endl;
	// Write the right bits
	_buffer[bits_index_in_bytes + 1] = (p_buffer[p_buffer.size() - 1] & g_masks[bits_remaining - 1]) << ((8 * sizeof(uint8_t)) - bits_remaining);
	//std::clog << "_buffer[" << bits_index_in_bytes + 1 << "](3.1)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)_buffer[bits_index_in_bytes + 1] << std::dec << std::endl;
      } else { // Write the right bits
	_buffer[bits_index_in_bytes] |= (p_buffer[p_buffer.size() - 1] & g_masks[bits_remaining - 1]) << ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_remaining);
	//std::clog << "_buffer[" << bits_index_in_bytes << "](3.2)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)_buffer[bits_index_in_bytes] << std::dec << std::endl;
	_bits_index += bits_remaining;
	//std::clog << "_bits_index(3.2)=" << (uint32_t)_bits_index << std::endl;
      }
    }

    //std::clog << "<<< write_bits " << bits_to_write << std::endl;
    return bits_to_write;
  } // End of method write_bits
  
  uint32_t obstream::write_bits(const uint8_t p_bits, const uint32_t p_bits_to_write) {
    //std::clog << std::dec << ">>> write_bits(uint8_t): " << p_bits_to_write << std::endl;
    
    // Sanity check
    if (p_bits_to_write == 0) { // No bits were write
      //std::clog << "write_bits: No bits to write" << std::endl;
      return 0;
    }
    
    // Calculate the number of bits to write
    uint32_t bits_to_write = (p_bits_to_write > (8 * sizeof(uint8_t))) ? 8 * sizeof(uint8_t) : p_bits_to_write;
    
    // Calculate the index of the next bits to write in bytes and bits
    //std::clog << "_bits_index=" << _bits_index << std::endl;
    uint32_t bits_index_in_bytes = _bits_index / (8 * sizeof(uint8_t));
    uint8_t bits_index_in_bits = _bits_index % (8 * sizeof(uint8_t));
    
    //std::clog << "bits_to_write= " << bits_to_write << std::endl << "bits_index_in_bytes=" << bits_index_in_bytes << std::endl << "bits_index_in_bits=" << (uint32_t)bits_index_in_bits << std::endl << "Capacity:" << _buffer.capacity() << std::endl;
    
    // Allocate memory
    if (_buffer.capacity() < (bits_index_in_bytes + 1)) {
      _buffer.reserve((bits_index_in_bytes + 1));
      //std::clog << "Increased capacity:" << _buffer.capacity() << std::endl;
    }
    
    //std::clog << "g_masks[" << (uint32_t)bits_to_write << "]=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)g_masks[bits_to_write - 1] << std::dec << std::endl << "Shift=" << ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_to_write) << std::endl << "mask:" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)g_masks[bits_to_write - 1] << std::endl << "value:" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_bits << std::dec << std::endl;
    uint8_t bits_to_shift = (8 * sizeof(uint8_t)) - bits_index_in_bits;
    //std::clog << "bits_to_shift:" << (uint32_t)bits_to_shift << std::endl;
    if (bits_to_shift < bits_to_write) {
      // Update first _bit_index
      _bits_index += bits_to_write;
      //std::clog << "_bits_index(1)=" << (uint32_t)_bits_index << std::endl;
      // Write the left bits
      _buffer[bits_index_in_bytes] |= (p_bits & g_masks[bits_to_write - 1]) >> (bits_to_write - bits_to_shift);
      //std::clog << "_buffer[" << bits_index_in_bytes << "](1)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)_buffer[bits_index_in_bytes] << std::dec << std::endl;
      bits_to_write -= bits_to_shift;
      //std::clog << "bits_to_write(1)=" << (uint32_t)bits_to_write << std::endl;
      // Write the right bits
      _buffer[bits_index_in_bytes + 1] = (p_bits & g_masks[bits_to_write - 1]) << ((8 * sizeof(uint8_t)) - bits_to_write);
      //std::clog << "_buffer[" << bits_index_in_bytes + 1 << "](1)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)_buffer[bits_index_in_bytes + 1] << std::dec << std::endl;
    } else { // Write the right bits
      if ((bits_to_write == 8) && (bits_index_in_bits == 0)) { // Erase the previous value
	_buffer[bits_index_in_bytes] = p_bits;
      } else { // Need a mask to keep already set bits
	_buffer[bits_index_in_bytes] |= (p_bits & g_masks[bits_to_write - 1]) << ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_to_write);
      }
      //std::clog << "_buffer[" << bits_index_in_bytes << "](2)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)_buffer[bits_index_in_bytes] << std::dec << std::endl;
      _bits_index += bits_to_write;
      //std::clog << "_bits_index=" << (uint32_t)_bits_index << std::endl;
    }
      
    //std::clog << "<<< write_bits(uint8_t) " << bits_to_write << std::endl;
    return bits_to_write;
  } // End of method write_bits
  
  uint32_t obstream::seekp(const uint32_t p_position) {
    //std::clog << std::dec << ">>> seekp(1): " << p_position << std::endl;

    // Sanity check
    if (p_position <= 0) { // No bits were write
      //std::clog << "seekg: wronf parameters" << std::endl;
      return -1;
    }
    // Allocate memory if required
    _bits_index = p_position;
    if (_buffer.capacity() < _bits_index) {
      _buffer.reserve(_bits_index + 1);
      //std::clog << "seekp(1): Increased capacity:" << _buffer.capacity() << std::endl;
    }

    return 0;
  } // End of method seekg
  
  uint32_t obstream::seekp(const int32_t p_offset, const std::ios_base::seekdir p_direction) {
    //std::clog << std::dec << ">>> seekp(2): " << p_offset << " - " << (uint32_t)p_direction << std::endl;

    // Set position
    switch (p_direction) {
    case std::ios_base::beg:
       _bits_index = p_offset;
       if (_bits_index < 0) _bits_index = 0;
      break;
    case std::ios_base::cur:
      _bits_index += p_offset;
      if (_bits_index < 0) _bits_index = 0;
      break;
    case std::ios_base::end:
      _bits_index = _buffer.size() - p_offset;
      if (_bits_index < 0) _bits_index = 0;
      break;
    default:
      return -1;
    } // End of 'switch' statement
    // Allocate memory if required
    if (_buffer.capacity() < _bits_index) {
      _buffer.reserve(_bits_index + 1);
      //std::clog << "seekp(2): Increased capacity:" << _buffer.capacity() << std::endl;
    }
    
    return 0;
  } // End of method seekp
  
  const bool obstream::is_equal(const uint8_t * p_buffer, const uint32_t p_length) const {
    if (p_length != gcount()) {
      return false;
    }
    for (std::vector<uint8_t>::const_iterator it = _buffer.cbegin(); it != _buffer.cend(); ++it) {
      if (*it != *(p_buffer + std::distance(_buffer.begin(), it))) {
	return false;
      }
    } // End of 'for' statement
  
    return true;
  }
  
} // End of namespace helpers
