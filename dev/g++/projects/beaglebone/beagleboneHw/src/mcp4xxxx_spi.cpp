/* beaglebone simplified access to Microchip 24LCxx Serial EEPROM devices (SPI)
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
#include <iostream> // Used for std::cerr
#include <cstring> // Used for strerror

#include <unistd.h> // Used for usleep and close

#include "mcp4xxxx_spi.h"
#include "wiringBbbSPI.h"
#include "wiringBbb.h"
#include "beagleboneHwEx.h"

namespace beagleboneHw {

  namespace spi {

    unsigned char mcp4xxxx_spi::SPIModuleRefCounter = 0;

    mcp4xxxx_spi::mcp4xxxx_spi(const unsigned char p_cs, const unsigned char p_reset, const unsigned char p_shdn, const Mcp4xxxxFamily p_family, const unsigned int p_frequency) : _internalId("") {
      DEBUG_ENTER("mcp4xxxx_spi")

	mcp4xxxx_spi::SPIModuleRefCounter += 1;
      DEBUG_ENTER("mcp4xxxx_spi: refCounter=%d", mcp4xxxx_spi::SPIModuleRefCounter)
	if (mcp4xxxx_spi::SPIModuleRefCounter > 1) {
	  //FIXME Check that SPI settings are identical. Otherwise it should failed
	  return;
	}

      // Create a spi connection
      _spiChannel = 0; // TODO To be enhanced
      if (::wiringBbbSPISetup(_spiChannel, p_frequency) == -1) {
	std::cerr << "Failed: wiringBbbSPISetup: " << strerror(errno) << std::endl;
	throw beagleboneHwEx("mcp4xxxx_spi: wiringBbbPISetup failure");
      }
      // TODO Spi setup is OK
      //    _spiChannel->frequency(p_frequency); // Set the frequency of the SPI interface
      //    _spiChannel->format(16, 0); // See http://mbed.org/users/mbed_official/code/mbed/docs/0954ebd79f59//classmbed_1_1SPI.html

      // TODO Declare wiringBbb node
      
      _cs = p_cs;
      if (p_cs != 0xff) {
	DEBUG("mcp4xxxx_spi: /CS managed");
	pinMode(_cs, OUTPUT);
	::digitalWrite(_cs, HIGH); // Disable chip
      } else {
	DEBUG("mcp4xxxx_spi: /CS not managed");
      }
    
      _reset = p_reset;
      if (p_reset != 0xff) {
	DEBUG("mcp4xxxx_spi: /RESET managed");
	::pinMode(_reset, OUTPUT);
	::digitalWrite(_reset, HIGH); // Disable reset
      } else {
	DEBUG("mcp4xxxx_spi: /RESET not managed");
      }

      _shdn = p_shdn;
      if (p_shdn != 0xff) {
	DEBUG("mcp4xxxx_spi: /SHDN managed");
	::pinMode(_shdn, OUTPUT);
	::digitalWrite(_shdn, HIGH); // Disable shutdown
      } else {
	DEBUG("mcp4xxxx_spi: /SHDN not managed");
      }
        
      _family = p_family;
      if ((_family != mcp4xxxx_spi::_41xxx) && (_family != mcp4xxxx_spi::_42xxx)) {
	// Setup TCON register 
	writeRegister(mcp4xxxx_spi::TCon, 0x77);
      }
        
      _incStep = 0x80; // Set to middle step
        
      DEBUG_LEAVE("mcp4xxxx_spi")
	} // End of Constructor

    mcp4xxxx_spi::~mcp4xxxx_spi() {
      DEBUG_ENTER("~mcp4xxxx_spi")
    
	// Release I2C instance
	DEBUG_ENTER("~mcp4xxxx_spi: refCounter=%d", mcp4xxxx_spi::SPIModuleRefCounter)
	mcp4xxxx_spi::SPIModuleRefCounter -= 1;
      if (mcp4xxxx_spi::SPIModuleRefCounter == 0) {
	close(_spiChannel);
	_spiChannel = -1;
      }
    
      DEBUG_LEAVE("~mcp4xxxx_spi")
	}
    
    unsigned short mcp4xxxx_spi::writeRegister(const Addresses p_address, const unsigned char p_value) {
      DEBUG_ENTER("mcp4xxxx_spi::writeRegister: 0x%02x - 0x%02x - 0x%02x", (unsigned char)p_address, p_value, (unsigned char)_family)
        
	// Sanity check
	if (p_address == mcp4xxxx_spi::Status) {
	  // Wrong parameters
	  return (unsigned short) -1;
	}
        
      unsigned char cmd0 = (unsigned char)((p_address & 0x0f) << 4);
      unsigned char cmd1 = p_value;
        
      DEBUG("mcp4xxxx_spi::writeRegister: Send command: 0x%02x%02x", cmd0, cmd1)
	if (_cs != 0xff) {
	  digitalWrite(_cs, LOW);
	}
      ::wiringBbbSPIDataRW(_spiChannel, &cmd0, sizeof(unsigned char));
      unsigned short result = (unsigned short)(cmd0 << 8) & 0xff00;
      ::wiringBbbSPIDataRW(_spiChannel, &cmd1, sizeof(unsigned char));
      result |= (unsigned short)cmd1;
      DEBUG("mcp4xxxx_spi::writeRegister: returned value=%d", result)
	if (_cs != 0xff) {
	  ::digitalWrite(_cs, HIGH);
	}
    
      DEBUG_LEAVE("mcp4xxxx_spi::writeRegister: %d", result & 0x00ff)
	return result & 0x00ff;
    } // End of 'writeRegister' method
    
    unsigned short mcp4xxxx_spi::readRegister(const Addresses p_address) {
      DEBUG_ENTER("mcp4xxxx_spi::readRegister: 0x%02x", (unsigned char)p_address)
        
	unsigned char cmd = (unsigned char)(p_address << 4 | 0x03/*Read command*/);    
      DEBUG("mcp4xxxx_spi::readRegister: Send command: 0x%02x", cmd)
	if (_cs != 0xff) {
	  ::digitalWrite(_cs, LOW);
	}
      ::wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      unsigned short result = (unsigned char)(cmd << 8) & 0xff00;
      cmd = 0x00;
      ::wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      result |= (unsigned short)cmd;
      if (_cs != 0xff) {
	::digitalWrite(_cs, HIGH);
      }
        
      DEBUG_LEAVE("mcp4xxxx_spi::readRegister: %d", result)
	return result;
    }

    unsigned short mcp4xxxx_spi::increment(const Hwands p_command) { 
      // Sanity check
      if (_incStep != 0xff) { 
	_incStep += 1; // TODO Change increment 1 by a parametrized increment step
      }
      return write(p_command, _incStep);
    }
  
    unsigned short mcp4xxxx_spi::decrement(const Hwands p_command) { 
      // Sanity check
      if (_incStep != 0x00) { 
	_incStep -= 1; // TODO Change increment 1 by a parametrized increment step
      }
      return write(p_command, _incStep);
    }
 
    unsigned short mcp4xxxx_spi::write(const Hwands p_command, const unsigned char p_value) {
      DEBUG_ENTER("mcp4xxxx_spi::write: 0x%02x - 0x%02x - 0x%02x", (unsigned char)p_command, p_value, (unsigned char)_family)
        
	// Sanity check
	if ((p_command != WriteToDigiPot1) && (p_command != WriteToDigiPot2) && (p_command != WriteToBoth)) {
	  // Wrong parameters
	  return (unsigned short) -1;
	}
        
      unsigned short command = 0;
      if ((_family == mcp4xxxx_spi::_41xxx) || (_family == mcp4xxxx_spi::_42xxx)) { 
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
      } else {
	switch (p_command) {
	case WriteToDigiPot2:  // Wiper1: adress=0x01, WriteCmd=00
	  command = (0x01 << 5 | p_value);
	  break;
	default:
	  command = p_value;  // Wiper1: adress=0x00, WriteCmd=00
	} // End of 'switch' statement
      }
        
      DEBUG("mcp4xxxx_spi::write: Send command: 0x%04x", (unsigned char)command)
	if (_cs != 0xff) {
	  ::digitalWrite(_cs, LOW);
	}
      unsigned char cmd = (unsigned char)(command >> 8);
      ::wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      unsigned short result = (unsigned short)(cmd << 8) & 0xff00;
      cmd = (unsigned char)command;
      ::wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      result |= (unsigned short)cmd;
      if (_cs != 0xff) {
	::digitalWrite(_cs, HIGH);
      }
        
      DEBUG_LEAVE("mcp4xxxx_spi::write: %d", result)
	return result;
    } // End of 'write' method

    unsigned short mcp4xxxx_spi::shutdown(const Hwands p_command, const bool p_set) {
      DEBUG_ENTER("mcp4xxxx_spi::shutdown: 0x%02x - 0x%02x", (unsigned char)p_command, (unsigned char)_family)
        
	// Sanity check
	if ((p_command != ShutdownDigiPot1) && (p_command != ShutdownDigiPot2) && (p_command != ShutdownBoth)) {
	  // Wrong parameters
	  return (unsigned short) -1;
	}
    
      unsigned short command = 0;
      if ((_family == mcp4xxxx_spi::_41xxx) || (_family == mcp4xxxx_spi::_42xxx)) { // See DS11195C
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
	unsigned short tcon = readRegister(TCon); // Read TCon register 
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
    
      DEBUG("mcp4xxxx_spi::shutdown: Send command: 0x%04x", command)
	if (_cs != 0xff) {
	  ::digitalWrite(_cs, LOW);
	}
      unsigned char cmd = (unsigned char)(command >> 8);
      ::wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      unsigned short result = (unsigned short)(cmd << 8) & 0xff00;
      cmd = (unsigned char)command;
      ::wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      result |= (unsigned short)cmd;
      if (_cs != 0xff) {
	::digitalWrite(_cs, HIGH);
      }
    
      DEBUG_LEAVE("mcp4xxxx_spi::shutdown: %d", result)
	return result;
    } // End of 'shutdown' method

    mcp4xxxx_spi::Mcp4xxxxFamily mcp4xxxx_spi::setFamily(const Mcp4xxxxFamily p_family) { 
      DEBUG_ENTER("mcp4xxxx_spi::setFamily: 0x%02x", (unsigned char)p_family)
      
	Mcp4xxxxFamily old = _family;
      _family = p_family;
      if ((_family != mcp4xxxx_spi::_41xxx) && (_family != mcp4xxxx_spi::_42xxx)) {
	// Setup TCON register 
	DEBUG("mcp4xxxx_spi::setFamily: Setup TCON register")
	  writeRegister(mcp4xxxx_spi::TCon, 0xff); // See DS22060B-page 36 REGISTER 4-2: TCON BITS
	// TODO Use 'define' for command
      }
      
      DEBUG_LEAVE("mcp4xxxx_spi::setFamily: 0x%02x", (unsigned char)old)
	return old;
    } // End of 'setFamily' method

    void mcp4xxxx_spi::reset() {
      _incStep = 0x80; // Set to middle step
      // Sanity check
      if (_reset != 0xff) {
	::digitalWrite(_reset, LOW); // Set level low to activate reset 
	usleep(1); // Wait for 1us
	::digitalWrite(_reset, HIGH); // Set level low to de-activate reset 
      }       
    } // End of 'reset' method
    
    void mcp4xxxx_spi::shutdown(const bool p_set) {
      if (_shdn != 0xff) {
	::digitalWrite(_shdn, (p_set == false) ? LOW : HIGH);
      }
    } // End of 'shutdown' method
  
  } // End of namespace spi

} // End of namespace beagleboneHw
