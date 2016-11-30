/* Raspberry PI simplified access to Microchip 24LCxx Serial EEPROM devices (I2C)
 * Copyright (c) 2010-2012 ygarcia, MIT License
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
#if !defined(__MCP24LC_I2C_H__)
#define __MCP24LC_I2C_H__

#include <string>
#include <vector>

#include <sys/ioctl.h>
#include <linux/i2c.h> // Use direct I2C driver
#include <linux/i2c-dev.h>

namespace beagleboneHw {

  namespace i2c {

    class mcp24lc_i2c {
 
      static unsigned char DeviceId;
      struct i2c_rdwr_ioctl_data _i2cMsgRdwr;
      struct i2c_msg _i2cMsg[2];

      int _fd;
      unsigned char _wp;

    public:
      /** Memory storage mode
       */
      enum Mode {
	LittleEndian, //<! Little Endian mode: 0xA0B70708 is stored as 08: MSB and A0 LSB
	BigEndian //<! Little Endian mode: 0xA0B70708 is stored as AO: MSB and 08 LSB
      };

      /** 24LC memory families
       */
      enum Families {
	_24lc256 //<! 256K I2C. CMOS Serial EEPROM - See DS21203M
      };

    public:
      /** Constructor with Write Protect command pin wired. Use it to manage the first I2C module on 3.3V or 5V network
       *
       * @param p_deviceId: Device identifier input: A0, A1, A2 (Pins <1,3>)
       * @param p_wp: Pin to manage Write Protect input. If 0xff (NC), WP is not managed, default value is NC, not connected
       * @param p_family: 24LC memory families
       * @param p_frequency: Frequency of the I2C interface (SCL), default value is 400KHz
       * Example:
       * - If A1 and A2 pins are tired to Vdd and A0 is tired to Vss, address shall '00000110'B
       * - If A0 and A1 pins are tired to Vss and A2 is tired to Vdd, address shall '00000100'B
       */
      mcp24lc_i2c(const unsigned char p_deviceId, const unsigned char p_wp = 0xff, const mcp24lc_i2c::Families p_family = mcp24lc_i2c::_24lc256, const unsigned int p_frequency = 400000);    
      /** Destructor
       */
      virtual ~mcp24lc_i2c();

      /** Activate or deactivate write protect (pin 7)
       *
       * Note that a voltage of 3.3V apply to WP input of 24LCxx device is enough to enable write protect
       * @param p_writeProtect: Set to true to activate write protection, false otherwise
       * @return true on success, false otherwise
       */
      bool writeProtect(const bool p_writeProtect);
      
      /** Erase of memory area starting at the specified address, using the specified pattern to fill the memory area
       *
       * @param p_startAddress The address of the memory area (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_count The size of the memory area to erase
       * @param p_pattern The pattern value to use to fill the memory area. Default vqlue: 0x00
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * ...
       * myEEPROM.EraseMemoryArea(0, 1024); // Set to 0x00 the first 1Kb memory 
       * ...
       * @endcode
       */
      int eraseMemoryArea(const short p_startAddress, const int p_count, const unsigned char p_pattern = 0x00);
    
      /** Write a byte at the specified memory address
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_byte The byte value to save
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * unsigned char value = 0xaa;
       * ...
       * myEEPROM.write(memoryAddress, value);
       * ...
       * @endcode
       */
      int write(const short p_address, const unsigned char p_byte);
    
      /** Write a short at the specified memory address according to the specified mode
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_short The short value to save
       * @param p_mode The storage mode. Default value: BigEndian
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * short value = 0xcafe;
       * ...
       * myEEPROM.write(memoryAddress, value, LittleEndian);
       * ...
       * @endcode
       */
      int write(const short p_address, const short p_short, const mcp24lc_i2c::Mode p_mode = BigEndian);
    
      /** Write an integer at the specified memory address according to the specified mode
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_int The integer value to save
       * @param p_mode The storage mode. Default value: BigEndian
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * int value = 0xcafedeca;
       * ...
       * myEEPROM.write(memoryAddress, value, LittleEndian);
       * ...
       * @endcode
       */
      int write(const short p_address, const int p_int, const mcp24lc_i2c::Mode p_mode = BigEndian);
    
      /** Write a buffer of bytes at the specified memory address
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_datas The string to save
       * @param p_storeLength If true, store also the length of the buffer in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
       * @param p_length2write The number of bytes to write, -1 for all characters. Default value: -1
       * @return true on success, false otherwise
       */
      int write(const short p_address, const std::vector<unsigned char> & p_datas, bool p_storeLength = true, const int p_length2write = -1);
    
      /** Write a buffer of bytes at the specified memory address
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_datas The buffer of bytes to save
       * @param p_storeLength If true, store also the length of the buffer in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
       * @param p_length2write The number of bytes to write, -1 for all bytes. Default value: -1
       * @return true on success, false otherwise
       */
      int write(const short p_address, const unsigned char *p_datas, bool p_storeLength = true, const int p_length2write = -1);
    
      /** Write a string at the specified memory address
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_string The string to save
       * @param p_storeLength If true, store also the length of the string in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
       * @param p_length2write The number of character to write, -1 for all characters
       * @return true on success, false otherwise
       * Exemple:
       * @code
       * std::string text2save("CafeDeca");
       * ...
       * myEEPROM.write(memoryAddress, text2save);
       * ...
       * @endcode
       */
      int write(const short p_address, const std::string & p_string, const bool p_storeLength = true, const int p_length2write = -1);
    
      /** Write a buffer of characters at the specified memory address (from 0 to N - 1, N is the number of cells of the memory)
       *
       * Note that the length of the buffer is not saved and the string is saved in Big Endian mode
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_datas The string to save
       * @param p_storeLength If true, store also the length of the string in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
       * @param length2write The number of character to write, -1 for all characters
       * @return true on success, false otherwise
       */
      int write(const short p_address, const char *p_datas, const bool p_storeLength = true, const int p_length2write = -1);
    
      /** Read a byte from the specified memory address
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_byte The byte value to read
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * unsigned char value;
       * ...
       * myEEPROM.read(memoryAddress, (unsigned char *)&value);
       * ...
       * @endcode
       */
      int read(const short p_address, unsigned char *p_byte);
    
      /** Read a short from the specified memory address
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_short The short value to read
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * short value;
       * ...
       * myEEPROM.read(memoryAddress, (short *)&value);
       * ...
       * @endcode
       */
      int read(const short p_address, short *p_short, mcp24lc_i2c::Mode p_mode = BigEndian);
    
      /** Read an integer from the specified memory address
       *
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_int The integer value to read
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * int value;
       * ...
       * myEEPROM.read(memoryAddress, (int *)&value);
       * ...
       * @endcode
       */
      int read(const short p_address, int *p_int, mcp24lc_i2c::Mode p_mode = BigEndian);
    
      /** Read a buffer of bytes from the specified memory address and store it into a std::vector<unsigned char> object
       *
       * Note that the size of the buffer object is used for the number of bytes to read
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_datas The buffer to fill
       * @param p_readLengthFirst If true, read the length first and p_length2write parameter is ignored, otherwise the length is provided by p_length2write parameter. Default value: true
       * @param p_length2read The number of character to write, -1 to use the size of the string buffer
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * std::vector<unsigned char> datas(bufferLength);
       * ...
       * myEEPROM.read(memoryAddress, datas);
       * ...
       * @endcode
       */
      int read(const short p_address, std::vector<unsigned char> & p_datas, bool p_readLengthFirst = true, int p_length2read = -1);
    
      /** Read a buffer of characters from the specified memory address and store it into a string object
       *
       * Note that the size of the string object is used for the number of characters to read
       * @param p_address The memory address (from 0 to N - 1, N is the number of cells of the memory)
       * @param p_string The string buffer to fill
       * @param p_readLengthFirst If true, read the length first and p_length2write parameter is ignored, otherwise the length is provided by p_length2write parameter. Default value: true
       * @param p_length2write The number of character to write, -1 to use the size of the string buffer
       * @return 0 on success, -1 otherwise
       * Exemple:
       * @code
       * std::string readtext;
       * ...
       * myEEPROM.read(memoryAddress, readtext);
       * ...
       * @endcode
       */
      int read(const short p_address, std::string & p_string, bool p_readLengthFirst = true, int p_length2write = -1);

    private:
      mcp24lc_i2c::Families _family;
    }; // End of class mcp24lc_i2c 
    
  } // End of namespace i2c

} // End of namespace beagleboneHw

using namespace beagleboneHw::i2c;

#endif //  __MCP24LC_I2C_H__
