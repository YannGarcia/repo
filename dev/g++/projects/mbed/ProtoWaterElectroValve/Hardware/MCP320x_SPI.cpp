/* mbed simplified access to Microchip 12 bits ADC devices (SPI)
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

#include "MCP320x_SPI.h"

namespace MCP320x_SPI {

    unsigned char CMCP320x_SPI::SPIModuleRefCounter = 0;

    CMCP320x_SPI::CMCP320x_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const PinName p_cs, const Mcp320xFamilly p_familly, const unsigned int p_frequency) : 
        _cs_gpio(0xff),
        _gpioExpender(NULL),
        _internalId("")
    {
        DEBUG_ENTER("CMCP320x_SPI")
        
        CMCP320x_SPI::SPIModuleRefCounter += 1;
        if (CMCP320x_SPI::SPIModuleRefCounter > 1) {
            //FIXME Check that SPI settings are identical. Otherwise it should failed
            return;
        }

        _familly = p_familly;
        switch (_familly) {
            case _3201:
                _channelsNum = 1;
                break;
            case _3204:
                _channelsNum = 4;
                break;
            default: // _3208
                _channelsNum = 8;
        } // End of 'switch' statement
        _settings = 0x02; // SGL/DIFF bit set to 1 = See DS21298E-page 19 TABLE 5-1: CONFIGURATION BITS FOR THE MCP3204/TABLE 5-2: CONFIGURATION BITS FOR THE MCP3208
        DEBUG("CMCP320x_SPI: familly:%d - #channels:%d", _familly, _channelsNum)
        _spiInstance = new SPI(p_mosi, p_miso, p_sclk);
        _spiInstance->frequency(p_frequency); // Set the frequency of the SPI interface
        _spiInstance->format(8, 3);
        DEBUG_ENTER("CMCP320x_SPI: refCounter=%d", CMCP320x_SPI::SPIModuleRefCounter)

       if (p_cs != NC) {
            DEBUG("CMCP320x_SPI: /CS managed");
            _cs = new DigitalOut(p_cs);
            _cs->write(1); // Disable chip
        } else {
            DEBUG("CMCP320x_SPI: /CS not managed");
            _cs = NULL; // Not used
        }
    
        DEBUG_LEAVE("CMCP320x_SPI")
    }
    
    CMCP320x_SPI::CMCP320x_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const unsigned char p_cs_gpio, AbstractGpioExpender * p_gpioExpender, const Mcp320xFamilly p_familly, const unsigned int p_frequency) : 
        _cs(NULL),
        _cs_gpio(p_cs_gpio),
        _gpioExpender(p_gpioExpender),
        _internalId("")
    {
        DEBUG_ENTER("CMCP320x_SPI")
        
        CMCP320x_SPI::SPIModuleRefCounter += 1;
        if (CMCP320x_SPI::SPIModuleRefCounter > 1) {
            //FIXME Check that SPI settings are identical. Otherwise it should failed
            return;
        }

        _familly = p_familly;
        switch (_familly) {
            case _3201:
                _channelsNum = 1;
                break;
            case _3204:
                _channelsNum = 4;
                break;
            default: // _3208
                _channelsNum = 8;
        } // End of 'switch' statement
        _settings = 0x02; // SGL/DIFF bit set to 1 = See DS21298E-page 19 TABLE 5-1: CONFIGURATION BITS FOR THE MCP3204/TABLE 5-2: CONFIGURATION BITS FOR THE MCP3208
        DEBUG("CMCP320x_SPI: familly:%d - #channels:%d", _familly, _channelsNum)
        _spiInstance = new SPI(p_mosi, p_miso, p_sclk);
        _spiInstance->frequency(p_frequency); // Set the frequency of the SPI interface
        _spiInstance->format(8, 3);
        DEBUG_ENTER("CMCP320x_SPI: refCounter=%d", CMCP320x_SPI::SPIModuleRefCounter)
        
        DEBUG_LEAVE("CMCP320x_SPI")
    }
    
    CMCP320x_SPI::~CMCP320x_SPI() {
        DEBUG_ENTER("~CMCP320x_SPI")
    
        // Release I2C instance
        DEBUG_ENTER("~CMCP320x_SPI: refCounter=%d", CMCP320x_SPI::SPIModuleRefCounter)
        CMCP320x_SPI::SPIModuleRefCounter -= 1;
        if (CMCP320x_SPI::SPIModuleRefCounter == 0) {
            delete _spiInstance;
            _spiInstance = NULL;
        }
        // Release _reset if required
        if (_cs != NULL) {
            _cs->write(1);
            delete _cs;
        }
   
        DEBUG_LEAVE("~CMCP320x_SPI")
    }

    float CMCP320x_SPI::Read(const Mcp320xChannels p_channels) {
        DEBUG_ENTER("CMCP320x_SPI::Read: %d", (unsigned char)p_channels)
        
        // Read a sample
        _sample.value = 0x00;
        switch (_familly) {
            case _3204:
                // No break;
            case _3208:
                Read_320x(p_channels);
                break;
            default: // _3201
                Read_3201();
                break;
        } // End of 'switch' statement 
        DEBUG("CMCP320x_SPI::Read: 0x%02x - 0x%02x", _sample.bytes[0], _sample.bytes[1])
        // Convert it
        float temp;
        _sample.value >>= 1; // Adjust composite integer for 12 valid bits
        _sample.value &= 0x0FFF; // Mask out upper nibble of integer
        temp = (_sample.value * 0.001225585);
        
        DEBUG_LEAVE("CMCP320x_SPI::Read: %f", temp)
        return temp;        
    }
    
    void CMCP320x_SPI::SetConfig(const bool p_settings) {
        DEBUG_LEAVE("CMCP320x_SPI::SetConfig: %x", (unsigned char)p_settings)
        
        if (_settings) {
            _settings = 0x02;
        } else {
            _settings = 0x00;
        }
    }
    
    bool CMCP320x_SPI::Shutdown(const bool p_shutdown) {
        // Sanity check
        if ((_cs == NULL) && (_gpioExpender != NULL)) {
            return false;
        }
        
        if (_cs != NULL) {
            _cs->write(p_shutdown == false ? 0 : 1);
        } else if (_gpioExpender != NULL) {
            _gpioExpender->write(_cs_gpio, p_shutdown == false ? 0 : 1);
        }
        
        return true;
    }
    
    void CMCP320x_SPI::Read_3201() {
        enableDevice();
        
        _sample.bytes[1] = _spiInstance->write(0);
        _sample.bytes[0] = _spiInstance->write(0);
        
        disableDevice();
    }

    void CMCP320x_SPI::Read_320x(const Mcp320xChannels p_channels) {
        DEBUG_ENTER("CMCP320x_SPI::Read_320x: %d", (unsigned char)p_channels)
        
        unsigned char _channels = (unsigned char)p_channels % _channelsNum;
        // Set start bit 
        unsigned char mask = 0x04 | _settings; // Start bit set to 1 - See DS21298E-page 19 Clause 5.0 SERIAL COMMUNICATIONS
        // Set channel address
        unsigned char cmd0;
        unsigned char cmd1;
        if (_familly == _3204) {
            cmd0 = mask;
            cmd1 = _channels << 6; // MCP3204 has 4 channels in single-ended mode
        } else { // MCP3208
            cmd0 = mask | ((_channels & 0x04) >> 2); // Extract D2 bit - See DS21298E-page 19 Clause 5.0 SERIAL COMMUNICATIONS
            cmd1 = _channels << 6; // MCP3204 has 8 channels in single-ended mode
        }
        DEBUG("CMCP320x_SPI::Read_320x: cmd0:%02x - cmd1:%02x", cmd0, cmd1)
        if (_cs != NULL) {
            _cs->write(0);
            wait_us(1);
        }
        _spiInstance->write(cmd0); // Don't care of the result - See DS21298E-page 21 Clause 6.1 Using the MCP3204/3208 with Microcontroller (MCU) SPI Ports
        _sample.bytes[1] = _spiInstance->write(cmd1); // DS21298E-page 21 See FIGURE 6-1: SPI Communication using 8-bit segments (Mode 0,0: SCLK idles low)
        _sample.bytes[0] = _spiInstance->write(0);
        if (_cs != NULL) {
            _cs->write(1);
        }
            
        DEBUG_LEAVE("CMCP320x_SPI::Read_320x")
    }
    
} // End of namespace MCP320x_SPI
