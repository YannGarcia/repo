/* mbed simplified access to Microchip 24LCxx Serial EEPROM devices (SPI)
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

#include "MCP4xxxx_SPI.h"

namespace MCP4xxxx_SPI {

    unsigned char CMCP4xxxx_SPI::SPIModuleRefCounter = 0;

    CMCP4xxxx_SPI::CMCP4xxxx_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const PinName p_cs, const PinName p_reset, const PinName p_shdn, const Mcp4xxxFamily p_family, const unsigned int p_frequency) : 
        _cs_gpio(0xff),
        _reset_gpio(0xff),
        _shdn_gpio(0xff),
        _gpioExpender(NULL),
        _internalId("")
    {
        DEBUG_ENTER("CMCP4xxxx_SPI")

        if (CMCP4xxxx_SPI::SPIModuleRefCounter != 0) {
            error("CMCP4xxxx_SPI: Wrong params");
        }

        _spiInstance = new SPI(p_mosi, p_miso, p_sclk);
        _spiInstance->frequency(p_frequency); // Set the frequency of the SPI interface
        _spiInstance->format(16, 0); // See http://mbed.org/users/mbed_official/code/mbed/docs/0954ebd79f59//classmbed_1_1SPI.html
        CMCP4xxxx_SPI::SPIModuleRefCounter += 1;
        DEBUG_ENTER("CMCP4xxxx_SPI: refCounter=%d", CMCP4xxxx_SPI::SPIModuleRefCounter)

        if (p_cs != NC) {
            DEBUG("CMCP4xxxx_SPI: /CS managed");
            _cs = new DigitalOut(p_cs);
            _cs->write(1); // Disable chip
        } else {
            DEBUG("CMCP4xxxx_SPI: /CS not managed");
            _cs = NULL; // Not used
        }
    
        if (p_reset != NC) {
            DEBUG("CMCP4xxxx_SPI: /RESET managed");
            _reset = new DigitalOut(p_reset);
            _reset->write(1); // Disable reset
        } else {
            DEBUG("CMCP4xxxx_SPI: /RESET not managed");
            _reset = NULL; // Not used
        }
    
        if (p_shdn != NC) {
            DEBUG("CMCP4xxxx_SPI: /SHDN managed");
            _shdn = new DigitalOut(p_shdn);
            _shdn->write(1); // Disable shutdown
        } else {
            DEBUG("CMCP4xxxx_SPI: /SHDN not managed");
            _shdn = NULL; // Not used
        }
        
        _family = p_family;
        SetFamilly(p_family);
        
        _incStep = 0x80; // Set to middle step
        
        DEBUG_LEAVE("CMCP4xxxx_SPI")
    }

    CMCP4xxxx_SPI::CMCP4xxxx_SPI(const PinName p_mosi, const PinName p_miso, const PinName p_sclk, const unsigned char p_cs_gpio, const unsigned char p_reset_gpio, const unsigned char p_shdn_gpio, AbstractGpioExpender * p_gpioExpender, const Mcp4xxxFamily p_family, const unsigned int p_frequency) : 
        _cs(NULL),
        _reset(NULL),
        _shdn(NULL),
        _cs_gpio(p_cs_gpio),
        _reset_gpio(p_reset_gpio),
        _shdn_gpio(p_shdn_gpio),
        _gpioExpender(p_gpioExpender),
        _internalId("")
    {
        DEBUG_ENTER("CMCP4xxxx_SPI")

        if (CMCP4xxxx_SPI::SPIModuleRefCounter != 0) {
            error("CMCP4xxxx_SPI: Wrong params");
        }

        _spiInstance = new SPI(p_mosi, p_miso, p_sclk);
        _spiInstance->frequency(p_frequency); // Set the frequency of the SPI interface
        _spiInstance->format(16, 0); // See http://mbed.org/users/mbed_official/code/mbed/docs/0954ebd79f59//classmbed_1_1SPI.html
        CMCP4xxxx_SPI::SPIModuleRefCounter += 1;
        DEBUG_ENTER("CMCP4xxxx_SPI: refCounter=%d", CMCP4xxxx_SPI::SPIModuleRefCounter)
        
        _family = p_family;
        SetFamilly(p_family);
        
        _incStep = 0x80; // Set to middle step
        
        DEBUG_LEAVE("CMCP4xxxx_SPI")
    }
    
    CMCP4xxxx_SPI::~CMCP4xxxx_SPI() {
        DEBUG_ENTER("~CMCP4xxxx_SPI")
    
        // Release I2C instance
        DEBUG_ENTER("~CMCP4xxxx_SPI: refCounter=%d", CMCP4xxxx_SPI::SPIModuleRefCounter)
        CMCP4xxxx_SPI::SPIModuleRefCounter -= 1;
        if (CMCP4xxxx_SPI::SPIModuleRefCounter == 0) {
            delete _spiInstance;
            _spiInstance = NULL;
        }
        // Release _reset if required
        if (_cs != NULL) {
            _cs->write(0);
            delete _cs;
        }
        // Release _reset if required
        if (_reset != NULL) {
            _reset->write(0);
            delete _reset;
        }
        // Release _shdn if required
        if (_shdn != NULL) {
            _shdn->write(0);
            delete _shdn;
        }
   
        DEBUG_LEAVE("~CMCP4xxxx_SPI")
    }
    
    unsigned short CMCP4xxxx_SPI::WriteRegister(const Addresses p_address, const unsigned char p_value) {
        DEBUG_ENTER("CMCP4xxxx_SPI::WriteRegister: 0x%02x - 0x%02x - 0x%02x", (unsigned char)p_address, p_value, (unsigned char)_family)
        
        // Sanity check
        if (p_address == CMCP4xxxx_SPI::Status) {
            // Wrong parameters
            return (unsigned short) -1;
        }
        
        unsigned short command = ((p_address & 0x0f) << 4 | 0x00/*TODO Use 'define' for Write command*/) << 8;
        command |= p_value;
        
        DEBUG("CMCP4xxxx_SPI::WriteRegister: Send command: 0x%04x", command)
        enableDevice();
        unsigned short result = _spiInstance->write(command);
        disableDevice();
        
        DEBUG_LEAVE("CMCP4xxxx_SPI::WriteRegister: %d", result & 0x00ff)
        return result & 0x00ff;
    }
    
    unsigned short CMCP4xxxx_SPI::ReadRegister(const Addresses p_address) {
        DEBUG_ENTER("CMCP4xxxx_SPI::ReadRegister: 0x%02x", (unsigned char)p_address)
        
        unsigned short command = ((p_address & 0x0f) << 4 | (0x03/*TODO Use 'define' for Read command*/ << 2)) << 8;
        
        DEBUG("CMCP4xxxx_SPI::ReadRegister: Send command: 0x%04x", command)
        enableDevice();
        unsigned short result = _spiInstance->write(command);
        DEBUG("CMCP4xxxx_SPI::ReadRegister: full result=0x%04x", result)
        disableDevice();
        
        DEBUG_LEAVE("CMCP4xxxx_SPI::ReadRegister: 0x%02x", result & 0x00ff)
        return result & 0x00ff;
    }
    
    unsigned short CMCP4xxxx_SPI::Increment(const Commands p_command) { 
        // Sanity check
        if (_incStep != 0xff) { 
            _incStep += 1; // TODO Change increment 1 by a parametrized increment step
        }
        return Write(p_command, _incStep);
    }
    
    unsigned short CMCP4xxxx_SPI::Decrement(const Commands p_command) { 
        // Sanity check
        if (_incStep != 0x00) { 
            _incStep -= 1; // TODO Change increment 1 by a parametrized increment step
        }
        return Write(p_command, _incStep);
    }

    
    unsigned short CMCP4xxxx_SPI::Write(const Commands p_command, const unsigned char p_value) {
        DEBUG_ENTER("CMCP4xxxx_SPI::Write: 0x%02x - 0x%02x - 0x%02x", (unsigned char)p_command, p_value, (unsigned char)_family)
        
        // Sanity check
        if ((p_command != WriteToDigiPot1) && (p_command != WriteToDigiPot2) && (p_command != WriteToBoth)) {
            // Wrong parameters
            return (unsigned short) -1;
        }
        
        unsigned short command = 0;
        if ((_family == CMCP4xxxx_SPI::_41xxx) || (_family == CMCP4xxxx_SPI::_42xxx)) {  // See DS11195C
            switch (p_command) {
                case WriteToDigiPot1: 
                    command = (0x11 << 8 | p_value);
                    break;
                case WriteToDigiPot2:
                    command = (0x12 << 8 | p_value);
                    break;
                default:
                    command = (0x13 << 8 | p_value);
            } // End of 'switch' statement
        } else { // See DS22060B
            switch (p_command) {
                case WriteToDigiPot2:  // Wiper1: adress=0x01, WriteCmd=00
                    command = (0x01 << 5 | p_value);
                    break;
                default:
                    command = p_value;  // Wiper1: adress=0x00, WriteCmd=00
            } // End of 'switch' statement
        }
        
        DEBUG("CMCP4xxxx_SPI::Write: Send command: 0x%04x", (unsigned char)command)
        enableDevice();
        unsigned short result = _spiInstance->write(command);
        disableDevice();
        
        DEBUG_LEAVE("CMCP4xxxx_SPI::Write: %d", result)
        return result;
    }

    unsigned short CMCP4xxxx_SPI::Shutdown(const Commands p_command, const bool p_set) {
        DEBUG_ENTER("CMCP4xxxx_SPI::Shutdown: 0x%02x - 0x%02x", (unsigned char)p_command, (unsigned char)_family)
        
        // Sanity check
        if ((p_command != ShutdownDigiPot1) && (p_command != ShutdownDigiPot2) && (p_command != ShutdownBoth)) {
            // Wrong parameters
            return (unsigned short) -1;
        }
        
       unsigned short command = 0;
       if ((_family == CMCP4xxxx_SPI::_41xxx) || (_family == CMCP4xxxx_SPI::_42xxx)) { // See DS11195C
           switch (p_command) {
                case ShutdownDigiPot1:
                    command = (0x21 << 8);
                    break;
                case ShutdownDigiPot2:
                    command = (0x21 << 8);
                    break;
                default: //<! Shutdown both digital potentiometers
                    command = (0x23 << 8);                
            } // End of 'switch' statement
        } else { // See DS22060B
            unsigned short tcon = ReadRegister(TCon); // Read TCon register 
            if (p_set == true) {
               switch (p_command) {
                    case ShutdownDigiPot1:
                        command = 0x4000 | (tcon & 0xf8);
                        break;
                    case ShutdownDigiPot2:
                        command = 0x4000 | (tcon & 0x8f);
                        break;
                    default: //<! Shutdown both digital potentiometers
                        command = 0x4000;                
                } // End of 'switch' statement
            } else {
                switch (p_command) {
                    case ShutdownDigiPot1:
                        command = 0x4000 | (tcon | 0x000f);
                        break;
                    case ShutdownDigiPot2:
                        command = 0x4000 | (tcon | 0x00f0);
                        break;
                    default: //<! Shutdown both digital potentiometers
                        command = 0x40ff;                
                } // End of 'switch' statement
            }
        }
        
        DEBUG("CMCP4xxxx_SPI::Shutdown: Send command: 0x%04x", command)
        enableDevice();
        unsigned short result = _spiInstance->write(command);
        disableDevice();
        
        DEBUG_LEAVE("CMCP4xxxx_SPI::Shutdown: %d", result)
        return result;
    }
    
    //unsigned short CMCP4xxxx_SPI::Write() {
    //    return _spiInstance->write(0);
    //}
    
    CMCP4xxxx_SPI::Mcp4xxxFamily CMCP4xxxx_SPI::SetFamilly(const CMCP4xxxx_SPI::Mcp4xxxFamily p_family) { 
        DEBUG_ENTER("CMCP4xxxx_SPI::SetFamilly: 0x%02x", (unsigned char)p_family)
        
        Mcp4xxxFamily old = _family;
        _family = p_family;
        if ((_family != CMCP4xxxx_SPI::_41xxx) && (_family != CMCP4xxxx_SPI::_42xxx)) {
            // Setup TCON register 
            DEBUG("CMCP4xxxx_SPI::SetFamilly: Setup TCON register")
            WriteRegister(CMCP4xxxx_SPI::TCon, 0xff); // See DS22060B-page 36 REGISTER 4-2: TCON BITS
                                                      // TODO Use 'define' for command
        }
        
       DEBUG_LEAVE("CMCP4xxxx_SPI::SetFamilly: 0x%02x", (unsigned char)old)
       return old;
    }

    void CMCP4xxxx_SPI::Reset() {
        _incStep = 0x80; // Set to middle step
        // Sanity check
        if (_reset != NULL) {
            _reset->write(0); // Set level low to activate reset 
            wait_us(1); // Wait for 1us
            _reset->write(1); // Set level low to de-activate reset 
        } else if ((_gpioExpender != NULL) && (_reset_gpio != 0xff)) {
            _gpioExpender->write(_reset_gpio, 0);
            wait_us(1); // Wait for 1us
            _gpioExpender->write(_reset_gpio, 1);
        }
    }
    
    void CMCP4xxxx_SPI::Shutdown(const bool p_set) {
        if (_shdn != NULL) {
            _shdn->write(p_set == false ? 0 : 1);
        } else if ((_gpioExpender != NULL) && (_shdn_gpio != 0xff)) {
            _gpioExpender->write(_shdn_gpio, 0);
            wait_us(1); // Wait for 1us
            _gpioExpender->write(_shdn_gpio, 1);
        }
    }

} // End of namespace MCP4xxxx_SPI
