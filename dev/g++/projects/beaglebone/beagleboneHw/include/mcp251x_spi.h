/* Simplified access to Microchip MCP251x CAN devices (SPI)
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
#if !defined(__MCP251x_SPI_H__)
#define __MCP251x_SPI_H__

#include <string>
#include <vector>

#include "canMessage.h"

#include "wiringBbb.h" // wiringBbb library

#include "debug.h" // Include mbed header + debug primitives. See DebugLibrary

namespace beagleboneHw {

  namespace spi {

    /** This class provides simplified SPI access to a Microchip MCP251x CAN module. V0.0.0.1
     *
     * Microchip MCP251x CAN device reference: DS21291C
     *
     * Note that for SPI details, please visit http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
     *
     * @remark This class was validated with Tektronix TDS2014 oscilloscope in 3.3V
     * @author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
     */
    class mcp251x_spi { 
      /** Reference counter used to guarentee unicity of the instance of SPI class
       */
      static unsigned char SPIModuleRefCounter;

      /** ChipSelect see DS21291C-page 57 Clause 11 SPI INTERFACE
       */
      unsigned char _cs;
        
      /** Reset state indicator (pin 11), see DS21291C-page 21 Clause 5.5 Reset (RS) Pin Operation
       */
      unsigned char _reset;
        
      /** Interrupt line #1
       */
      unsigned char _isr1;
        
      /** Interrupt line #2
       */
      unsigned char _isr2;
        
      /** An unique instance of SPI class
       */
      int _spiChannel;
        
    public:
      enum canClock : unsigned char {
	    _8MHz = 0x00
	  };
	  enum canSpeed : unsigned char {
	    _500kBPS = 0x00
	  };
	  
    public:
      /** Constructor.
       *
       * @param p_cs  : Pin for Chip Select. If NC, assumes that application manage /CS, default value is NC, not connected
       * @param p_frequency: Frequency of the SPI interface (SCK), default value is 1MHz
       */
      mcp251x_spi(const unsigned char p_rx, const unsigned char p_tx, const unsigned char p_cs = 0xff, const unsigned char p_isr1 = 0xff, const unsigned char p_isr2 = 0xff, const unsigned char p_hwReset = 0xff, const unsigned int p_frequency = 1000000);
    
      /** Destructor
       * If managed, the /CS pin is set to 1 before to release it
       */
      virtual ~mcp251x_spi();
	  
	  /** Initialize the component
	   * @see http://www.bittiming.can-wiki.info/
	   * @see http://www.bittiming.can-wiki.info/
	   */
	  int initialize(const canClock p_clock = _8MHz, const canSpeed p_speed = _500kBPS);
	  
	  inline void softReset() { };
	  void hwReset();
	  
	  int read(canMessage & p_message, const int handle = 0);
	  int write(const canMessage & p_message);
	  
	  int startMonitoring(const unsigned int id, const unsigned int mask, const CANFormat format = CANAny, const int handle = 0);
	  int stopMonitoring(const int handle = 0);
	   
	  int setFilter();
	  
	private:
	  void spiReset();
	  void spiRead(const unsigned char p_register, unsigned char * p_value);
      /** Write one byte into the specified register
	   *  @see DS21291F-page 57 Clause 11.3 Write Instruction
	   */
	  void spiWrite(const unsigned char p_register, const unsigned char p_value);
    }; // End of class mcp251x_spi

  } // End of namespace spi

} // End of namespace beagleboneHw

using namespace beagleboneHw::spi;

#endif // __MCP251x_SPI_H__