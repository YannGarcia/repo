/* Raspberry simplified access to Microchip 12 bits ADC devices (SPI)
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
#include <iostream> // Used for cerr
#include <iomanip> // Used for setw, setprecision...
#include <cstring> // Used for strerror

#include <unistd.h> // Used for usleep and close

#include "mcp320x_spi.h"
#include "wiringBbbSPI.h"
#include "beagleboneHwEx.h"

namespace beagleboneHw {

  namespace spi {

    unsigned char mcp320x_spi::SPIModuleRefCounter = 0;

    mcp320x_spi::mcp320x_spi(const unsigned char p_cs, const Mcp320xFamilly p_familly, const unsigned int p_frequency) : _internalId("") {
      mcp320x_spi::SPIModuleRefCounter += 1;
      if (mcp320x_spi::SPIModuleRefCounter > 1) {
	std::cerr << "mcp320x_spi: Not yet implemented" << std::endl;
	throw beagleboneHwEx("mcp320x_spi: Not yet implemented");
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
      std::clog << "mcp320x_spi: familly:" << std::dec << _familly << " - #channels:" << (int)_channelsNum << std::endl;
	// Create a spi connection
	_spiChannel = 0; // TODO To be enhanced
      if (::wiringBbbSPISetup(_spiChannel, p_frequency) == -1) {
	std::cerr << "Failed: wiringBbbSPISetup: " << strerror(errno) << std::endl;
	throw beagleboneHwEx("mcp320x_spi: wiringBbbSPISetup failure");
      }

      // TODO Spi setup is OK
      //   _spiInstance->frequency(p_frequency); // Set the frequency of the SPI interface
      //    _spiInstance->format(8, 3);

      // TODO Declare wiringBbb node
    
      _cs = p_cs;
      if (_cs != 0xff) {
	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH); // Disable chip
      }
    } // End of Constructor
    
    mcp320x_spi::~mcp320x_spi() {
      // Release I2C instance
      DEBUG_ENTER("~mcp320x_spi: refCounter=%d", mcp320x_spi::SPIModuleRefCounter)
	mcp320x_spi::SPIModuleRefCounter -= 1;
      if (mcp320x_spi::SPIModuleRefCounter == 0) {
	// Release _reset if required
	close(_spiChannel);
	_spiChannel = -1;
      }
      
    } // End of Destructor
    
    float mcp320x_spi::read(const Mcp320xChannels p_channels) {    
      // Read a samplea
      _sample.value = 0x00;
      switch (_familly) {
      case _3204:
	// No break;
      case _3208:
	read_320x(p_channels);
	break;
      default: // _3201
	read_3201();
	break;
      } // End of 'switch' statement 
      //      std::clog << "mcp320x_spi::read: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)_sample.bytes[0] << " - 0x" << (int)_sample.bytes[1] << std::endl;
      // Convert it
      float temp;
      _sample.value >>= 1; // Adjust composite integer for 12 valid bits
      _sample.value &= 0x0FFF; // Mask out upper nibble of integer
      temp = (_sample.value * 0.001225585);
      
      //      std::clog << "mcp320x_spi::read: " << std::setprecision(5) << temp << std::endl;
      return temp;        
    } // End of method read
    
    void mcp320x_spi::setConfig(const bool p_settings) {
      //      DEBUG_LEAVE("mcp320x_spi::setConfig: %x", (unsigned char)p_settings)
    
      if (_settings) {
	_settings = 0x02;
      } else {
	_settings = 0x00;
      }
    } // End of method setConfig
    
    bool mcp320x_spi::shutdown(const bool p_shutdown) {
      // Sanity check
      if (_cs == 0xff) {
	return false;
      }
      digitalWrite(_cs, p_shutdown == false ? LOW : HIGH);
      
      return true;
    } // End of method shutdown
    
    void mcp320x_spi::read_3201() {
      if (_cs != 0xff) {
	digitalWrite(_cs, LOW);
      }
      unsigned char cmd = 0x00;
      wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      _sample.bytes[1] = cmd;
      cmd = 0x00;
      wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      _sample.bytes[0] = cmd;
      if (_cs != 0xff) {
	digitalWrite(_cs, HIGH);
      }    
    } // End of method read_3201
    
    void mcp320x_spi::read_320x(const Mcp320xChannels p_channels) {
      //      DEBUG_ENTER("mcp320x_spi::read_320x: %d", (unsigned char)p_channels)
      
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
      //      DEBUG("mcp320x_spi::read_320x: cmd0:%02x - cmd1:%02x", cmd0, cmd1)
      if (_cs != 0xff) {
	digitalWrite(_cs, LOW);
	usleep(1);
      }
      wiringBbbSPIDataRW(_spiChannel, &cmd0, sizeof(unsigned char)); // Don't care of the result - See DS21298E-page 21 Clause 6.1 Using the MCP3204/3208 with Microcontroller (MCU) SPI Ports
      wiringBbbSPIDataRW(_spiChannel, &cmd1, sizeof(unsigned char)); // DS21298E-page 21 See FIGURE 6-1: SPI Hwunication using 8-bit segments (Mode 0,0: SCLK idles low)
      _sample.bytes[1] = cmd1;
      cmd0 = 0x00; // Dummy value
      wiringBbbSPIDataRW(_spiChannel, &cmd0, sizeof(unsigned char));
      _sample.bytes[0] = cmd0;
      //      DEBUG("mcp320x_spi::read_320x: 0x%02x - 0x%02x", _sample.bytes[0], _sample.bytes[1])
      if (_cs != 0xff) {
        digitalWrite(_cs, HIGH);
      }
      
      //      DEBUG_LEAVE("mcp320x_spi::read_320x")
    } // End of method read_320x

  } // End of namespace spi
    
} // End of namespace beagleboneHw
