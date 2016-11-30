/* mbed simplified access to Microchip 24LCxx Serial EEPROM devices (I2C)
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
#include <iostream>
#include <sstream>

#include "24LCxx_I2C.h"

namespace _24LCXX_I2C {

    unsigned char C24LCXX_I2C::I2CModuleRefCounter = 0;

    C24LCXX_I2C::C24LCXX_I2C(const PinName p_sda, const PinName p_scl, const unsigned char p_address, const PinName p_wp, const unsigned int p_frequency) : _internalId("") {
        DEBUG_ENTER("C24LCXX_I2C")

        if (C24LCXX_I2C::I2CModuleRefCounter != 0) {
            error("C24LCXX_I2C: Wrong params");
        }
#ifdef __DEBUG
        std::ostringstream out(std::ostringstream::out);
        out << "C24LCXX_I2C #" << C24LCXX_I2C::I2CModuleRefCounter;
        _internalId.assign(out.str());
        DEBUG("C24LCXX_I2C: _internalId='%s'", _internalId.c_str())
#endif // __DEBUG
        _i2cInstance = new I2C(p_sda, p_scl);
        C24LCXX_I2C::I2CModuleRefCounter += 1;
        DEBUG_ENTER("C24LCXX_I2C: refCounter=%d", C24LCXX_I2C::I2CModuleRefCounter)

        _slaveAddress = (p_address << 1) | 0xa0; // Slave address format is: 1 0 1 0 A3 A2 A1 R/W
        DEBUG("C24LCXX_I2C: I2C slave adress:  0x%02x", _slaveAddress)
        _i2cInstance->frequency(p_frequency); // Set the frequency of the I2C interface
        if (p_wp != NC) {
            DEBUG("C24LCXX_I2C: WP managed");
            _wp = new DigitalOut(p_wp);
            _wp->write(0); // Disable write protect
        } else {
            DEBUG("C24LCXX_I2C: WP not managed");
            _wp = NULL; // Not used
        }
    
        DEBUG_LEAVE("C24LCXX_I2C")
    }

    C24LCXX_I2C::~C24LCXX_I2C() {
        DEBUG_ENTER("~C24LCXX_I2C")
    
        // Release I2C instance
        DEBUG_ENTER("~C24LCXX_I2C: refCounter=%d", C24LCXX_I2C::I2CModuleRefCounter)
        C24LCXX_I2C::I2CModuleRefCounter -= 1;
        if (C24LCXX_I2C::I2CModuleRefCounter == 0) {
            delete _i2cInstance;
            _i2cInstance = NULL;
        }
        // Release _wp if required
        if (_wp != NULL) {
            _wp->write(0);
            delete _wp;
        }
    
        DEBUG_LEAVE("~C24LCXX_I2C")
    }
    
    bool C24LCXX_I2C::WriteProtect(const bool p_writeProtect) {
        if (_wp != NULL) {
            DEBUG("WP set to: %x", (int)p_writeProtect)
            _wp->write((int)(p_writeProtect));
            return true;
        }
    
        return false;
    }

    bool C24LCXX_I2C::EraseMemoryArea(const short p_startAddress, const int p_count, const unsigned char p_pattern) {
        DEBUG_ENTER("C24LCXX_I2C::EraseMemoryArea: 0x%02x - %d - 0x%02x", p_startAddress, p_count, p_pattern)
    
        std::vector<unsigned char> eraseBuffer(p_count, p_pattern);
        return Write(p_startAddress, eraseBuffer, false);
    }

    bool C24LCXX_I2C::Write(const short p_address, const unsigned char p_byte) {
        DEBUG_ENTER("C24LCXX_I2C::Write (byte): Memory address: 0x%02x - 0x%02x", p_address, p_byte)
    
        // 1.Prepare buffer
        char i2cBuffer[3]; // Memory address + one byte of data
        // 1.1. Memory address
        short address = p_address; // Index start to 0
        i2cBuffer[0] = (unsigned char)(address >> 8);
//        DEBUG("C24LCXX_I2C::Write (byte): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)address & 0xff);
//        DEBUG("C24LCXX_I2C::Write (byte): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
        // 1.2. Datas
        i2cBuffer[2] = p_byte;
//        DEBUG("C24LCXX_I2C::Write (byte): value=0x%02x", i2cBuffer[2])
    
        // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
        int result = _i2cInstance->write(_slaveAddress, i2cBuffer, 3);
        wait(0.02);
    
        DEBUG_LEAVE("C24LCXX_I2C::Write (byte) %x", (bool)(result == 0))
        return (bool)(result == 0);
    }
    
    bool C24LCXX_I2C::Write(const short p_address, const short p_short, const C24LCXX_I2C::Mode p_mode) {
        DEBUG_ENTER("C24LCXX_I2C::Write (short): Memory address:0x%02x, Mode:%d", p_address, p_mode)
    
        // 1.Prepare buffer
        char i2cBuffer[4]; // Memory address + one short (2 bytes)
        // 1.1. Memory address
        short address = p_address; // Index start to 0
        i2cBuffer[0] = (unsigned char)(address >> 8);
//        DEBUG("C24LCXX_I2C::Write (short): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)address & 0xff);
//        DEBUG("C24LCXX_I2C::Write (short): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
        // 1.2. Datas
        if (p_mode == BigEndian) {
            i2cBuffer[2] = (unsigned char)(p_short >> 8);
            i2cBuffer[3] = (unsigned char)((unsigned char)p_short & 0xff);
        } else {
            i2cBuffer[2] = (unsigned char)((unsigned char)p_short & 0xff);
            i2cBuffer[3] = (unsigned char)(p_short >> 8);
        }
//        DEBUG("C24LCXX_I2C::Write (short): value=0x%02x%02x", i2cBuffer[2], i2cBuffer[3])
    
        // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
        int result = _i2cInstance->write(_slaveAddress, i2cBuffer, 4);
        wait(0.02);
    
        DEBUG_LEAVE("C24LCXX_I2C::Write (short) %x", (bool)(result == 0))
        return (bool)(result == 0);
    }
    
    bool C24LCXX_I2C::Write(const short p_address, const int p_int, const C24LCXX_I2C::Mode p_mode) {
        DEBUG_ENTER("C24LCXX_I2C::Write (int): Memory address:0x%02x, Mode:%d", p_address, p_mode)
    
        // 1.Prepare buffer
        char i2cBuffer[6]; // Memory address + one integer (4 bytes)
        // 1.1. Memory address
        short address = p_address; // Index start to 0
        i2cBuffer[0] = (unsigned char)(address >> 8);
//        DEBUG("C24LCXX_I2C::Write (int): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)address & 0xff);
//        DEBUG("C24LCXX_I2C::Write (int): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
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
//        DEBUG("C24LCXX_I2C::Write (int): value=0x%02x%02x%02x%02x", i2cBuffer[2], i2cBuffer[3], i2cBuffer[4], i2cBuffer[5])
    
        // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
        int result = _i2cInstance->write(_slaveAddress, i2cBuffer, 6);
        wait(0.02);
    
        DEBUG_LEAVE("C24LCXX_I2C::Write (int) %x", (bool)(result == 0))
        return (bool)(result == 0);
    }
    
    bool C24LCXX_I2C::Write(const short p_address, const std::string & p_string, const bool p_storeLength, const int p_length2write) {
        DEBUG_ENTER("C24LCXX_I2C::Write (std::string)")
        return Write(p_address, p_string.c_str(), p_storeLength, p_length2write);
    }
    
    bool C24LCXX_I2C::Write(const short p_address, const std::vector<unsigned char> & p_datas, const bool p_storeLength, const int p_length2write) {
        DEBUG_ENTER("C24LCXX_I2C::Write (std::vector)")
    
        int length = (p_length2write == -1) ? p_datas.size() : p_length2write;
        unsigned char array[length];
        std::copy(p_datas.begin(), p_datas.end(), array);
        bool result = Write(p_address, array, p_storeLength, length);
        wait(0.02);
    
        DEBUG_LEAVE("C24LCXX_I2C::Write (std::vector): %d", result)
        return result;
    }
    
    bool C24LCXX_I2C::Write(const short p_address, const char *p_datas, const bool p_storeLength, const int p_length2write) {
        DEBUG_ENTER("C24LCXX_I2C::Write (char *): Memory address: 0x%02x - %x - %d", p_address, p_storeLength, p_length2write)
    
        // 1.Prepare buffer
        int length = (p_length2write == -1) ? strlen(p_datas) : p_length2write;
        if (p_storeLength) {
            length += 4; // Add four bytes for the length as integer
        }
        DEBUG("C24LCXX_I2C::Write (char *): length:%d", length)
        
        char i2cBuffer[2 + length];
        // 1.1. Memory address
        short address = p_address; // Index start to 0
        i2cBuffer[0] = (unsigned char)(address >> 8);
//        DEBUG("C24LCXX_I2C::Write (char *): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)address & 0xff);
//        DEBUG("C24LCXX_I2C::Write (char *): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
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
        int result = _i2cInstance->write(_slaveAddress, i2cBuffer, 2 + length);
        wait(0.02);
    
        DEBUG_LEAVE("C24LCXX_I2C::Write (char *) %x", (bool)(result == 0))
        return (bool)(result == 0);
    }
    
    bool C24LCXX_I2C::Write(const short p_address, const unsigned char *p_datas, const bool p_storeLength, const int p_length2write) {
        DEBUG_ENTER("C24LCXX_I2C::Write (byte *): Memory address: 0x%02x - %x - %d", p_address, p_storeLength, p_length2write)
        return Write(p_address, (const char *)p_datas, p_storeLength, p_length2write);
    }
    
    bool C24LCXX_I2C::Read(const short p_address, unsigned char * p_byte) {
        DEBUG_ENTER("C24LCXX_I2C::Read (byte): Memory address:0x%02x", p_address)
    
        // 1.Prepare buffer
        char i2cBuffer[2];
        // 1.1. Memory address
        i2cBuffer[0] = (unsigned char)(p_address >> 8);
//        DEBUG("C24LCXX_I2C::Read (byte): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
//        DEBUG("C24LCXX_I2C::Read (byte): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
    
        // 2. Send I2C start + memory address
        if (_i2cInstance->write(_slaveAddress, i2cBuffer, 2, true) == 0) {
            wait(0.02);
//            DEBUG("C24LCXX_I2C::Read (byte): Write memory done")
            // 2. Read data + I2C stop
            int result = _i2cInstance->read(_slaveAddress, (char *)p_byte, 1);
            wait(0.02);
    
            DEBUG_LEAVE("C24LCXX_I2C::Read (byte): %x", (bool)(result == 0))
            return (bool)(result == 0);
        }
    
        DEBUG_LEAVE("C24LCXX_I2C::Read (byte) (false)")
        return false;
    }
    
    bool C24LCXX_I2C::Read(const short p_address, short *p_short, const C24LCXX_I2C::Mode p_mode) {
        DEBUG_ENTER("C24LCXX_I2C::Read (short): Memory address:0x%02x, Mode:%d", p_address, p_mode)
    
        // 1.Prepare buffer
        char i2cBuffer[2];
        // 1.1. Memory address
        i2cBuffer[0] = (unsigned char)(p_address >> 8);
//        DEBUG("C24LCXX_I2C::Read (short): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
//        DEBUG("C24LCXX_I2C::Read (short): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
    
        // 2. Send I2C start + memory address
        if (_i2cInstance->write(_slaveAddress, i2cBuffer, 2, true) == 0) {
            wait(0.02);
//            DEBUG("C24LCXX_I2C::Read (short): Write memory done")
            // 2. Read data + I2C stop
            int result = _i2cInstance->read(_slaveAddress, i2cBuffer, 2);
            if (result == 0) {
//                DEBUG("C24LCXX_I2C::Read (short): value: 0x%02x - 0x%02x", i2cBuffer[0], i2cBuffer[1])
                if (p_mode ==  BigEndian) {
                    *p_short = (short)(i2cBuffer[0] << 8 | i2cBuffer[1]);
                } else {
                    *p_short = (short)(i2cBuffer[1] << 8 | i2cBuffer[0]);
                }
        
                DEBUG_LEAVE("C24LCXX_I2C::Read (short): 0x%04x", *p_short)
                return true;
            }
        }
    
        DEBUG_LEAVE("C24LCXX_I2C::Read (short) (false)")
        return false;
    }
    
    bool C24LCXX_I2C::Read(const short p_address, int *p_int, const C24LCXX_I2C::Mode p_mode) {
        DEBUG_ENTER("C24LCXX_I2C::Read (int): Memory address:0x%02x, Mode:%d", p_address, p_mode)
    
        // 1.Prepare buffer
        char i2cBuffer[4];
        // 1.1. Memory address
        i2cBuffer[0] = (unsigned char)(p_address >> 8);
//        DEBUG("C24LCXX_I2C::Read (int): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)p_address & 0xff);
//        DEBUG("C24LCXX_I2C::Read (int): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
    
        // 2. Send I2C start + memory address
        if (_i2cInstance->write(_slaveAddress, i2cBuffer, 2, true) == 0) {
            wait(0.02);
//            DEBUG("C24LCXX_I2C::Read (int): Write memory done")
            // 2. Read data + I2C stop
            int result = _i2cInstance->read(_slaveAddress, i2cBuffer, 4);
            if (result == 0) {
//                DEBUG("C24LCXX_I2C::Read (int): value: 0x%02x - 0x%02x - 0x%02x - 0x%02x", i2cBuffer[0], i2cBuffer[1], i2cBuffer[2], i2cBuffer[3])
                wait(0.02);
                if (p_mode ==  BigEndian) {
                    *p_int = (int)(i2cBuffer[0] << 24 | i2cBuffer[1] << 16 | i2cBuffer[2] << 8 | i2cBuffer[3]);
                } else {
                    *p_int = (int)(i2cBuffer[3] << 24 | i2cBuffer[2] << 16 | i2cBuffer[1] << 8 | i2cBuffer[0]);
                }
    
                DEBUG_LEAVE("C24LCXX_I2C::Read (int): %d", *p_int)
                return true;
            }
    
            DEBUG_LEAVE("C24LCXX_I2C::Read (int):false")
            return false;
        }
    
        DEBUG_LEAVE("C24LCXX_I2C::Read (int) (false)")
        return false;
    }
    
    bool C24LCXX_I2C::Read(const short p_address, std::vector<unsigned char> & p_datas, const bool p_readLengthFirst, const int p_length2read) {
        DEBUG_ENTER("C24LCXX_I2C::Read (vector): Memory address:0x%02x, readLength:%01x, Length:%d", p_address, p_readLengthFirst, p_length2read)
    
        // 1.Prepare buffer
        short address = p_address;
        int length = 0;
        if (p_readLengthFirst) {
            if (!Read(address, &length)) { // Read the length in big endian mode
                DEBUG_LEAVE("C24LCXX_I2C::Read (vector) Failed to read length")
                return false;
            }
            DEBUG("C24LCXX_I2C::Read (vector): length= %d", length)
            if (length == 0) {
                return true;
            }
            address += 4; // Skip the length value 
            length -= 4; // length is the size of (string length + string)
        } else {
            if (p_length2read == -1) {
                length = p_datas.size();
            } else {
                length = p_length2read;
            }
        }
        DEBUG("C24LCXX_I2C::Read (vector): length= %d", length)
    
        // 2. Memory address
        char i2cBuffer[2];
        i2cBuffer[0] = (unsigned char)(address >> 8);
//        DEBUG("C24LCXX_I2C::Read (vector): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)address & 0xff);
//        DEBUG("C24LCXX_I2C::Read (vector): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
    
        // 3. Send I2C start + memory address
        if (_i2cInstance->write(_slaveAddress, i2cBuffer, 2, true) == 0) {
            wait(0.02);
//            DEBUG("C24LCXX_I2C::Read (vector): Write memory done")
            // 4. read data + I2C stop
            unsigned char buffer[length];
            int result = _i2cInstance->read(_slaveAddress, (char *)buffer, length);
            wait(0.02);
            if (result == 0) {
                p_datas.assign(buffer, buffer + length);
    
                DEBUG_LEAVE("C24LCXX_I2C::Read (vector): %x", (bool)(result == 0))
                return (bool)(result == 0);
            }
        }
    
        DEBUG_LEAVE("C24LCXX_I2C::Read (vector) (false)")
        return false;
    }
    
    bool C24LCXX_I2C::Read(const short p_address, std::string & p_string, const bool p_readLengthFirst, const int p_length2read) {
        DEBUG_ENTER("C24LCXX_I2C::Read (string): Memory address:0x%02x, readLength:%01x, Length:%d", p_address, p_readLengthFirst, p_length2read)

/*        std::vector<unsigned char> datas;
        if (Read(p_address, datas, p_readLengthFirst, p_length2write) == true) {
            p_string.assign((char *)datas.begin(), datas.size());
            
            return true;
        }
        
        DEBUG_LEAVE("C24LCXX_I2C::Read (string) (false)")
        return false;
*/        
    
        // 1.Prepare buffer
        short address = p_address;
        int length = -1;
        if (p_readLengthFirst) { // The string was stored with its length
            if (!Read(address, &length)) { // Read the length as integer in big endian mode
                DEBUG_ERROR("C24LCXX_I2C::Read (string): Failed to read length")
                return false;
            }
            wait(0.02);
            DEBUG("C24LCXX_I2C::Read (string): length=%d", length)
            if (length == 0) {
                DEBUG_ERROR("C24LCXX_I2C::Read (string): empty")
                return true;
            }
            address += 4; // Skip the length value size 
            length -= 4; // length is the size of (string length + string)
        } else { // The string length is provided by p_length2write parameter
            if (p_length2read == -1) {
                length = p_string.size();
            } else {
                length = p_length2read;
                p_string.resize(p_length2read);
            }
        }
        DEBUG("C24LCXX_I2C::Read (string): Address=0x%02x - Length=%d", address, length)
    
        // 2. Memory address
        char i2cBuffer[2];
        i2cBuffer[0] = (unsigned char)(address >> 8);
