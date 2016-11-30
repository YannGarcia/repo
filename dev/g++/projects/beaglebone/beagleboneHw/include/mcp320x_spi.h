/* Simplified access to Microchip MCP320x 12 bits ADC devices (SPI)
 * Copyright (c) 2013-2014 ygarcia, MIT License
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
#if !defined(__MCP320x_SPI_H__)
#define __MCP320x_SPI_H__

#include <string>
#include <vector>

#include "wiringBbb.h" // wiringBbb library

#include "debug.h" // Include mbed header + debug primitives. See DebugLibrary

namespace beagleboneHw {

  namespace spi {

    /** This class provides simplified SPI access to a Microchip MCP320x 12-Bit A/D Converter with SPI Serial Interface device. V0.0.0.1
     *
     * Microchip MCP42xxx/MCP41xxx Serial EEPROM device reference: DS11195C
     *
     * Note that MCP3201 has no SI pin, only a SO output pin
     * Note that for SPI details, please visit http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
     *
     * @remark This class was validated with Tektronix TDS2014 oscilloscope in 3.3V
     * @author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
     */
    class mcp320x_spi { 
      /** Reference counter used to guarentee unicity of the instance of SPI class
       */
      static unsigned char SPIModuleRefCounter;

      /** ChipSelect (pin 1) see DS21290F-page 15 Clause 3.3 Chip Select/Shutdown (CS/SHDN)
       */
      unsigned char _cs;
        
      /** An unique instance of SPI class
       */
      int _spiChannel;
        
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
       * @see DS21298E-page 19 Clause 5.0 SERIAL COMMUNICATIONS
       */
      unsigned char _settings;

    public:
      /** MCP320x familly
       */
      enum Mcp320xFamilly : unsigned char {
	_3201 = 0x00, /** See DS21290F */
	  _3204 = 0x01, /** See DS21298E */
	  _3208 = 0x03  /** See DS21298E */
	  };
      Mcp320xFamilly _familly;
      /** MCP320x channels to read
       */
      enum Mcp320xChannels : unsigned char {
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
       * @param p_cs        Pin for Chip Select. If NC, assumes that application manage /CS, default value is NC, not connected
       * @param p_familly   MCP320x familly. Default: _3201
       * @param p_frequency Frequency of the SPI interface (SCK), default value is 1MHz
       */
      mcp320x_spi(const unsigned char p_cs = 0xff, const Mcp320xFamilly p_familly = _3201, const unsigned int p_frequency = 1000000);
    
      /** Destructor
       * If managed, the /CS pin is set to 1 before to release it
       */
      virtual ~mcp320x_spi();

      /** 
       * @desc Launch an analog to digital conversion on the specified channel
       * @param p_channel The channel to convert
       * @return The converted value
       */
      float read(const Mcp320xChannels p_channels = mcp320x_spi::CH1);
        
      /** 
       * @desc Change current configuration (only for MCP3204/8)
       * @param p_setConfig Set to true for single-ended inputs configuration, false for pseudo-differential inputs
       * @see DS21298E-page 17 Clause 4.1 Analog Inputs
       */
      void setConfig(const bool p_settings);
    
      /** Shutdown the device
       */
      bool shutdown(const bool p_shutdown);
    
    private:
      /** Internal reference identifier
       */
      std::string _internalId;
        
    private:
    
      /** 
       * @desc Launch an analog to digital conversion on the specified channel for MCP3201
       * @see DS21290F-page 17 Clause 4.1 Analog Inputs
       */
      void read_3201();
        
      /** 
       * @desc Launch an analog to digital conversion on the specified channel for MCP3204/8
       * @param p_setConfig Set to true for single-ended inputs configuration, false for pseudo-differential inputs
       * @see DS21298E-page 17 Clause 4.1 Analog Inputs
       */
      void read_320x(const Mcp320xChannels p_channels);    
        
    }; // End of class mcp320x_spi

  } // End of namespace spi

} // End of namespace beagleboneHw

using namespace beagleboneHw::spi;

#endif // __MCP320x_SPI_H__
