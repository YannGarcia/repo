/*!
 * \file      ibstream.cpp
 * \brief     Implementation file for input binary stream.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "ibstream.hh"

namespace helpers {

  uint8_t ibstream::g_masks[] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f };
    
  ibstream::ibstream() : _bits_index(0), _size(0), _buffer() {
  } // End of default ctor

  ibstream::ibstream(const std::vector<uint8_t> & p_input_buffer, const uint32_t p_size) : _bits_index(0), _size(p_size), _buffer(p_input_buffer), _state(std::ios_base::iostate::_S_goodbit) {
  }

  ibstream::~ibstream() {
    close();
  } // End of dtor

  void ibstream::open(const std::vector<uint8_t> & p_input_buffer, const uint32_t p_size) {
    _bits_index = 0;
    _size = p_size;
    _buffer.clear();
    _buffer.assign(p_input_buffer.cbegin(), p_input_buffer.cend());
    _state = std::ios_base::iostate::_S_goodbit;
  } // End of default ctor
  
  void ibstream::close() {
    _buffer.clear();
    _size = 0;
    _bits_index = 0;
    _state = std::ios_base::iostate::_S_eofbit;
  } // End of dtor
  
  uint32_t ibstream::seekg(const uint32_t p_position) { 
    //std::clog << std::dec << ">>> seekp(1): " << p_position << std::endl;

    // Sanity check
    if (p_position >= _size) {
      //std::clog << "seekg(2): No more bits to read" << std::endl;
      return -1;
    }

    _bits_index = p_position; 
    //std::clog << "_bits_index=" << _bits_index << std::endl;
    if (_bits_index >= _size) { 
      _state = std::ios_base::iostate::_S_eofbit; 
    } else { 
      _state = std::ios_base::iostate::_S_goodbit; 
    } 

    return 0;
  }

  uint32_t ibstream::seekg(const int32_t p_offset, std::ios_base::seekdir p_direction) { 
    //std::clog << std::dec << ">>> seekp(2): " << p_offset << " - " << (uint32_t)p_direction << std::endl;

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
    if (_bits_index >= _size) { 
      _state = std::ios_base::iostate::_S_eofbit; 
    } else { 
      _state = std::ios_base::iostate::_S_goodbit; 
    } 

    return 0;
  }
  
  std::vector<uint8_t> ibstream::peek(const uint32_t p_bits_to_read) {
    // Sanity check
    std::vector<uint8_t> buffer;
    if (_state == std::ios_base::iostate::_S_eofbit) { // All bits were read
      //std::clog << "peek: No more bits to read" << std::endl;
      return buffer;
    }

    // Backup the index
    uint32_t bits_index = _bits_index;
    // Normal read
    read_bits(buffer, p_bits_to_read);
    // Restore the index
    _bits_index = bits_index;

    return buffer;
  } // End of method peek

  uint32_t ibstream::read_bits(std::vector<uint8_t> & p_buffer, const uint32_t p_bits_to_read) {
    //std::clog << std::dec << ">>> read_bits: " << p_bits_to_read << std::endl;

    // Sanity check
    if (_state == std::ios_base::iostate::_S_eofbit) { // All bits were read
      //std::clog << "read_bits: No more bits to read" << std::endl;
      p_buffer.clear();
      return 0;
    }

    // Calculate the number of bits to read
    //std::clog << "_size=" << _size << std::endl << "_bits_index=" << _bits_index << std::endl;
    uint32_t bits_to_read = ((_size - _bits_index) < p_bits_to_read) ? _size - _bits_index : p_bits_to_read;

    // Calculate the index of the next bit to read in bytes and bits
    uint32_t bits_index_in_bytes = _bits_index / (8 * sizeof(uint8_t));
    uint8_t bits_index_in_bits = _bits_index % (8 * sizeof(uint8_t));

    // Calculate the number of bytes and bits to read
    uint32_t bytes_to_read = bits_to_read / (8 * sizeof(uint8_t));
    uint8_t bits_remaining = bits_to_read % (8 * sizeof(uint8_t));

    // Allocate space in output buffer
    //std::clog << "calculated p_buffer size=" << static_cast<size_t>(bytes_to_read + ((bits_remaining != 0) ? 1 : 0)) << std::endl;
    p_buffer.assign(static_cast<size_t>(bytes_to_read + ((bits_remaining != 0) ? 1 : 0)), 0x00);
    //std::clog << "p_buffer size=" << p_buffer.size() << std::endl;
    
    //std::clog << "bits_to_read= " << bits_to_read << std::endl << "bits_index_in_bytes=" << bits_index_in_bytes << std::endl << "bits_index_in_bits=" << (uint32_t)bits_index_in_bits << std::endl << "bytes_to_read=" << bytes_to_read << std::endl << "bits_remaining=" << (uint32_t)bits_remaining << std::endl;

    // Read bytes first
    if (bytes_to_read != 0) {
      if (bits_index_in_bits == 0) { // There is no left bits to read first 
	// Copy the bytes
	//std::clog << "# bytes to copy(1)=" << bytes_to_read << std::endl;
	std::copy(
		  _buffer.cbegin() + bits_index_in_bytes,
		  _buffer.cbegin() + bits_index_in_bytes + bytes_to_read,
		  p_buffer.begin()
		  );

	// Update current bits index
	_bits_index += bytes_to_read * (8 * sizeof(uint8_t));
	//std::clog << std::dec << "_bits_index(1)=" << _bits_index << std::endl;

	// Calculate the index of the next bit to read in bytes and bits
	bits_index_in_bytes = _bits_index / (8 * sizeof(uint8_t));
	bits_index_in_bits = _bits_index % (8 * sizeof(uint8_t));
	//std::clog << "bits_index_in_bytes(1)=" << (uint32_t)bits_index_in_bytes << std::endl << "bits_index_in_bits(3)=" << (uint32_t)bits_index_in_bits << std::endl;
      } else { // Some bits shall be read first
	//std::clog << "g_masks[" << (uint32_t)bits_index_in_bits << "](2)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)g_masks[bits_index_in_bits - 1] << std::endl;
	for (uint32_t i = 0; i < bytes_to_read; i++) {
	  //std::clog << "p_buffer[" << i << "](2.0)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[i] << std::endl << "_buffer[i + bits_index_in_bytes](2)=" << (uint32_t)_buffer[i + bits_index_in_bytes] << std::endl << "shift=" << (uint32_t)bits_index_in_bits << std::endl;

	  p_buffer[i] |= _buffer[i + bits_index_in_bytes] << (uint32_t)bits_index_in_bits;
	  //std::clog << "p_buffer[" << i << "](2.1)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[i] << std::endl;

	  //std::clog << "_buffer[" << i + 1 + bits_index_in_bytes << "] >> " << ((8 * sizeof(uint8_t)) - bits_index_in_bits) << "(2.2)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)(_buffer[i + 1 + bits_index_in_bytes] >> ((8 * sizeof(uint8_t)) - bits_index_in_bits)) << std::endl;
	  p_buffer[i] |= (_buffer[i + 1 + bits_index_in_bytes] >> ((8 * sizeof(uint8_t)) - bits_index_in_bits)) & g_masks[bits_index_in_bits - 1];
	  //std::clog << "p_buffer[" << i << "](2.3)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[i] << std::endl;

	  uint8_t shifted_bits = (_buffer[i + 1 + bits_index_in_bytes] << (uint32_t)bits_index_in_bits);
	  //std::clog << "shifted_bits(2)=" << shifted_bits << std::hex << std::setw(2) << std::setfill('0') << ((uint32_t)shifted_bits & 0x000000ff) << std::endl;
	  
	  p_buffer[i + 1] = shifted_bits;
	  //std::clog << "p_buffer[" << i + 1 << "](2)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[i + 1] << std::endl;
	} // End of 'for' statement
	
	// Update current bits index
	_bits_index += (bytes_to_read * (8 * sizeof(uint8_t)));
	//std::clog << "_bits_index(2)=" << _bits_index << std::endl;
	//std::clog << "bits_remaining(2)=" << (uint32_t)bits_remaining << std::endl;	

	if (bits_remaining != 0) {
	  // Calculate the index of the next bit to read in bytes and bits
	  bits_index_in_bytes = _bits_index / (8 * sizeof(uint8_t));
	  bits_index_in_bits = _bits_index % (8 * sizeof(uint8_t));
	  //std::clog << "bits_index_in_bytes(2)=" << (uint32_t)bits_index_in_bytes << std::endl << "bits_index_in_bits(2)=" << (uint32_t)bits_index_in_bits << std::endl;
	  p_buffer[p_buffer.size() - 1] |= (_buffer[bits_index_in_bytes] >> ((8 * sizeof(uint8_t)) - bits_index_in_bits)) & g_masks[bits_index_in_bits - 1];
	  //std::clog << "p_buffer[" << p_buffer.size() - 1 << "](2)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[p_buffer.size() - 1] << std::dec << std::endl;
	}
	
	// Check the end of the buffer
	if (_bits_index >= _size) {
	  _state = std::ios_base::iostate::_S_eofbit;
	}
	
	//std::clog << "<<< read_bits: " << bits_to_read << std::endl;
	return bits_to_read;
      }
    }
    
    if (bits_remaining != 0) {
      // Read the right bits
      //std::clog << "_buffer[" << bits_index_in_bytes << "](3)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)_buffer[bits_index_in_bytes] << std::dec << std::endl;
      //std::clog << "g_masks[" << (uint32_t)bits_remaining << "](3)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)g_masks[bits_remaining - 1] << std::dec << std::endl << "Shift(3)=" << ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_remaining) << std::endl << "mask(3):" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)(g_masks[bits_remaining - 1] << ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_remaining)) << std::endl << "value(3):" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[p_buffer.size() - 1] << std::dec << std::endl;
      p_buffer[p_buffer.size() - 1] |= (_buffer[bits_index_in_bytes] & g_masks[bits_remaining - 1] << ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_remaining));
      //std::clog << "p_buffer[" << p_buffer.size() - 1 << "](3.1)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[p_buffer.size() - 1] << std::dec << std::endl;
      if (p_buffer.size() == 1) { 
	p_buffer[0] >>= ((8 * sizeof(uint8_t)) - bits_index_in_bits - bits_remaining);
	//std::clog << "p_buffer[0](3.2)=" << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)p_buffer[0] << std::dec << std::endl;
      }
      _bits_index += bits_remaining;
      //std::clog << "_bits_index(3)=" << _bits_index << std::endl;
    }
    
    // Check the end of the buffer
    if (_bits_index >= _size) {
      _state = std::ios_base::iostate::_S_eofbit;
    }
	
    //std::clog << "<<< read_bits: " << bits_to_read << std::endl;
    return bits_to_read;
  } // End of method read_bits

  const bool ibstream::is_equal(const uint8_t * p_buffer, const uint32_t p_length) const {
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
