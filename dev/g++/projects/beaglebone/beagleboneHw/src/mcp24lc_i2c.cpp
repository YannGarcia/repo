/* Raspberry simplified access to Microchip 24LCxx EEPROM Memory device (I2C)
 * Copyright (c) 2013 ygarcia, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

#include <linux/fs.h> // Used for BLKFLSBUF (clear buffer)
#include <unistd.h> // Used for usleep and close

#include "wiringBbb.h"

#include "mcp24lc_i2c.h"
#include "wiringBbbI2C.h" // Using I2C functions
#include "converter.h"
#include "beagleboneHwEx.h"

namespace beagleboneHw {

  namespace i2c {

    unsigned char mcp24lc_i2c::DeviceId = 0x50; // Microchip device identifier

    mcp24lc_i2c::mcp24lc_i2c(const unsigned char p_deviceId, const unsigned char p_wp, const mcp24lc_i2c::Families p_family, const unsigned int p_frequency) {
      // Setup I2C support
      DeviceId |= (p_deviceId & 0x07); // Slave address format is: 1 0 1 0 A3 A2 A1 R/W
      if ((_fd = ::wiringBbbI2CSetup(mcp24lc_i2c::DeviceId)) == -1) {
	std::cerr << "Failed to initialize address 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)mcp24lc_i2c::DeviceId << " (24LCxx)" << std::endl;
	throw beagleboneHwEx("mcp24lc_i2c: wiringPiI2CSetup failure");
      }
      std::clog << "mcp24lc_i2c::mcp24lc_i2c: Device address=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)mcp24lc_i2c::DeviceId << " - fd=" << std::dec << _fd << std::endl;

      _family = p_family;
      // TODO Setup page, bytes per page and MAX_BYTES to write in one chrunk

      // TODO Declare wiringPi node

      // TODO _i2cInstance->frequency(p_frequency); // Set the frequency of the I2C interface

      // Setup WriteProtect
      _wp = p_wp;
      if (p_wp != 0xff) {
	::pinMode(_wp, OUTPUT);
	::digitalWrite(_wp, HIGH); // Protect memory access
      }
    } // End of Constructor

    mcp24lc_i2c::~mcp24lc_i2c() {
      if(_wp != 0xff) { // WP pin was used
	::pinMode(_wp, INPUT);
      }

      if (_fd != -1) {
	close(_fd);
      }
    } // End of Destructor
    
    bool mcp24lc_i2c::writeProtect(const bool p_writeProtect) {
      if (_wp != 0xff) {
	::digitalWrite(_wp, (p_writeProtect == true) ? HIGH : LOW);
	return true;
      }
      
      return false;
    } // End of writeProtect

    int mcp24lc_i2c::eraseMemoryArea(const short p_startAddress, const int p_count, const unsigned char p_pattern) {
      std::vector<unsigned char> eraseBuffer(p_count, p_pattern);
      return write(p_startAddress, eraseBuffer, false);
    } // End of eraseMemoryArea

    int mcp24lc_i2c::write(const short p_address, const unsigned char p_byte) {
      // 1.Prepare buffer
      unsigned char i2cBuffer[2 + sizeof(unsigned char)]; // Memory address + one byte of data
      // 1.1. Memory address
      i2cBuffer[0] = (unsigned char)(p_address >> 8);
      i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
      // 1.2. Datas
      i2cBuffer[2] = p_byte;
      // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = 0;
      _i2cMsg[0].len  = 2 + sizeof(unsigned char);
      _i2cMsg[0].buf = (unsigned char *)i2cBuffer;
      
      int result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::write (unsigned char) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      usleep(200);
      
      return result;
    } // End of 'write unsgned char' method
    
    int mcp24lc_i2c::write(const short p_address, const short p_short, const mcp24lc_i2c::Mode p_mode) {
      // 1.Prepare buffer
      unsigned char i2cBuffer[2 + sizeof(short)]; // Memory address + one short (2 bytes)
      // 1.1. Memory address
      i2cBuffer[0] = (unsigned char)(p_address >> 8);
      i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
      // 1.2. Datas
      if (p_mode == BigEndian) {
	i2cBuffer[2] = (unsigned char)(p_short >> 8);
	i2cBuffer[3] = (unsigned char)((unsigned char)p_short & 0xff);
      } else {
	i2cBuffer[2] = (unsigned char)((unsigned char)p_short & 0xff);
	i2cBuffer[3] = (unsigned char)(p_short >> 8);
      }    
      // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = 0;
      _i2cMsg[0].len  = 2 + sizeof(short);
      _i2cMsg[0].buf = (unsigned char *)i2cBuffer;
      
      int result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::write (short) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      usleep(200);

      return result;
    } // End of 'write short' method
    
    int mcp24lc_i2c::write(const short p_address, const int p_int, const mcp24lc_i2c::Mode p_mode) {
      // 1.Prepare buffer
      unsigned char i2cBuffer[2 + sizeof(int)]; // Memory address + one int (4 bytes)
      // 1.1. Memory address
      i2cBuffer[0] = (unsigned char)(p_address >> 8);
      i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
      // 1.2. Datas
      if (p_mode == BigEndian) {
	i2cBuffer[2] = (unsigned char)(p_int >> 24);
	i2cBuffer[3] = (unsigned char)(p_int >> 16);
	i2cBuffer[4] = (unsigned char)(p_int >> 8);
	i2cBuffer[5] = (unsigned char)((unsigned char)p_int & 0xff);
      } else {
	i2cBuffer[2] = (unsigned char)((unsigned char)p_int & 0xff);
	i2cBuffer[3] = (unsigned char)(p_int >> 8);
	i2cBuffer[4] = (unsigned char)(p_int >> 16);
	i2cBuffer[5] = (unsigned char)(p_int >> 24);
      }
      // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = 0;
      _i2cMsg[0].len  = 2 + sizeof(int);
      _i2cMsg[0].buf = (unsigned char *)i2cBuffer;
      
      int result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::write (int) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      usleep(200);

      return result;
    } // End of 'write int' method
    
    int mcp24lc_i2c::write(const short p_address, const std::string & p_string, const bool p_storeLength, const int p_length2write) {
      return write(p_address, p_string.c_str(), p_storeLength, p_length2write);
    } // End of 'write std::string' method
    
    int mcp24lc_i2c::write(const short p_address, const std::vector<unsigned char> & p_datas, const bool p_storeLength, const int p_length2write) {
      int length = (p_length2write == -1) ? p_datas.size() : p_length2write;
      unsigned char array[length];
      std::copy(p_datas.begin(), p_datas.end(), array); // FIXME Use data() method
      return write(p_address, array, p_storeLength, length);
    } // End of 'write std::vector' method
    
    int mcp24lc_i2c::write(const short p_address, const char *p_datas, const bool p_storeLength, const int p_length2write) {
      // 1.Prepare buffer
      int length = (p_length2write == -1) ? strlen(p_datas) : p_length2write;
      if (p_storeLength) {
	length += 4; // Add four bytes for the length as integer
      }
      unsigned char i2cBuffer[2 + length];
      // 1.1. Memory address
      i2cBuffer[0] = (unsigned char)(p_address >> 8);
      i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
      // 1.2. Datas
      if (p_storeLength) {
	// Fill the length
	i2cBuffer[2] = (unsigned char)(length >> 24);
	i2cBuffer[3] = (unsigned char)(length >> 16);
	i2cBuffer[4] = (unsigned char)(length >> 8);
	i2cBuffer[5] = (unsigned char)((unsigned char)length & 0xff);
	for (int i = 0; i < length - 4; i++) {
	  i2cBuffer[6 + i] = *(p_datas + i);
	}
      } else { // The length was not stored
	for (int i = 0; i < length; i++) {
	  i2cBuffer[2 + i] = *(p_datas + i);
	}
      }
      // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = 0;
      _i2cMsg[0].len  = 2 + length;
      _i2cMsg[0].buf = (unsigned char *)i2cBuffer;
      
      int result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::write (char *) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      usleep(200);

      return result;
    } // End of 'write char *' method
    
    int mcp24lc_i2c::write(const short p_address, const unsigned char *p_datas, const bool p_storeLength, const int p_length2write) {
      return write(p_address, (const char *)p_datas, p_storeLength, p_length2write);
    }
    
    int mcp24lc_i2c::read(const short p_address, unsigned char * p_byte) {
      // 1.Prepare buffer
      unsigned char i2cBuffer[2];
      // 1.1. Memory address
      i2cBuffer[0] = (unsigned char)(p_address >> 8);
      i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
      // 2. Prepare I2C start + memory address
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 2;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = 0;
      _i2cMsg[0].len  = 2;
      _i2cMsg[0].buf = (unsigned char *)i2cBuffer; 
      // 3. Prepare read data + I2C stop
      _i2cMsg[1].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[1].flags = I2C_M_RD;
      _i2cMsg[1].len  = 1;
      _i2cMsg[1].buf = p_byte;

      int result =  ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::read (unsigned char) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      std::clog << "mcp24lc_i2c::read (unsigned char): 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)*p_byte << std::endl;
      
      return result;
    } // End of 'read byte' method
    
    int mcp24lc_i2c::read(const short p_address, short *p_short, const mcp24lc_i2c::Mode p_mode) {
      // 1. Clear buffer 
      ioctl(_fd, BLKFLSBUF);
      // 2 .Prepare buffer
      unsigned char i2cBuffer[2];
      // 2.1 Memory address
      i2cBuffer[0] = (unsigned char)(p_address >> 8);
      i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
      std::clog << "Memory address: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)i2cBuffer[0] << " - " << std::setw(2) << (int)i2cBuffer[1] << std::endl;
      // 2.2 Prepare I2C start + memory address
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = 0;
      _i2cMsg[0].len  = 2;
      _i2cMsg[0].buf = (unsigned char *)i2cBuffer; 
      int result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::read (short) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      // 3. Preapre read data + I2C stop
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = I2C_M_RD;
      _i2cMsg[0].len  = sizeof(short);
      _i2cMsg[0].buf = (unsigned char *)p_short; 

      result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::read (short) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      if (p_mode == BigEndian) { // When reading using ioctl, bytes are swapped, so p_mode test is inverted
	*p_short = converter::getInstance().swap(*p_short);
      }    
      std::clog << "mcp24lc_i2c::read (short): 0x" << std::hex << std::setw(4) << std::setfill('0') << (short)*p_short << " - " << std::dec << *p_short << std::endl;

      return result;
    } // End of 'read short' method
    
    int mcp24lc_i2c::read(const short p_address, int *p_int, const mcp24lc_i2c::Mode p_mode) {
      // 1. Clear buffer 
      ioctl(_fd, BLKFLSBUF);
      // 2 .Prepare buffer
      unsigned char i2cBuffer[2];
      // 2.1 Memory address
      i2cBuffer[0] = (unsigned char)(p_address >> 8);
      i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
      std::clog << "Memory address: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)i2cBuffer[0] << " - " << std::setw(2) << (int)i2cBuffer[1] << std::endl;
      // 2.2 Prepare I2C start + memory address
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = 0;
      _i2cMsg[0].len  = 2;
      _i2cMsg[0].buf = (unsigned char *)i2cBuffer; 
      int result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::read (int) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      // 3. Preapre read data + I2C stop
      _i2cMsgRdwr.msgs = _i2cMsg;
      _i2cMsgRdwr.nmsgs = 1;
      _i2cMsg[0].addr = mcp24lc_i2c::DeviceId;
      _i2cMsg[0].flags = I2C_M_RD;
      _i2cMsg[0].len  = sizeof(int);
      _i2cMsg[0].buf = (unsigned char *)p_int; 

      result = ioctl(_fd, I2C_RDWR, &_i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "mcp24lc_i2c::read (int) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      if (p_mode == BigEndian) { // When reading using ioctl, bytes are swapped, so p_mode test is inverted
	*p_int = converter::getInstance().swap(*p_int);
      }    
      std::clog << "mcp24lc_i2c::read (int): 0x" << std::hex << std::setw(4) << std::setfill('0') << (int)*p_int << " - " << std::dec << *p_int << std::endl;

      return result;
    } // End of 'read int' method
    
    int mcp24lc_i2c::read(const short p_address, std::vector<unsigned char> & p_datas, const bool p_readLengthFirst, const int p_length2write) {
      return -1;
    } // End of 'read std::vector' method
    
    int mcp24lc_i2c::read(const short p_address, std::string & p_string, const bool p_readLengthFirst, const int p_length2write) {
      return -1;
    } // End of 'read std::string' method

  } // End of namespace i2c

} // End of namespace beagleboneHw
