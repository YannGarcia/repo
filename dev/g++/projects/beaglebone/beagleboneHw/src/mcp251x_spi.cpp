/* Simplified access to Microchip MCP251x CAN devices implementation (SPI)
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
#include <iostream>
#include <iomanip>

#include "mcp251x_spi.h"

#include "wiringBbbSPI.h"
#include "beagleboneHwEx.h"

#include "converter.h"

// Commands on SPI
#define  RESET      0xc0 /** Reset internal registers to default state, set conf mode - See DS21291F-page 58 TABLE 11-1: SPI INSTRUCTION SET */
#define  RTS        0x80 /** Trigg transmission - See DS21291F-page 58 TABLE 11-1: SPI INSTRUCTION SET */
#define  RD_STAT    0xA0 /** Start reading status - See DS21291F-page 58 TABLE 11-1: SPI INSTRUCTION SET */
#define  BIT_MOD    0x05 /** Bit modify command data == MASK, BITS - See DS21291F-page 58 TABLE 11-1: SPI INSTRUCTION SET */
#define  READ       0x03 /** Read data from memory - See DS21291F-page 58 TABLE 11-1: SPI INSTRUCTION SET */
#define  WRITE      0x02 /** Write data to memory - See DS21291F-page 58 TABLE 11-1: SPI INSTRUCTION SET */

// Register offsets into the transmit buffers.
#define TXBnCTRL    0 /**  */
#define TXBnSIDH        1
#define TXBnSIDL        2
#define TXBnEID8        3
#define TXBnEID0        4
#define TXBnDLC        5
#define TXBnD0          6
#define TXBnD1          7
#define TXBnD2          8
#define TXBnD3          9
#define TXBnD4          10
#define TXBnD5          11
#define TXBnD6          12
#define TXBnD7          13
#define CANSTAT         14
#define CANCTRL         15

#define SIDH        0
#define SIDL        1
#define EID8        2
#define EID0        3

/*
** Register offsets into the receive buffers.
*/
#define RXBnCTRL        0
#define RXBnSIDH        1
#define RXBnSIDL        2
#define RXBnEID8        3
#define RXBnEID0        4
#define RXBnDLC         5
#define RXBnD0          6
#define RXBnD1          7
#define RXBnD2          8
#define RXBnD3          9
#define RXBnD4          10
#define RXBnD5          11
#define RXBnD6          12
#define RXBnD7          13

/*
** Bits in the TXBnCTRL registers.
*/
#define TXB_TXBUFE_M    0x80
#define TXB_ABTF_M      0x40
#define TXB_MLOA_M      0x20
#define TXB_TXERR_M     0x10
#define TXB_TXREQ_M     0x08
#define TXB_TXIE_M      0x04
#define TXB_TXP10_M     0x03

#define DLC_MASK        0x0F
#define RTR_MASK        0x40


#define TXB0CTRL        0x30
#define TXB0SIDH        0x31

#define TXB1CTRL        0x40
#define TXB1SIDH        0x41

#define TXB2CTRL        0x50
#define TXB2SIDH        0x51

#define TXPRIOHIGH      0x03
#define TXPRIOHIGHLOW   0x02
#define TXPRIOLOWHIGH   0x01
#define TXPRIOLOW       0x00

#define TXB_EXIDE_M     0x08    // In TXBnSIDL
#define TXB_RTR_M       0x40    // In TXBnDLC

#define RXB_IDE_M       0x08    // In RXBnSIDL
#define RXB_RTR_M       0x40    // In RXBnDLC

#define BFPCTRL         0x0C

#define B2RTS           0x20
#define B1RTS           0x10
#define B0RTS           0x08
#define B2RTSM          0x04
#define B1RTSM          0x02
#define B0RTSM          0x01

#define TEC             0x1C
#define REC             0x1D
#define CLKCTRL         CANCTRL

#define RXF0SIDH        0
#define RXF0SIDL        1
#define RXF0EID8        2
#define RXF0EID0        3
#define RXF1SIDH        4
#define RXF1SIDL        5
#define RXF1EID8        6
#define RXF1EID0        7
#define RXF2SIDH        8
#define RXF2SIDL        9
#define RXF2EID8        10
#define RXF2EID0        11

#define RXF3SIDH        16
#define RXF3SIDL        17
#define RXF3EID8        18
#define RXF3EID0        19
#define RXF4SIDH        20
#define RXF4SIDL        21
#define RXF4EID8        22
#define RXF4EID0        23
#define RXF5SIDH        24
#define RXF5SIDL        25
#define RXF5EID8        26
#define RXF5EID0        27

#define RXF_EXIDE_M     0x08

#define RXM0SIDH        0x20
#define RXM1SIDH        0x24
#define CNF3            0x28
#define CNF2            0x29
#define CNF1            0x2A /** DS21291F-page 39 REGISTER 5-1: CNF1 - CONFIGURATION REGISTER1 (ADDRESS: 2Ah) */
#define CANINTE         0x2B
#define CANINTF         0x2C
#define EFLG            0x2D
#define TXRTSCTRL       0x0D

