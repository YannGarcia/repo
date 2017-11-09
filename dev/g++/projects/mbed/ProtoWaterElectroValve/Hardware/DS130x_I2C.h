/* mbed simplified access to MAXIM DS130x Serial, I2C Real-Time Clock
 * Copyright (c) 2011 ygarcia
 *
 * of this software and associated documentation files (the "Software"), to deal
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <string>
#include <vector>

#include "Debug.h"

namespace DS130X_I2C {
    /** This class provides simplified I2C access to a MAXIM DS130x Real-Time Clock device. V0.0.0.2
     *
     * A typical use case should be the Mbed which acts as a time server with an ethernet connection, it synchronyzes a RTC circuit for all other module (Microchip/ATiny MCUs).
     *
     * Note that if the LPC1768 is powered in 3.3V and MAXIM DS130x Real-Time Clock device should be powered at 5V.
     * In this case, you shall use a bi-directional level shifter for I2C-bus. Please refer to AN97055 (http://ics.nxp.com/support/documents/interface/pdf/an97055.pdf)
     * MAXIM DS130x Real-Time Clock device reference: http://pdfserv.maxim-ic.com/en/ds/DS1307.pdf
     *
     * Note that for I2C details, please visit http://www.datelec.fr/fiches/I2C.htm
     *
     * Note that this header file include following headers:
     * - <string>
     * - <vector>
     * - <mbed.h>
     *
     * \remark This class was validated with Tektronix TDS2014 oscilloscope in 3.3V and in mixte power mode 3.3V for mbed and 5V for the MAXIM DS130x Real-Time Clock device
     * \author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
     */
    class CDS130X_I2C : public I2C {

    public: // Enumerated
        /** OscillatorMode modes
         */
        enum OscillatorMode {
            One_Hz, //<! Oscillator set for 1Hz square signal generation
            Four_KHz, //<! Oscillator set for 4096Hz square signal generation
            Height_KHz, //<! Oscillator set for 8192Hz square signal generation
            ThirtyTwo_KHz, //<! Oscillator set for 32768Hz square signal generation
            Output //<! Oscillator is not used, \see _outputControlLevel for logocal outpout level
        };

        /** Time register format
         */
        enum RegisterFormatEnum {
            Binary, //<! Time register format is binary
            Bcd //<! Time register format is BCD
        };

        /** Memory storage mode
         */
        enum Mode {
            LittleEndian, //<! Little Endian mode: 0xA0B70708 is stored as 08: MSB and A0 LSB
            BigEndian //<! Little Endian mode: 0xA0B70708 is stored as AO: MSB and 08 LSB
        };

        /** Registers address
         */
        enum RegisterEnum {
            SecondsAddress = 0x00, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            MinutesAddress = 0x01, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            HoursAddress = 0x02, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            DayOfWeekAddress = 0x03, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            DayAddress = 0x04, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            MonthAddress = 0x05, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            YearAddress = 0x06, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            ControlRegisterAddress = 0x07, //<! The register addresses for DS1307 - See Datasheet - Table 2. Timekeeper Registers
            BaseMemoryAddress = 0x08 //<! First address of the memory area
        };

    private: // Internal data structures

        /** String used to convert day of week string into numerical value
         */
        std::string _dayOfWeek;
        
        /** This method controls the clock halting or starting it
         *
         * See datasheet - Clause CLOCK AND CALENDAR (CH bit)
         *
         * \param p_mode: true to restart the clock, false to halt it
         * \return true on success, false otherwise
         */
        bool ControlClock(bool p_mode);

        /** The slave address byte for DS130x - See Datasheet - Figure 4. Data Write-Slave Receiver Mode
         */
        unsigned char _slaveAddress;
        /** Enable/disable the oscillator output
         */
        CDS130X_I2C::OscillatorMode _oscillatorMode;
        /** This flag controls the output level of the SQW/OUT pin when the square-wave output is disabled
         * Set to true if OUT level should be logic level 1, false otherwise
         */
        bool _outputLevel;

    public: // Construction methods
        /** Ctor with Write Protect command pin wired
         *
         * \param p_slaveAddress: I2C device address
         * \param p_sda: MBed pin for SDA
         * \param p_scl: MBed pin for SCL
         * \param p_oscillatorMode Indicate the oscillator mode (pin 7 - SQW/OUT). Default: Output (oscillator not used)
         * \param p_outputLevel Indicate the output level (0V/Vdd) when oscillator mode is Output. Default: 0V
         * \param p_frequency: Frequency of the I2C interface (SCL), default value is 100KHz - See datasheet - Clause I2C DATA BUS
         */
        CDS130X_I2C(const unsigned char p_slaveAddress, const PinName p_sda, const PinName p_scl, const CDS130X_I2C::OscillatorMode p_oscillatorMode = Output, const bool p_outputLevel = false, const int p_frequency = 400000);
    
        /** Dtor
         */
        virtual ~CDS130X_I2C();

        /** Initialize the module, configuring the module and starting the clock
         *
         * \return true on success, false otherwise
         */
        bool Initialize();

    public: // Time part
        /** This methods converts a packed BCD value < 99 into an hexadecimal value (e.g. 0x32 -> 0x10)
         *
         * \param p_hexaValue: The hexadecimal value to convert
         * \return The packed BCD value
         */
        inline unsigned char ConvertBCDToHex(const unsigned char p_bcdValue) {
            /*DEBUG("ConvertBCDToHex: %02x - %02x - %02x - %02x - %d", 
                p_bcdValue, 
                (unsigned char)(p_bcdValue >> 4), 
                (unsigned char)((unsigned char)(p_bcdValue >> 4) * 10), 
                (unsigned char)(p_bcdValue & 0x0f), 
                (int)(
                    (unsigned char)((unsigned char)(p_bcdValue >> 4) * 10) | (unsigned char)(p_bcdValue & 0x0f)
                 )
            )*/
            return (unsigned char)((unsigned char)(p_bcdValue >> 4) * 10) | (unsigned char)(p_bcdValue & 0x0f);
        }

        /** This methods converts an hexadecimal value < 99 into a packed BCD (e.g. 0x20 -> 0x32)
         *
         * \param p_hexaValue: The hexadecimal value to convert
         * \return The packed BCD value
         */
        inline unsigned char ConvertHexToBCD(const unsigned char p_hexaValue) {
            //DEBUG("ConvertHexToBCD: %02x - %02x - %02x", p_hexaValue, (unsigned char)(p_hexaValue / 10 << 4), (unsigned char)(p_hexaValue % 10))
            return (unsigned char)(p_hexaValue / 10 << 4) | (unsigned char)(p_hexaValue % 10);
        }

        /** Restart the clock
         *
         * See datasheet - Clause CLOCK AND CALENDAR (CH bit)
         *
         * \return true on success, false otherwise
         */
        bool RestartClock();
        /** Halt the clock
         *
         * See datasheet - Clause CLOCK AND CALENDAR (CH bit)
         *
         * \return true on success, false otherwise
         */
        bool HaltClock();

        /** This method reads the DS130x time registers value in BCD or binary format
         *
         * See datasheet - Clause CLOCK AND CALENDAR
         *
         * \param p_address: The time register identifier the to read
         * \param p_byte: The register value in BDC format
         * \param p_format: The format of the value to return. Default is BCD
         * \return true on success, false otherwise
         * \see RegisterEnum
         * \see RegisterFormatEnum
         */
        bool Read(const RegisterEnum p_address, unsigned char * p_byte, const CDS130X_I2C::RegisterFormatEnum p_format = Bcd);

        /** This method writes a value (provided in BCD or binary format) into the specified DS130x register
         *
         * See datasheet - Clause CLOCK AND CALENDAR
         *
         * \param p_address: The time register identifier the to write
         * \param p_byte: The value to write in BCD format
         * \param p_format: The format of the value 'p_byte'. Default is BCD
         * \return true on success, false otherwise
         * \see RegisterEnum
         * \see RegisterFormatEnum
         */
        bool Write(const RegisterEnum p_address, const unsigned char p_byte, const CDS130X_I2C::RegisterFormatEnum p_format = Bcd);

        /** Set RTC time using string format "Www Mmm dd hh:mm:ss yyyy", e.g. Thu 02 10 07:13:29 2011
         *
         * \param p_utcTime: UTC string format
         * \return true on success, false otherwise
         */
        bool SetTime(const std::string p_utcTime);

        /** This methods returns the current time in string format "Www Mmm dd hh:mm:ss yyyy"
         *
         * \return The current time in C struct tm format
         */
        struct tm GetTime();

    public: // Memory part
        /** Erase of memory area starting at the specified address, using the specified pattern to fill the memory area
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_count The size of the memory area to erase
         * \param p_pattern The pattern value to use to fill the memory area. Defqult vqlue: 0x00
         * \return true on success, false otherwise
         * Exemple:
         * @code
         * ...
         * myRTC.EraseMemoryArea(0, 8); // Set to 0x00 the first heigh memory byte
         * ...
         * @endcode
         */
        bool EraseMemoryArea(const unsigned char p_startAddress, const int p_count, unsigned char const p_pattern = 0x00);

        /** Write a byte at the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_byte The byte value to save. The address start from 0.
         * \return true on success, false otherwise
         */
        bool WriteMemory(const unsigned char p_address, const unsigned char p_byte);
    
        /** Write a short at the specified memory address according to the specified mode
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_short The short value to save
         * \param p_mode The storage mode. Default value: BigEndian
         * \return true on success, false otherwise
         */
        bool WriteMemory(const unsigned char p_address, const short p_short, const CDS130X_I2C::Mode p_mode = BigEndian);
    
        /** Write an integer at the specified memory address according to the specified mode
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_int The integer value to save
         * \param p_mode The storage mode. Default value: BigEndian
         * \return true on success, false otherwise
         */
        bool WriteMemory(const unsigned char p_address, const int p_int, const CDS130X_I2C::Mode p_mode = BigEndian);
    
        /** Write a buffer of bytes at the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_datas The string to save
         * \param p_storeLength If true, store also the length of the buffer in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
         * \param p_length2write The number of bytes to write, -1 for all characters. Default value: -1
         * \return true on success, false otherwise
         */
        bool WriteMemory(const unsigned char p_address, const std::vector<unsigned char> & p_datas, bool p_storeLength = true, const int p_length2write = -1);
    
        /** Write a buffer of bytes at the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_datas The buffer of bytes to save
         * \param p_storeLength If true, store also the length of the buffer in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
         * \param p_length2write The number of bytes to write, -1 for all bytes. Default value: -1
         * \return true on success, false otherwise
         */
        bool WriteMemory(const unsigned char p_address, const unsigned char *p_datas, bool p_storeLength = true, const int p_length2write = -1);
    
        /** Write a string at the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_string The string to save
         * \param p_storeLength If true, store also the length of the string in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
         * \param p_length2write The number of character to write, -1 for all characters
         * \return true on success, false otherwise
         */
        bool WriteMemory(const unsigned char p_address, const std::string & p_string, const bool p_storeLength = true, const int p_length2write = -1);
    
        /** Write a buffer of characters at the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * Note that the length of the buffer is not saved and the string is saved in Big Endian mode
         * \param p_startAddress The address of the memory area.
         * \param p_datas The string to save
         * \param p_storeLength If true, store also the length of the string in Big Endian mode, otherwise the length will be provided by p_length2write parameter. Default value: true.
         * \param length2write The number of character to write, -1 for all characters
         * \return true on success, false otherwise
         */
        bool WriteMemory(const unsigned char p_address, const char *p_datas, const bool p_storeLength = true, const int p_length2write = -1);
    
        /** Read a byte from the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_byte The byte value to read
         * \return true on success, false otherwise
         */
        bool ReadMemory(const unsigned char p_address, unsigned char *p_value);
    
        /** Read a short from the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_short The short value to read
         * \return true on success, false otherwise
         */
        bool ReadMemory(const unsigned char p_address, short *p_short, const CDS130X_I2C::Mode p_mode = BigEndian);
    
        /** Read an integer from the specified memory address
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_int The integer value to read
         * \return true on success, false otherwise
         */
        bool ReadMemory(const unsigned char p_address, int *p_int, const CDS130X_I2C::Mode p_mode = BigEndian);
    
        /** Read a buffer of bytes from the specified memory address and store it into a std::vector<unsigned char> object
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that the size of the buffer object is used for the number of bytes to read
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_datas The buffer to fill
         * \param p_readLengthFirst If true, read the length first and p_length2write parameter is ignored, otherwise the length is provided by p_length2write parameter. Default value: true
         * \param p_length2write The number of character to write, -1 to use the size of the string buffer
         * \return true on success, false otherwise
         * Exemple:
         * @code
         * std::vector<unsigned char> datas(bufferLength);
         * ...
         * myEEPROM.Read(memoryAddress, datas);
         * ...
         * @endcode
         */
        bool ReadMemory(const unsigned char p_address, std::vector<unsigned char> & p_datas, const bool p_readLengthFirst = true, const int p_length2write = -1);
    
        /** Read a buffer of characters from the specified memory address and store it into a string object
         *
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that the size of the string object is used for the number of characters to read
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_string The string buffer to fill
         * \param p_readLengthFirst If true, read the length first and p_length2write parameter is ignored, otherwise the length is provided by p_length2write parameter. Default value: true
         * \param p_length2write The number of character to write, -1 to use the size of the string buffer
         * \return true on success, false otherwise
         */
        bool ReadMemory(const unsigned char p_address, std::string & p_string, const bool p_readLengthFirst = true, const int p_length2write = -1);
    
#if defined(__DEBUG)
        /** Dump a memory area
         * 
         * Note that for the DS1307, the memory segment is [08h, 3Fh]
         * Note that the size of the string object is used for the number of characters to read
         * Note that this method only access the memeory registered. The memory address starts from 0x00
         * \param p_startAddress The address of the memory area.
         * \param p_count The number of bytes toi dump
         * \return true on success, false otherwise
         */
        void DumpMemoryArea(const unsigned char p_address, const int p_count);
#endif // _DEBUG
    }; // End of class CDS130X_I2C

} // End of namespace DS130X_I2C

using namespace DS130X_I2C;
