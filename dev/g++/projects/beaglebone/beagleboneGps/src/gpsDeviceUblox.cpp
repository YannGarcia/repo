
#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "gpsDeviceUblox.h"

#include "channelManager.h"

#include "beagleboneGpsEx.h"

#include "dump.h" // TODO To be removed

namespace beagleboneGps {

  namespace ublox {

    gpsDeviceUblox::gpsDeviceUblox() : _send(), _recv() {
      std::clog << ">>> gpsDeviceUblox::gpsDeviceUblox" << std::endl;

      resetDecoderUbx();
    } // End of Constructor
    
    gpsDeviceUblox::~gpsDeviceUblox() { 
      std::clog << ">>> gpsDeviceUblox::~gpsDeviceUblox" << std::endl;
      uninitialize();
    } // End of Destructor

    int gpsDeviceUblox::initialize(std::unique_ptr<gpsProtocolsAnalyzer> & p_protocol) { 
      std::clog << ">>> gpsDeviceUblox::initialize" << std::endl;
      
      // Create access to gps
      _gps = beagleboneComm::channelManager::getInstance().createChannel(abstractChannel::serial, "/dev/ttyO2", 9600); // TODO Parameters from command line
      
      // Create instance of NMEA decoder
      _protocol = p_protocol.get();
      
      // Configure the device
      configure();

      return 0;
    }
    
    int gpsDeviceUblox::uninitialize() { 
      std::clog << ">>> gpsDeviceUblox::uninitialize" << std::endl;

      beagleboneComm::channelManager::getInstance().removeChannel(_gps);
      return 0;
    }
    
    void gpsDeviceUblox::stop() { 
      std::clog << ">>> gpsDeviceUblox::stop" << std::endl;
      runnable::stop();
    }
    
    void gpsDeviceUblox::run() { 
      std::clog << ">>> gpsDeviceUblox::run" << std::endl;
      
      _running = true;
      int result;
      while (_running) {
        // Read available data
        if (beagleboneComm::channelManager::getInstance().getChannel(_gps).dataAvailable() != 0) {
          std::vector<unsigned char> buffer(beagleboneComm::channelManager::getInstance().getChannel(_gps).dataAvailable());
          if ((result  = beagleboneComm::channelManager::getInstance().getChannel(_gps).read(buffer)) != -1) {
	    //            std::clog << "gpsDeviceUblox::run: # of byte read: " << (int)buffer.size() << std::endl;
            // Process it
            if ((result = _protocol->process(buffer)) != 0) {
	      // Display error message
	      //std::cerr << "gpsDeviceUblox::run: Failed to process GPS data" << std::endl;
	      usleep(400);
	    }
	  } // else, display error message
	} else { // No data available
          usleep(400);
        }
      } // End of 'while' statement
      
      std::clog << "<<< gpsDeviceUblox::run" << std::endl;
    }
    
    void gpsDeviceUblox::configure() {
      std::clog << ">>> gpsDeviceUblox::configure" << std::endl;

      std::clog << "gpsDeviceUblox::configure: Set frequency to 1Hz" << std::endl;
      std::string init("$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"); // Set frequency to 1Hz
      beagleboneComm::channelManager::getInstance().getChannel(_gps).write(_send);
      std::clog << "gpsDeviceUblox::sendWithAck: Turn on all the available data " << std::endl;
      init.assign("$PMTK220,1000*1F"); // Turn on all the available data
      beagleboneComm::channelManager::getInstance().getChannel(_gps).write(_send);

      /*
      unsigned char buffer0[] = { 0xb5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x24, 0x32, 0x5b };
      unsigned char buffer1[] = { 0x24, 0x47, 0x50, 0x54, 0x58, 0x54, 0x2C, 0x30, 0x31, 0x2C, 0x30, 0x31, 0x2C, 0x30, 0x32, 0x2C, 0x53, 0x74, 0x6F, 0x70, 0x70, 0x69, 0x6E, 0x67, 0x20, 0x47, 0x50, 0x53, 0x2A, 0x30, 0x31, 0x0D, 0x0A };
      unsigned char buffer2[] = { 0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x04, 0x12, 0x3B };
      std::vector<std::vector<unsigned char>> buffers(0, std::vector<unsigned char>());
      buffers.push_back(std::vector<unsigned char>(buffer0, buffer0 + sizeof(buffer0) / sizeof(unsigned char)));
      buffers.push_back(std::vector<unsigned char>(buffer1, buffer1 + sizeof(buffer1) / sizeof(unsigned char)));
      buffers.push_back(std::vector<unsigned char>(buffer2, buffer2 + sizeof(buffer2) / sizeof(unsigned char)));

      for (auto it = buffers.cbegin(); it != buffers.cend(); ++it) {
	std::vector<unsigned char> v = *it;
	_recv.assign(v.begin(), v.end());
	decoderUbxMessage(gpsDeviceUblox::stateAckAck, buffer0[6], buffer0[7]);
      } // End of 'for' statement
      */
      /*
      // Set UART, 9600 8n1
      setHeader();
      setUart();
      setChecksum();
      sendWithAck();
      // Stop GPS
      setHeader();
      setStartGps();
      setChecksum();
      sendWithAck();
      // Set Cfg-Nav5
      setHeader();
      setCfgNav5();
      setChecksum();
      sendWithAck();
      // Set NMEA message filtering
      setHeader();
      setNmeaFiltering();
      setChecksum();
      sendWithAck();
      // Start GPS
      setHeader();
      setStartGps();
      setChecksum();
      sendWithAck();
      */
      std::clog << "<<< gpsDeviceUblox::configure" << std::endl;
   }

