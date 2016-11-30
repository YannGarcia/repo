#if !defined(__GPS_DEVICE_UBLOX_H__)
#define __GPS_DEVICES_UBLOX_H__

#include "gpsDevices.h"

namespace beagleboneGps {

  namespace ublox {

    class gpsDeviceUblox : public gpsDevices {

      // TODO If memory shall e saved, use #define instead of const
      const unsigned short MaxUbxMessageLen = 512;
      const unsigned char MaxLen = 40;
      const unsigned char ChecksumStartIdx = 2;
      const unsigned char ChecksumLen = 2;
      const unsigned char ChecksumMaxRangeLen = 38;

      // Sync constants
      const unsigned char Sync1 = 0xb5;
      const unsigned char Sync2 = 0x62;
      // UBX class codes
      const unsigned char ClassCodeNav= 0x01;
      const unsigned char ClassCodeAck= 0x05;
      const unsigned char ClassCodeCfg= 0x06;
      // UBX class Ids
      const unsigned char ClassIdCfgPrt = 0x00;
      const unsigned char ClassIdCfgMsg = 0x01;
      const unsigned char ClassIdCfgRst = 0x04;
      const unsigned char ClassIdCfgNmea = 0x17;
      const unsigned char ClassIdCfgNav5 = 0x24;
      const unsigned char ClassIdAck= 0x01;
      const unsigned char ClassIdNack= 0x00;


      // Communication buffers
      std::vector <unsigned char> _send;
      std::vector <unsigned char> _recv;

      enum States : unsigned char {
	stateSync1 =     0x00,
	stateSync2 =     0x01,
	stateClassCode = 0x02,
	stateClassId =   0x03,
	stateLengthLsb = 0x04,
	stateLengthMsb = 0x05,
	statePayloadLength = 0x06,
	stateChecksum1 = 0x07,
	stateChecksum2 = 0x08,
	stateAckAck = 0xa0,
	stateAckNack = 0xa1,

	stateUndefined = 0xff
      };

      States _ubxDecoderState;
      std::vector<unsigned int> _ubxPayload;
      unsigned short _decodedLength;
      unsigned char _checksum1;
      unsigned char _checksum2;
      unsigned char _recvChecksum1;
      unsigned char _recvChecksum2;
      unsigned char _classCode;
      unsigned char _classId;

    public:
      gpsDeviceUblox();
      virtual ~gpsDeviceUblox();
      
      virtual int initialize(std::unique_ptr<gpsProtocolsAnalyzer> & p_protocol);
      virtual int uninitialize();
      
      virtual void stop();
      
    private:
      virtual void run();
      
    protected:
      virtual void configure();
      virtual int sendWithAck();
      virtual int sendWithoutAck() { return -1; };

      virtual void resetDecoderUbx();
      virtual int decoderUbxMessage(const States p_expectedState, const unsigned char _expectedClassCode, const unsigned char _expectedClassId);
      virtual int decoderUbxMessagePayload(const States p_expectedState, const unsigned char p_expectedClassCode, const unsigned char p_expectedClassId);
      
      /*      inline virtual void setHeader() { 
	_send.clear(); 
	_send.push_back(Sync1); 
	_send.push_back(Sync2); 
      };
      inline virtual void setUart() { 
	_send.push_back(ClassCodeCfg); 
	_send.push_back(ClassIdCfgPrt);
	_send.push_back(0x14); // Set length to 20 - See GPS.G6-SW-10018-D Clause 31.16.3
	_send.push_back(0x00);
	_send.push_back(0x01); // UART port
	_send.push_back(0x00); // Reserved 
	_send.push_back(0x00); // txReady
	_send.push_back(0x00);
	_send.push_back(0xD0); // mode
	_send.push_back(0x08);
	_send.push_back(0x00);
	_send.push_back(0x00);
	_send.push_back(0x80); // Baurate
	_send.push_back(0x25);
	_send.push_back(0x00);
	_send.push_back(0x00);
	_send.push_back(0x03); // inProtoMask
	_send.push_back(0x00);
	_send.push_back(0x03); // outProtoMask
	_send.push_back(0x00);
	_send.push_back(0x00); // Reserved
	_send.push_back(0x00);
	_send.push_back(0x00); // Reserved
	_send.push_back(0x00);
      };
      inline virtual void setCfgNav5() { 
	_send.push_back(ClassCodeCfg); 
	_send.push_back(ClassIdCfgNav5); 
	_send.push_back(0x24); // Set length to 26 - See GPS.G6-SW-10018-D Clause 31.10.2
	_send.push_back(0x00);
	_send.push_back(0x00); // mask : dynamic model
	_send.push_back(0x01);
	_send.push_back(0x04); // dynModel: automotive
	_send.push_back(0x03); // fixMode: auto 2D/3D
	_send.push_back(0x00); // fixedAlt
	_send.push_back(0x00); 
	_send.push_back(0x00);
	_send.push_back(0x00);
	_send.push_back(0x10);// fixedAltVar
	_send.push_back(0x27);
	_send.push_back(0x00);
	_send.push_back(0x00);
	_send.push_back(0x05); // minElev
	_send.push_back(0x00); // drLimit
	_send.push_back(0xFA); // pDop
	_send.push_back(0x00);
	_send.push_back(0xFA); // tDop
	_send.push_back(0x00);
	_send.push_back(0x64); // pAcc
	_send.push_back(0x00);
	_send.push_back(0x2C); // tAcc
	_send.push_back(0x01);
	_send.push_back(0x00); // staticHoldThresh
	_send.push_back(0x00); // dgpsTimeout
	_send.push_back(0x00);
	_send.push_back(0x00);
      };
      inline virtual void setNmeaFiltering() { 
	_send.push_back(ClassCodeCfg); 
	_send.push_back(ClassIdCfgNmea); 
	_send.push_back(0x04); // Set length to 4 - See GPS.G6-SW-10018-D Clause 31.
	_send.push_back(0x00);
	_send.push_back(0x00); // filter
	_send.push_back(0x23); // version : NMEA v2.3
	_send.push_back(0x00); // numSV
	_send.push_back(0x00); // flags
      };
      inline virtual void setStopGps() { 
	_send.push_back(ClassCodeCfg); 
	_send.push_back(ClassIdCfgRst);
	_send.push_back(0x04); // Set length to 4 - See GPS.G6-SW-10018-D Clause 31.
	_send.push_back(0x00);
	_send.push_back(0x00); 
	_send.push_back(0x08); // resetMode: Controler GPS stop
	_send.push_back(0x00); 
      };
      inline virtual void setStartGps() { 
	_send.push_back(ClassCodeCfg); 
	_send.push_back(ClassIdCfgRst); 
	_send.push_back(0x04); // Set length to 4 - See GPS.G6-SW-10018-D Clause 31.
	_send.push_back(0x00);
	_send.push_back(0x00);
	_send.push_back(0x00); 
	_send.push_back(0x09); // resetMode: Controller GPS start 
	_send.push_back(0x00); 
      };
      */

      virtual void setChecksum();
    }; // End of class gpsDeviceUblox
    
  } // End of namespace ublox

} // End of namespace beagleboneGps

using namespace beagleboneGps::ublox;

#endif // __GPS_DEVICES_UBLOX_H__
