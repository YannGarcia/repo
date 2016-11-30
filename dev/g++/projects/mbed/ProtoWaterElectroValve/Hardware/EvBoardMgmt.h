/* mbed simplified access to Electrovalve board
 * Copyright (c) 2010-2012 ygarcia, MIT License
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

#include <map>

#include "mbed.h"

#include "HwConfig.h"

namespace EvBoardMgmt {

    class EvBoardMgmt { 
        DigitalOut _sipoClock;
        DigitalOut _sipoData;
        unsigned char _muxEnable;
        unsigned char _muxAddr0; 
        unsigned char _muxAddr1; 
        unsigned char _muxAddr2;
        std::map<unsigned char, unsigned char> _portStatus; 
        
        static EvBoardMgmt * Instance;
    
        EvBoardMgmt(const PinName p_sipoClock, const PinName p_sipoData, const unsigned char p_muxEnable, const unsigned char p_muxAddr0, const unsigned char p_muxAddr1, const unsigned char p_muxAddr2);
        ~EvBoardMgmt() {
            if (Instance != NULL) {
                delete Instance;
                Instance = NULL;
            }
        };
    
    public:
        inline static EvBoardMgmt & getInstance() {
            if (Instance == NULL) {
                Instance = new EvBoardMgmt(
                    SIPO_CLOCK,
                    SIPO_DATA,
                    MUX_EN,
                    MUX_ADDR0,
                    MUX_ADDR1,
                    MUX_ADDR2
                );
            }
            return *Instance;
        };
    
        int startEv(const unsigned char p_boardNum, const unsigned char p_portNum); 
        int stopEv(const unsigned char p_boardNum, const unsigned char p_portNum); 
        bool isEvStarted(const unsigned char p_boardNum, const unsigned char p_portNum); 
        
    private:
        void flushData(const unsigned char p_board, const unsigned char p_ports);
        
    }; // End of class EvBoardMgmt


} // End of namespace _24LCXX_I2C

using namespace EvBoardMgmt;
