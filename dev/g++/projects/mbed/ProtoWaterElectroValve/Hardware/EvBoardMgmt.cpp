#include <iostream>
#include <sstream>

#include "EvBoardMgmt.h"

#include "Debug.h"

namespace EvBoardMgmt {

    EvBoardMgmt * EvBoardMgmt::Instance = NULL;
    
    EvBoardMgmt::EvBoardMgmt(const PinName p_sipoClock, const PinName p_sipoData, const unsigned char p_muxEnable, const unsigned char p_muxAddr0, const unsigned char p_muxAddr1, const unsigned char p_muxAddr2) :
        _sipoClock(p_sipoClock), 
        _sipoData(p_sipoData), 
        _muxEnable(p_muxEnable),
        _muxAddr0(p_muxAddr0), 
        _muxAddr1(p_muxAddr1), 
        _muxAddr2(p_muxAddr2),
        _portStatus()
    {
    }
    
    int EvBoardMgmt::startEv(const unsigned char p_boardNum, const unsigned char p_portNum) {
        unsigned char portNum = 0x00;
        
        // Check if board is declared
        std::map<unsigned char, unsigned char>::iterator result = _portStatus.find(p_boardNum);
        if (result == _portStatus.end()) {
            _portStatus.insert(std::pair<unsigned char, unsigned char>(p_boardNum, portNum));
        } else {
            portNum = result->second;
        }
        
        // Check if port is already started
        if ((portNum & p_portNum) == p_portNum) {
            return -1; // Port already started
        }
        
        // Set the port bit
        portNum |= p_portNum;
        _portStatus[p_boardNum] = portNum;
        
        // Start port(s)
        flushData(p_boardNum, portNum);
        
        return 0;        
    }
    
    int EvBoardMgmt::stopEv(const unsigned char p_boardNum, const unsigned char p_portNum) {
        // Sanity check
        std::map<unsigned char, unsigned char>::iterator result = _portStatus.find(p_boardNum);
        if (result == _portStatus.end()) {
            return -1; // Unknown board
        }
        
        // Check if port is already stopped
        if ((result->second ^ p_portNum) == 0) {
            return -2; // Port already stopped
        }
        
        // Set the port bit
        result->second ^= p_portNum;
        
        // Stop port(s)
        flushData(p_boardNum, result->second);
        
        return 0;        
    }
    
    bool EvBoardMgmt::isEvStarted(const unsigned char p_boardNum, const unsigned char p_portNum) {
        return false;
    }
    
    void EvBoardMgmt::flushData(const unsigned char p_board, const unsigned char p_ports) {
        // TODO Enable the board
        // TODO Enable MUX
        
        for (unsigned char i = 0; i < 8; i++) {
            // Set bit
            unsigned char bit2transmit = p_ports << 1;
            _sipoData.write(bit2transmit); // TODO Simplify: _sipoData.write(portNum << 1)
            // Validate bit
            _sipoClock.write(1);
            wait_us(1);
            _sipoClock.write(0);
        } // End of 'for' statement
        
        // TODO Disable MUX

    }        
} // End of namespace EvBoardMgmt
