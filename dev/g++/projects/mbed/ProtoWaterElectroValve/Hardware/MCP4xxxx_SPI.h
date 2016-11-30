/* mbed simplified access to Microchip MCP42xxx/MCP41xxx Digital Potentiometer devices (SPI)
 * Copyright (c) 2013-2013 ygarcia, MIT License
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
#pragma once

#include <string>
#include <vector>

#include "AbstractGpioExpender.h"

#include "Debug.h" // Include mbed header + debug primitives. See DebugLibrary

namespace MCP4xxxx_SPI {

    /** This class provides simplified SPI access to a Microchip MCP42xxx/MCP41xxx Digital Potentiometer device. V0.0.0.2
     * The SPI static settings are: 16bits, mode (0,0)
     * This class manages ONLY ONE /CS pin. If more than one SPI devices are used, ALL /CS pins shall be done by the application itself
     *
     * Microchip MCP42xxx/MCP41xxx Serial EEPROM device reference: DS11195C
     * Microchip MCP41xx/MCP42xx Serial EEPROM device reference: DS122060B
     *
     * Note that MCP41xxx has no SO pin, only a SOI input pin
     * Note that MCP42xxx has a SO pin reserved for daisy-chain configuration
     * Note that MCP41xx/MCP42xx increment/decrement operations are not supported due to the 16 bits SPI setting
     * Note that for SPI details, please visit http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
     *
     * @remark This class was validated with Tektronix TDS2014 oscilloscope in 3.3V and in mixte power mode 3.3V for mbed and 5V for the Microchip 24LCxx Serial EEPROM device
     * @author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
     */
    class CMCP4xxxx_SPI { 
        /** Reference counter used to guarentee unicity of the instance of SPI class
         */
        static unsigned char SPIModuleRefCounter;
        
        /** ChipSelect (pin 1) see DS11195C-page 12 Clause 3.4 Chip Select (CS)
         */
        DigitalOut *_cs;
        
        /** Reset state indicator (pin 11), see DS11195C-page 21 Clause 5.5 Reset (RS) Pin Operation
         */
        DigitalOut *_reset;
        
        /** Shutdown state indicator (pin 12) see DS11195C-page 21 5.6 Shutdown (SHDN) Pin Operation
         */
        DigitalOut *_shdn;
        
        /** ChipSelect (pin 1) see DS11195C-page 12 Clause 3.4 Chip Select (CS)
         *  Connected to a GPIO expender
         */
        unsigned char _cs_gpio;
        
        /** Reset state indicator (pin 11), see DS11195C-page 21 Clause 5.5 Reset (RS) Pin Operation
         *  Connected to a GPIO expender
         */
        unsigned char _reset_gpio;
        
        /** Shutdown state indicator (pin 12) see DS11195C-page 21 5.6 Shutdown (SHDN) Pin Operation
         *  Connected to a GPIO expender
         */
        unsigned char _shdn_gpio;
        
        /** Reference to the GPIO expender (e.g. MCP23x17)
         */
        AbstractGpioExpender * _gpioExpender;
        
        /** An unique instance of SPI class
         */
        SPI *_spiInstance;
        /** Increment step value used for inc/dec methods, when inc/dec commands are not supported (such as _4xxxx famillies)
         */
        unsigned char _incStep;
     public:
        /** MCP32xx family
         */
        enum Mcp4xxxFamily {
            _41xxx = 0x00,  //<! 41xxx family such as MCP41010 - See DS11195C
            _42xxx = 0x01,  //<! 42xxx family such as MCP42010 - See DS11195C
            _41xx = 0x02,   //<! 41xx family such as MCP4251 - Single Potentiometer/Rheostat
            _42xx = 0x03    //<! 42x2 family such as MCP42x2 - Dual Potentiometer/Rheostat
       };
        Mcp4xxxFamily _family;     
     public:
        /** Authorized commands
         * See DS11195C-page 18
         */
        enum Commands {
            WriteToDigiPot1,    //<! Write to digital potentiometer #1
            WriteToDigiPot2,    //<! Write to digital potentiometer #2
            WriteToBoth,        //<! Write to both digital potentiometers
            ShutdownDigiPot1,   //<! Shutdown digital potentiometer #1
            ShutdownDigiPot2,   //<! Shutdown digital potentiometer #2
            ShutdownBoth,       //<! Shutdown both digital potentiometers
        };
        /** Register adresses
         * See DS22060B-page 33 Clause 4.0 FUNCTIONAL OVERVIEW 
         */
        enum Addresses {
            TCon = 0x04,
            Status = 0x05
        };
   public:
        /** Constructor with Write Protect command pin wired.
         *
         * @param p_mosi: MBed pin for SDI
         * @param p_miso: MBed pin for SDO. Note that this pin does not exist for MCP41xxx
         * @param p_sclk: MBed pin for CLK
         * @param p_cs  : MBed pin for Chip Select. If NC, assumes that application manage /CS, default value is NC, not connected
         * @param p_reset: MBed pin to manage /RESET input. If NC, /RESET is not managed, default value is NC, not connected
         * @param p_shdn: MBed pin to manage /SHDN input. If NC, /SHDN is not managed, default value is NC, not connected
         * @param p_familly: MCP4xxx familly. Default: _41xxx
         * @param p_frequency: Frequency of the SPI interface (SCK), default value is 1MHz
         */
        CMCP4xxxx_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const PinName p_cs = NC, const PinName p_reset = NC, const PinName p_shdn = NC, const Mcp4xxxFamily family = _41xxx, const unsigned int p_frequency = 1000000);
    
        /** Constructor with Write Protect command pin wired.
         *
         * @param p_mosi: MBed pin for SDI
         * @param p_miso: MBed pin for SDO. Note that this pin does not exist for MCP41xxx
         * @param p_sclk: MBed pin for CLK
         * @param p_cs_gpio  : GPIO expender pin for Chip Select. If 0xFF, assumes that application manage /CS, default value is 0xFF, not connected
         * @param p_reset_gpio: GPIO expender pin to manage /RESET input. If 0xFF, /RESET is not managed, default value is 0xFF, not connected
         * @param p_shdn_gpio: GPIO expender pin to manage /SHDN input. If 0xFF, /SHDN is not managed, default value is 0xFF, not connected
         * @param p_gpioExpender : Reference to the GPIO expender
         * @param p_familly: MCP4xxx familly. Default: _41xxx
         * @param p_frequency: Frequency of the SPI interface (SCK), default value is 1MHz
         */
        CMCP4xxxx_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const unsigned char p_cs_gpio = 0xff, const unsigned char p_reset_gpio = 0xff, const unsigned char p_shdn_gpio = 0xff, AbstractGpioExpender * p_gpioExpender = NULL, const Mcp4xxxFamily family = _41xxx, const unsigned int p_frequency = 1000000);
    
        /** Destructor
         */
        virtual ~CMCP4xxxx_SPI();

        /** Used to return the unique instance of SPI instance
         */
        inline const SPI * operator * () { return (const SPI *)_spiInstance; };

        /** Write a value to the specified register. Only for _41xx/_42xx famillies
         * @param p_address The register to be written
         * @param p_value The value to write
         * @return 0x0000 on success, any value otherwise
         */
        unsigned short WriteRegister(const Addresses p_address, const unsigned char p_value);
        
        /** Read the content of the specified register. Only for _41xx/_42xx famillies
         * @param p_address The register to be written
         * @return The register value 
         */
        unsigned short ReadRegister(const Addresses p_address);
        
        /** Increment the specified digital potentiometer
         * @param p_command The digital potentiometer to increment. Default: WriteToDigiPot1
         * @return 0x0000 on success, any value otherwise
         */
        unsigned short Increment(const Commands p_command = WriteToDigiPot1);
        
        /** Decrement the specified digital potentiometer
         * @param p_command The digital potentiometer to decrement. Default: WriteToDigiPot1
         * @return 0x0000 on success, any value otherwise
         */
        unsigned short Decrement(const Commands p_command = WriteToDigiPot1);

        /** Send a write a command (WriteToPot1, WriteToPot2 or WriteBoth)
         *
         * @param p_command The command to execute (See DS11195C-page 18)
         * @param p_value The potentiometer selection bits (See DS11195C-page 14 Clause 4.1 Modes of Operation)
         * @return 0x0000 on success, 0Xffff otherwise
         * Exemple:
         * @code
         * unsigned char potLevel;
         * ...
         * g_chipSelect.write(0);
         * g_digitalPot.Write(CMCP4xxxx_SPI::WriteToPot2, potLevel);
         * g_chipSelect.write(1);
         * ...
         * @endcode
         */
        unsigned short Write(const Commands p_command, const unsigned char p_value);
    
        /** Send a shutdown a command (ShutdownPot1, ShutdownPot2 or ShutdownBoth)
         *
         * @param p_command The command to execute (See DS11195C-page 18)
         * @param p_set Set to true to shutdown, false to set up. Only used for _41xx/_42xx famillies (See DS22060B-page 36 REGISTER 4-2: TCON BITS)
         * @return 0x0000 on success, 0Xffff otherwise
         * Exemple:
         * @code
         * ...
         * g_chipSelect.write(0);
         * g_digitalPot.Shutdown(CMCP4xxxx_SPI::ShutdownPot1);
         * g_chipSelect.write(1);
         * ...
         * @endcode
         */
        unsigned short Shutdown(const Commands p_command, const bool p_set = true);
    
         /** Write a NOP command
          */
        //unsigned short Write();
    
         /** Change the current family 
          * @param family The new type of ICs 
          * @return the previous type of ICs 
          */
        Mcp4xxxFamily SetFamilly(const Mcp4xxxFamily family);
    
       /** Reset the device
        * @code
        * unsigned char potLevel;
        * ...
        * g_digitalPot.Reset();
        * ...
        * @endcode
        */
        void Reset();
    
        /** Shutdown the device
         */
        void Shutdown(const bool p_set);
    
    private:
        /** Internal reference identifier. Only used when /RS in is available
         *
         * @param p_set Set to true to shutdown, false to set up
         */
        std::string _internalId;

        inline void enableDevice() {
            if (_cs != NULL) {
                _cs->write(0);
            } else if (_gpioExpender != NULL) {
                _gpioExpender->write(_cs_gpio, 0);
            }
        };
           
        inline void disableDevice() {
            if (_cs != NULL) {
                _cs->write(1);
            } else if (_gpioExpender != NULL) {
                _gpioExpender->write(_cs_gpio, 1);
            }
        };
        
    }; // End of class CMCP4xxxx_SPI

} // End of namespace MCP4xxxx_SPI

using namespace MCP4xxxx_SPI;