//        DEBUG("C24LCXX_I2C::Read (string): pI2CBuffer[0]: 0x%02x", i2cBuffer[0])
        i2cBuffer[1] = (unsigned char)((unsigned char)address & 0xff);
//        DEBUG("C24LCXX_I2C::Read (string): pI2CBuffer[1]: 0x%02x", i2cBuffer[1])
    
        // 3. Send I2C start + memory address with repeat start
        if (_i2cInstance->write(_slaveAddress, i2cBuffer, 2, true) == 0) {
            wait(0.02);
//            DEBUG("C24LCXX_I2C::Read (string): Write memory done")
            // 4. Read data + I2C stop
            char buffer[length];
            int result = _i2cInstance->read(_slaveAddress, (char *)buffer, length);
            if (result == 0) {
                p_string.assign(buffer, length);
        
                return true;
            }
        }
        
        DEBUG_LEAVE("C24LCXX_I2C::Read (string) (false)")
        return false;
    }

#if defined(__DEBUG)
    void C24LCXX_I2C::DumpMemoryArea(const int p_address, const int p_count) {
        DEBUG_ENTER("C24LCXX_I2C::DumpMemoryArea: %d - %d", p_address, p_count)
    
        DEBUG("C24LCXX_I2C::DumpMemoryArea: Reading datas...");
        std::vector<unsigned char> datas(p_count);
        if (!Read(p_address, datas, false)) { // Read bytes, including the lenght indication, buffer size is not set before the call
            std::cout << "C24LCXX_I2C::DumpMemoryArea: read failed\r" << std::endl;
        } else {
            std::cout << "C24LCXX_I2C::DumpMemoryArea: Read bytes:\r" << std::endl;
            HEXADUMP(&datas[0], p_count);
            std::cout << "\r" << std::endl;
        }
    }
#endif // _DEBUG

} // End of namespace _24LCXX_I2C