    void gpsDeviceUblox::setChecksum() {
      unsigned char chk1 = 0x00, chk2 = 0x00;

      for (unsigned char i = ChecksumStartIdx; i < _send.size(); i++) {
        chk1 += _send[i];
        chk2 += chk1;
      } // End of 'for' statement
      _send.push_back(chk1);
      _send.push_back(chk2);
    }

    int gpsDeviceUblox::sendWithAck() {
      std::clog << ">>> gpsDeviceUblox::sendWithAck" << std::endl;

      std::clog << "gpsDeviceUblox::sendWithAck: send packet: " << (int)_send.size() << std::endl;
      dump::hexaDump(_send);
      beagleboneComm::channelManager::getInstance().getChannel(_gps).write(_send);
      do {
	if (beagleboneComm::channelManager::getInstance().getChannel(_gps).dataAvailable() != 0) {
	  _recv.assign(beagleboneComm::channelManager::getInstance().getChannel(_gps).dataAvailable(), 0x00);
	  beagleboneComm::channelManager::getInstance().getChannel(_gps).read(_recv);
	  std::clog << "gpsDeviceUblox::sendWithAck: received packet: " << std::endl;
	  dump::hexaDump(_recv);
	  if (decoderUbxMessage(gpsDeviceUblox::stateAckAck, _send[2], _send[3]) == 0) {
	    return 0;
	  }
	} else {
	  usleep(500);
	}
      } while (true);

      std::clog << "<<< gpsDeviceUblox::sendWithAck: -1" << std::endl;
      return -1;
    }

