/* Raspberry simplified access to Microchip MCP42xxx/MCP41xxx Digital Potentiometer devices (SPI)
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
#if !defined(__MCP4xxxx_SPI_H__)
#define __MCP4xxxx_SPI_H__

#include <string>
#include <vector>

#include "debug.h" // Include mbed header + debug primitives. See DebugLibrary

namespace beagleboneHw {

  namespace spi {

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
    class mcp4xxxx_spi { 
      /** Reference counter used to guarentee unicity of the instance of SPI class
       */
      static unsigned char SPIModuleRefCounter;
        
      /** ChipSelect (pin 1) see DS11195C-page 12 Clause 3.4 Chip Select (CS)
       */
      unsigned char _cs;
        
      /** Reset state indicator (pin 11), see DS11195C-page 21 Clause 5.5 Reset (RS) Pin Operation
       */
      unsigned char _reset;
        
      /** Shutdown state indicator (pin 12) see DS11195C-page 21 5.6 Shutdown (SHDN) Pin Operation
       */
      unsigned char _shdn;
        
      /** An unique instance of SPI class
       */
      int _spiChannel;

      /** Increment step value used for inc/dec methods, when inc/dec commands are not supported (such as _4xxxx families)
       */
      unsigned char _incStep;
    public:
      /** MCP4xxxx family
       */
      enum Mcp4xxxxFamily {
	_41xxx = 0x00,  //<! 41xxx family such as MCP41010 - See DS11195C
	_42xxx = 0x01,  //<! 42xxx family such as MCP42010 - See DS11195C
	_41xx = 0x02,   //<! 41xx family such as MCP4251 - Single Potentiometer/Rheostat
	_42xx = 0x03    //<! 42x2 family such as MCP42x2 - Dual Potentiometer/Rheostat
      };
      Mcp4xxxxFamily _family;     
    public:
      /** Authorized commands
       * See DS11195C-page 18
       */
      enum Hwands {
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
       * @param p_cs  : MBed pin for Chip Select. If NC, assumes that application manage /CS, default value is NC, not connected
       * @param p_reset: MBed pin to manage /RESET input. If NC, /RESET is not managed, default value is NC, not connected
       * @param p_shdn: MBed pin to manage /SHDN input. If NC, /SHDN is not managed, default value is NC, not connected
       * @param p_frequency: Frequency of the SPI interface (SCK), default value is 1MHz
       */
      mcp4xxxx_spi(const unsigned char p_cs = 0xff, const unsigned char p_reset = 0xff, const unsigned char p_shdn = 0xff, const Mcp4xxxxFamily p_family = _41xxx, const unsigned int p_frequency = 1000000);
    
      /** Destructor
       */
      virtual ~mcp4xxxx_spi();

      /** Write a value to the specified register. Only for _41xx/_42xx families
       * @param p_address The register to be written
       * @param p_value The value to write
       * @return 0x0000 on success, any value otherwise
       */
      unsigned short writeRegister(const Addresses p_address, const unsigned char p_value);
    
      /** Read the content of the specified register. Only for _41xx/_42xx families
       * @param p_address The register to be written
       * @return The register value 
       */
      unsigned short readRegister(const Addresses p_address);
    
      /** Increment the specified digital potentiometer
       * @param p_command The digital potentiometer to increment. Default: WriteToDigiPot1
       * @return 0x0000 on success, any value otherwise
       */
      unsigned short increment(const Hwands p_command = WriteToDigiPot1);
    
      /** Decrement the specified digital potentiometer
       * @param p_command The digital potentiometer to decrement. Default: WriteToDigiPot1
       * @return 0x0000 on success, any value otherwise
       */
      unsigned short decrement(const Hwands p_command = WriteToDigiPot1);
    
      /** Send a write a command (WriteToPot1, WriteToPot2 or WriteBoth)
       *
       * @param p_command The command to execute: Write or Shutdown (See DS11195C-page 18)
       * @param p_value The potentiometer selection bits (See DS11195C-page 14 Clause 4.1 Modes of Operation)
       * @return 0x0000 on success, 0Xffff otherwise
       * Exemple:
       * @code
       * unsigned char potLevel;
       * ...
       * g_chipSelect.write(0);
       * g_digitalPot.write(mcp4xxxx_spiHw::WriteToPot2, potLevel);
       * g_chipSelect.write(1);
       * ...
       * @endcode
       */
      unsigned short write(const Hwands p_command, const unsigned char p_value);
    
      /** Send a shutdown a command (ShutdownPot1, ShutdownPot2 or ShutdownBoth)
       *
       * @param p_command The command to execute: Write or Shutdown (See DS11195C-page 18)
       * @param p_value The potentiometer selection bits (See DS11195C-page 14 Clause 4.1 Modes of Operation)
       * @return 0x0000 on success, 0Xffff otherwise
       * Exemple:
       * @code
       * ...
       * g_chipSelect.write(0);
       * g_digitalPot.Write(mcp4xxxx_spiHw::ShutdownPot1);
       * g_chipSelect.write(1);
       * ...
       * @endcode
       */
      unsigned short shutdown(const Hwands p_command, const bool p_set);
    
      /** Change the current family 
       * @param family The new type of ICs 
       * @return the previous type of ICs 
       */
      Mcp4xxxxFamily setFamily(const Mcp4xxxxFamily p_family);
    
      /** Reset the device
       * @code
       * unsigned char potLevel;
       * ...
       * g_chipSelect.write(0);
       * g_digitalPot.Reset();
       * g_chipSelect.write(1);
       * ...
       * @endcode
       */
      void reset();
    
      /** Shutdown (hardware) the device
       */
      void shutdown(const bool p_set);
    
    private:
      /** Internal reference identifier
       */
      std::string _internalId;

    }; // End of class mcp4xxxx_spi

  } // End of namespace spi

} // End of namespace beagleboneHw

using namespace beagleboneHw;

#endif // __MCP4xxxx_SPI_H__
