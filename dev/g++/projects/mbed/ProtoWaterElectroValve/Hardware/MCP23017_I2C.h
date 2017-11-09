/* mbed simplified access to Microchip MCP28x17 GPIO expender devices (I2C)
 * Copyright (c) 2014-2015 ygarcia, MIT License
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

#include "Debug.h"

#include "AbstractGpioExpender.h"

namespace MCP23017_I2C {
    /** This class provides simplified I2C access to a Microchip 2MCP28x17 GPIO expender. V0.0.0.1
     *
     * Note that if the LPC1768 is powered in 3.3V and Microchip MCP28x17 GPIO expender device could be powered at 3.3V or 5V.
     * In this case, you shall use a bi-directional level shifter for I2C-bus. Please refer to AN97055 (http://ics.nxp.com/support/documents/interface/pdf/an97055.pdf)
     * Microchip MCP28x17 GPIO expender device reference: DS21952B
     *
     * Note that for I2C details, please visit http://www.datelec.fr/fiches/I2C.htm
     *
     * Note that this header file include following headers:
     * - <string>
     * - <vector>
     * - <mbed.h>
     *
     * \author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
     */
    class CMCP23017_I2C : public AbstractGpioExpender {
        /** Reference counter used to guarentee unicity of the instance of I2C class
         */
        static unsigned char I2CModuleRefCounter;
        
        /** Device address input: A0, A1, A2 (Pins <1,3>). See DS21203K/DS21189D - Figure 5-1: Control Byte Format for address format details
         */
        unsigned char _slaveAddress;
        /** Interrupt on GPIOA
         */
        InterruptIn *_intA;
        /** Interrupt on GPIOB
         */
        InterruptIn *_intB;
        /** Device reset pin
         */
        DigitalOut *_reset;
        /** An unique instance of I2C class
         */
        I2C *_i2cInstance;
    public:
        /** Constructor with Write Protect command pin wired. Use it to manage the first I2C module on 3.3V or 5V network
         *
         * \param p_sda: MBed pin for SDA
         * \param p_scl: MBed pin for SCL
         * \param p_address: Device address input: A0, A1, A2 (Pins <1,3>)
         * \param p_intA: MBed pin to manage interrupt on GPIOA, default value is NC, not connected
         * \param p_intB: MBed pin to manage device reset. If NC, WP is not managed, default value is NC, not connected
         * \param p_reset: MBed pin to manage Write Protect input. If NC, WP is not managed, default value is NC, not connected
         * \param p_internalPullUp: Set to true to use internal pull-up resistor, default value is true
         * \param p_frequency: Frequency of the I2C interface (SCL), default value is 400KHz
         * Example:
         * - If A1 and A2 pins are tired to Vdd and A0 is tired to Vss, address shall '00000110'B
         * - If A0 and A1 pins are tired to Vss and A2 is tired to Vdd, address shall '00000100'B
         */
        CMCP23017_I2C(const PinName p_sda, const PinName p_scl, const unsigned char p_address, const PinName p_intA = NC, const PinName p_intB = NC, const PinName p_reset = NC, const bool p_internalPullUp = true, const unsigned int p_frequency = 400000);
    
        /** Destructor
         */
        virtual ~CMCP23017_I2C();

        /** Used to return the unique instance of I2C instance
         */
        inline const I2C * operator * () { return (const I2C *)_i2cInstance; };
        
        /** Reset the device
         */
        void reset();
        
        virtual int read(const unsigned char p_portId);
        virtual int write(const unsigned char p_portId, const unsigned char p_value);
        
        virtual unsigned char createBus(const std::list<unsigned char> p_lines, const PinMode p_mode = PullNone);
        virtual void deleteBus(const unsigned char p_busId);
        virtual int busRead(const unsigned char p_busId);
        virtual int busWrite(const unsigned char p_busId, const unsigned char p_value);
        
    private:
    
       /** Configure the device:
        *   BANK0 (A register followed by B register)
        *   INTs: active low
        *   INTs for all ports
        *   INTs are not mirrored
        */
       void configure();
       
       bool _write(const unsigned char p_address, const unsigned char p_byte);
        
    private:
        /** Internal reference identifier
         */
        std::string _internalId;

    }; // End of class CMCP23017_I2C

} // End of namespace _MCP23017_I2C

using namespace MCP23017_I2C;
