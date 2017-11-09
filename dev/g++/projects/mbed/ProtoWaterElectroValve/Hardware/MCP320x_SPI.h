/* mbed simplified access to Microchip MCP320x 12 bits ADC devices (SPI)
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

namespace MCP320x_SPI {

    /** This class provides simplified SPI access to a Microchip MCP320x 12-Bit A/D Converter with SPI Serial Interface device. V0.0.0.1
     *
     * Microchip MCP42xxx/MCP41xxx Serial EEPROM device reference: DS11195C
     *
     * Note that MCP3201 has no SI pin, only a SO output pin
     * Note that for SPI details, please visit http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
     *
     * \remark This class was validated with Tektronix TDS2014 oscilloscope in 3.3V
     * \author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
     */
    class CMCP320x_SPI { 
        /** Reference counter used to guarentee unicity of the instance of SPI class
         */
        static unsigned char SPIModuleRefCounter;
        
        /** ChipSelect (pin 1) see DS21290F-page 15 Clause 3.3 Chip Select/Shutdown (CS/SHDN)
         */
        DigitalOut *_cs;
        
        /** ChipSelect (pin 1) see DS21290F-page 15 Clause 3.3 Chip Select/Shutdown (CS/SHDN)
         *  Connected to a GPIO expender
         */
        unsigned char _cs_gpio;

        /** An unique instance of SPI class
         */
        SPI *_spiInstance;
        
        /** Reference to the GPIO expender (e.g. MCP23x17)
         */
        AbstractGpioExpender * _gpioExpender;
        
        /** ADC sample structure
         */
        typedef union {
            unsigned int value;
            struct {
                unsigned char bytes[2];
            };
        } ADCValue;
        ADCValue _sample;
        /** Number of channels according to the IC type 
         */
        unsigned char _channelsNum;
        /** Set to true for single-ended inputs configuration, false for pseudo-differential inputs
         * \see DS21298E-page 19 Clause 5.0 SERIAL COMMUNICATIONS
         */
        unsigned char _settings;
   public:
        /** MCP320x familly
         */
        enum Mcp320xFamilly {
            _3201 = 0x00, /** See DS21290F */
            _3204 = 0x01, /** See DS21298E */
            _3208 = 0x03  /** See DS21298E */
        };
        Mcp320xFamilly _familly;
        /** MCP320x channels to read
         */
        enum Mcp320xChannels {
            CH0 = 0x00, /** See DS21290F/DS21290F */
            CH1 = 0x01, /** See DS21298E */
            CH2 = 0x02, /** See DS21298E */
            CH3 = 0x03, /** See DS21298E */
            CH4 = 0x04, /** See DS21298E */
            CH5 = 0x05, /** See DS21298E */
            CH6 = 0x06, /** See DS21298E */
            CH7 = 0x07  /** See DS21298E */
        };
   public:
        /** Constructor with Write Protect command pin wired.
         *
         * \param p_mosi: MBed pin for SDI
         * \param p_miso: MBed pin for SDO
         * \param p_sclk: MBed pin for CLK
         * \param p_cs  : MBed pin for Chip Select. If NC, assumes that application manage /CS, default value is NC, not connected
         * \param p_familly: MCP320x familly. Default: _3201
         * \param p_frequency: Frequency of the SPI interface (SCK), default value is 1MHz
         */
        CMCP320x_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const PinName p_cs = NC, const Mcp320xFamilly p_familly = _3201, const unsigned int p_frequency = 1000000);

        /** Constructor with Write Protect command pin wired.
         *
         * \param p_mosi: MBed pin for SDI
         * \param p_miso: MBed pin for SDO
         * \param p_sclk: MBed pin for CLK
         * \param p_cs_gpio: GPIO expender pin for Chip Select
         * \param p_gpioExpender : Reference to the GPIO expender
         * \param p_familly: MCP320x familly. Default: _3201
         * \param p_frequency: Frequency of the SPI interface (SCK), default value is 1MHz
         */
        CMCP320x_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const unsigned char p_cs_gpio = 0xff, AbstractGpioExpender * p_gpioExpender = NULL, const Mcp320xFamilly p_familly = _3201, const unsigned int p_frequency = 1000000);
    
        /** Destructor
         * If managed, the /CS pin is set to 1 before to release it
         */
        virtual ~CMCP320x_SPI();

        /** Used to return the unique instance of SPI instance
         */
        inline const SPI * operator * () { return (const SPI *)_spiInstance; };

        /** 
         * @desc Launch an analog to digital conversion on the specified channel
         * \param p_channel The channel to convert
         * \return The converted value
         */
        float Read(const Mcp320xChannels p_channels = CH1);
        
        /** 
         * @desc Change current configuration (only for MCP3204/8)
         * \param p_setConfig Set to true for single-ended inputs configuration, false for pseudo-differential inputs
         * \see DS21298E-page 17 Clause 4.1 Analog Inputs
         */
        void SetConfig(const bool p_settings);
    
         /** Shutdown the device
         */
        bool Shutdown(const bool p_shutdown);
    
   private:
        /** Internal reference identifier
         */
        std::string _internalId;
        
    private:
    
        /** 
         * @desc Launch an analog to digital conversion on the specified channel for MCP3201
         * \see DS21290F-page 17 Clause 4.1 Analog Inputs
         */
        void Read_3201();
        
        /** 
         * @desc Launch an analog to digital conversion on the specified channel for MCP3204/8
         * \param p_setConfig Set to true for single-ended inputs configuration, false for pseudo-differential inputs
         * \see DS21298E-page 17 Clause 4.1 Analog Inputs
         */
        void Read_320x(const Mcp320xChannels p_channels);
        
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
        
    }; // End of class CMCP320x_SPI

} // End of namespace MCP320x_SPI

using namespace MCP320x_SPI;