#define EFLG_RX1OVR     0x80
#define EFLG_RX0OVR     0x40
#define EFLG_TXBO       0x20
#define EFLG_TXEP       0x10
#define EFLG_RXEP       0x08
#define EFLG_TXWAR      0x04
#define EFLG_RXWAR      0x02
#define EFLG_EWARN      0x01

#define SJW1            0x00 /** Synchronization Jump Width Length - Length = 1 x TQ */
#define SJW2            0x40 /** Synchronization Jump Width Length - Length = 2 x TQ */
#define SJW3            0x80 /** Synchronization Jump Width Length - Length = 3 x TQ */
#define SJW4            0xC0 /** Synchronization Jump Width Length - Length = 4 x TQ */

#define BTLMODE_CNF3    0x80 /** Phase Segment 2 Bit Time Length */

#define SAMP1           0x00 /** Sample Point Configuration: Bus line is sampled once at the sample point */
#define SAMP3           0x40 /** Sample Point Configuration: Bus line is sampled three times at the sample point */

#define SEG1            0x00
#define SEG2            0x01
#define SEG3            0x02
#define SEG4            0x03
#define SEG5            0x04
#define SEG6            0x05
#define SEG7            0x06
#define SEG8            0x07

#define BRP1            0x00 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 0 */
#define BRP2            0x01 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 1 */
#define BRP3            0x02 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 2 */
#define BRP4            0x03 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 3 */
#define BRP5            0x04 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 4 */
#define BRP6            0x05 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 5 */
#define BRP7            0x06 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 6 */
#define BRP8            0x07 /** Baud Rate Prescaler (TQ = 2 x (BRP + 1) / FOSC) - BRP-1 == 7 */

#define IVRIE           0x80
#define WAKIE           0x40
#define ERRIE           0x20
#define TX2IE           0x10
#define TX1IE           0x08
#define TX0IE           0x04
#define RX1IE           0x02
#define RX0IE           0x01
#define NO_IE           0x00

#define IVRINT          0x80
#define WAKINT          0x40
#define ERRINT          0x20
#define TX2INT          0x10
#define TX1INT          0x08
#define TX0INT          0x04
#define RX1INT          0x02
#define RX0INT          0x01
#define NO_INT          0x00

#define RXB0CTRL        0x60
#define RXB1CTRL        0x70

#define RXB_RXRDY       0x80
#define RXB_RXM1        0x40
#define RXB_RXM0        0x20
#define RXB_RX_ANY      0x60
#define RXB_RX_EXT      0x40
#define RXB_RX_STD      0x20
#define RXB_RX_STDEXT   0x00
#define RXB_RXMx_M      0x60
// #define RXB_RXIE_M      0x10
#define RXB_RXRTR       0x08    // In RXBnCTRL
#define RXB_BUKT        0x04
#define RXB_BUKT_RO     0x02

#define RXB_FILHIT      0x01
#define RXB_FILHIT2     0x04
#define RXB_FILHIT1     0x02
#define RXB_FILHIT_M    0x07
#define RXB_RXF5        0x05
#define RXB_RXF4        0x04
#define RXB_RXF3        0x03
#define RXB_RXF2        0x02
#define RXB_RXF1        0x01
#define RXB_RXF0        0x00

#define CLKEN           0x04

#define CLK1            0x00
#define CLK2            0x01
#define CLK4            0x02
#define CLK8            0x03

#define MODE_NORMAL     0x00
#define MODE_SLEEP      0x20
#define MODE_LOOPBACK   0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG     0xE0
#define ABORT           0x10

#define RECEIVE_BUFFER(x)   (0x60 + 0x10*(x))
#define TRANSMIT_BUFFER(x)  (0x30 + 0x10*(x))

namespace beagleboneHw {

  namespace spi {

    unsigned char mcp251x_spi::SPIModuleRefCounter = 0;