    int gpsDeviceUblox::decoderUbxMessage(const States p_expectedState, const unsigned char _expectedClassCode, const unsigned char _expectedClassId) {
      //      std::clog << ">>> gpsDeviceUblox::decoderUbxMessage" << std::endl;

      // Sanity checks
      if (_recv.size() == 0) {
	std::cerr << "<<< gpsDeviceUblox::decoderUbxMessage: No data available" << std::endl;
	return -1;
      }

      do {
	//	std::clog << "gpsDeviceUblox::decoderUbxMessage: " << std::hex << std::setw(2) << std::setfill('0') << (int)_ubxDecoderState << " - " << (int)_classCode << " - " << (int)_classId << std::endl;
	switch (_ubxDecoderState) {
	case gpsDeviceUblox::stateSync1:
	  if (_recv.front() == Sync1) {
	    std::clog << "gpsDeviceUblox::decoderUbxMessage: Sync1 received" << std::endl;
	    _ubxDecoderState = gpsDeviceUblox::stateSync2;
	  }
	  break;
	case gpsDeviceUblox::stateSync2:
	  if (_recv.front() == Sync2) {
	    std::clog << "gpsDeviceUblox::decoderUbxMessage: Sync2 received" << std::endl;
	    _ubxDecoderState = gpsDeviceUblox::stateClassCode;
	  } else {
	    resetDecoderUbx();
	  }
	  break;
	case gpsDeviceUblox::stateClassCode:
	  _classCode = _recv.front();
	  _checksum1 += _classCode;
	  _checksum2 += _checksum1;
	  _ubxDecoderState = gpsDeviceUblox::stateClassId;
	  std::clog << "gpsDeviceUblox::decoderUbxMessage: " << std::hex << std::setw(2) << std::setfill('0') << (int)_classCode << std::endl;
	  break;
	case gpsDeviceUblox::stateClassId:
	  _classId = _recv.front();
	  _checksum1 += _classId;
	  _checksum2 += _checksum1;
	  _ubxDecoderState = gpsDeviceUblox::stateLengthLsb;
	  std::clog << "gpsDeviceUblox::decoderUbxMessage: " << std::hex << std::setw(2) << std::setfill('0') << (int)_classId << std::endl;
	  break;
	case gpsDeviceUblox::stateLengthLsb:
	  _decodedLength = _recv.front();
	  _checksum1 += _recv.front();
	  _checksum2 += _checksum1;
	  _ubxDecoderState = gpsDeviceUblox::stateLengthMsb;
	  break;
	case gpsDeviceUblox::stateLengthMsb:
	  _decodedLength |= (_recv.front() << 8);
	  _checksum1 += _recv.front();
	  _checksum2 += _checksum1;
	  _ubxDecoderState = gpsDeviceUblox::statePayloadLength;
	  _ubxPayload.clear();
	  if (_decodedLength > MaxUbxMessageLen) {
	    resetDecoderUbx();
	  }
	  break;
	case gpsDeviceUblox::statePayloadLength:
	  _ubxPayload.push_back(_recv.front()); // Fill ubxPayload vector (UBX payload)
	  _checksum1 += _recv.front();
	  _checksum2 += _checksum1;
	  if (_ubxPayload.size() == _decodedLength) { // All payload bytes were read
	    _ubxDecoderState = gpsDeviceUblox::stateChecksum1;
	  }
	  break;
	case gpsDeviceUblox::stateChecksum1:
	  _recvChecksum1 = _recv.front();
	  _ubxDecoderState = gpsDeviceUblox::stateChecksum2;
	  break;
	case gpsDeviceUblox::stateChecksum2:
	  _recvChecksum2 = _recv.front();
	  if ((_checksum1 != _recvChecksum1) || (_checksum2 != _recvChecksum2)) { // Checksum mismatch
	    resetDecoderUbx();
	  } else { // Process the payload
	    int result = decoderUbxMessagePayload(p_expectedState, _expectedClassCode, _expectedClassId);
	    resetDecoderUbx();

	    std::clog << "<<< gpsDeviceUblox::decoderUbxMessage: " << result << std::endl;
	    return result; // 0 on success, -1 otherwise
	  }
	  break;
	default: // Reset decoder
	  resetDecoderUbx();
	} // End of 'switch' statement
	// Remove byte
	_recv.erase(_recv.begin(), _recv.begin() + 1);
      } while ((_recv.size()) != 0 && (_ubxDecoderState != p_expectedState));

      //      std::clog << "<<< gpsDeviceUblox::decoderUbxMessage: -2" << std::endl;
      return -2; // Decoding in progress
    } // End of method decoderUbxMessage

    void gpsDeviceUblox::resetDecoderUbx() {
	_ubxDecoderState = gpsDeviceUblox::stateSync1;
	_decodedLength = 0;
	_checksum1 = 0x00;
	_checksum2 = 0x00;
	_recvChecksum1 = 0x00;
	_recvChecksum2 = 0x00;
	_classCode = 0xff;
	_classId = 0xff;
    } // End of method resetDecoderUbx

    int gpsDeviceUblox::decoderUbxMessagePayload(const States p_expectedState, const unsigned char p_expectedClassCode, const unsigned char p_expectedClassId) {
      int result = -1; // FIXME Process p_expectedClassCode and p_expectedClassId
      switch (_classCode) {
      case 0x05:
	if (_classId == ClassIdAck) {
	  _ubxDecoderState = gpsDeviceUblox::stateAckAck;
	} else {
	  _ubxDecoderState = gpsDeviceUblox::stateAckNack;
	}
	break;
      default:
	  _ubxDecoderState = gpsDeviceUblox::stateUndefined;
	break;
      } // End of 'swith' statement

      if (_ubxDecoderState == p_expectedState) {
	result = 0;
      } 

      return result;
    } // End of method decoderUbxMessagePayload

  } // End of namespace ublox

} // End of namespace beagleboneGps