    mcp251x_spi::mcp251x_spi(const unsigned char p_rx, const unsigned char p_tx, const unsigned char p_cs, const unsigned char p_isr1, const unsigned char p_isr2, const unsigned char p_hwReset, const unsigned int p_frequency) : _internalId("") {
      DEBUG_ENTER("mcp251x_spi")

	mcp251x_spi::SPIModuleRefCounter += 1;
      DEBUG_ENTER("mcp251x_spi: refCounter=%d", mcp251x_spi::SPIModuleRefCounter)
	if (mcp251x_spi::SPIModuleRefCounter > 1) {
	  //FIXME Check that SPI settings are identical. Otherwise it should failed
	  return;
	}

      // Create a spi connection
      _spiChannel = 0; // TODO To be enhanced
      if (::wiringBbbSPISetup(_spiChannel, p_frequency) == -1) {
	std::cerr << "Failed: wiringBbbSPISetup: " << strerror(errno) << std::endl;
	throw beagleboneHwEx("mcp251x_spi: wiringBbbPISetup failure");
      }
      // TODO Spi setup is OK
      //    _spiChannel->frequency(p_frequency); // Set the frequency of the SPI interface
      //    _spiChannel->format(16, 0); // See http://mbed.org/users/mbed_official/code/mbed/docs/0954ebd79f59//classmbed_1_1SPI.html

      // TODO Declare wiringBbb node
      
      _cs = p_cs;
      if (p_cs != 0xff) {
	DEBUG("mcp251x_spi: /CS managed");
	pinMode(_cs, OUTPUT);
	::digitalWrite(_cs, HIGH); // Disable chip
      } else {
	DEBUG("mcp251x_spi: /CS not managed");
      }
    
      _reset = p_reset;
      if (p_reset != 0xff) {
	DEBUG("mcp251x_spi: /RESET managed");
	::pinMode(_reset, OUTPUT);
	::digitalWrite(_reset, HIGH); // Disable reset
      } else {
	DEBUG("mcp251x_spi: /RESET not managed");
      }

      _isr1 = p_isr1;
      if (p_isr1 != 0xff) {
	DEBUG("mcp251x_spi: /ISR1 managed");
	::pinMode(_isr1, INPUT);
      } else {
	DEBUG("mcp251x_spi: /ISR1 not managed");
      }
      _isr2 = p_isr2;
      if (p_isr2 != 0xff) {
	DEBUG("mcp251x_spi: /ISR2 managed");
	::pinMode(_isr2, INPUT);
      } else {
	DEBUG("mcp251x_spi: /ISR2 not managed");
      }
        
      DEBUG_LEAVE("mcp251x_spi")
	} // End of Constructor

    mcp251x_spi::~mcp251x_spi() {
      DEBUG_ENTER("~mcp251x_spi")
    
	// Release I2C instance
	DEBUG_ENTER("~mcp251x_spi: refCounter=%d", mcp251x_spi::SPIModuleRefCounter)
	mcp251x_spi::SPIModuleRefCounter -= 1;
      if (mcp251x_spi::SPIModuleRefCounter == 0) {
	close(_spiChannel);
	_spiChannel = -1;
      }
    
      DEBUG_LEAVE("~mcp251x_spi")
	}
  
  	int mcp251x_spi::initialize(const canClock p_clock, const canSpeed p_speed) {
	  // Reset register and set config mode
	  spiReset();
	  
	  unsigned char value = JW0 | BRP1; // Default config: 8MHz/500kBPS => Tq=125ns
	  // Bit timing setup - DS21291F-page 39 Clause 5.10.1 CNF1
	  switch (static_cast<unsigned char>p_clock) {
	    default:
	      break;
	  } // End of 'switch' statement
	  spiWrite(CNF1, value);
	  // Bit timing setup - DS21291F-page 39 Clause 5.10.2 CNF2
      value = (BTLMODE_CNF3 + SEG4 * 8 + SEG7);  // Phase Seg 1 = 4, Prop Seg = 7
	  spiWrite(CNF2, value);
	  // Bit timing setup - DS21291F-page 39 Clause 5.10.3 CNF3
      value = SEG4;  // Phase Seg 1 = 4, Prop Seg = 7
	  spiWrite(CNF3, value);
	  // Disable interrups.
	  value = NO_IE;
	  spiWrite(CANINTE, value);
	}

    void mcp251x_spi::hwReset() {
      if (_reset != 0xff) {
	::digitalWrite(_reset, LOW); // Enable reset
	usleep(1.0);
	::digitalWrite(_reset, HIGH); // Disable reset
      }
	}
	  
    int mcp251x_spi::read(canMessage & p_message, const int handle = 0) {
	  retunr -1;
	}
	
    int mcp251x_spi::write(const canMessage & p_message) {
	  retunr -1;
	}
	  
    int mcp251x_spi::startMonitoring(const unsigned int id, const unsigned int mask, const CANFormat format = CANAny, const int handle = 0) {
	  retunr -1;
	}
	
    int mcp251x_spi::stopMonitoring(const int handle = 0) {
	  retunr -1;
	}
	   
    int mcp251x_spi::setFilter() {
	  retunr -1;
	}
	
	void mcp251x_spi::spiReset() {
      if (_cs != 0xff) {
	digitalWrite(_cs, LOW);
      }
      unsigned char cmd = RESET;
      wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      if (_cs != 0xff) {
	digitalWrite(_cs, HIGH);
      }    
	}

	void mcp251x_spi::spiRead(const unsigned char p_register, unsigned char * p_value) {
	}

	void mcp251x_spi::spiWrite(const unsigned char p_register, const unsigned char p_value) {
      if (_cs != 0xff) {
	digitalWrite(_cs, LOW);
      }
	  // Write register address
      unsigned char cmd = p_register;
      wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
	  // Write register value
      cmd = p_value;
      wiringBbbSPIDataRW(_spiChannel, &cmd, sizeof(unsigned char));
      if (_cs != 0xff) {
	digitalWrite(_cs, HIGH);
      }    
	}

	
  } // End of namespace spi

} // End of namespace beagleboneHw
