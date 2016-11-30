#if !defined(__SERIALCHANNEL_H__)
#define __SERIALCHANNEL_H__

#include "abstractChannel.h"

#include "wiringBbb.h"
#include "wiringSerial.h"

namespace beagleboneComm {

  namespace serial{

    class serialChannel : public abstractChannel {
      int _fd;

    public:
      serialChannel(const std::string & p_device, const int p_speed);
      virtual ~serialChannel();

      int connect();

      int disconnect() { return -1; };

      int write(const std::string & p_string);
      int write(const std::vector<unsigned char> & p_buffer);
      int read(std::string & p_line);
      int read(std::vector<unsigned char> & p_buffer);
      char read();

      inline int dataAvailable() { return ::serialDataAvail(_fd); };

      inline virtual const int getFd() const { return _fd; };
      
    }; // End of class serialChannel

  } // End of namespace serial

} // End of namespace beagleboneComm

using namespace beagleboneComm::serial;

#endif // __SERIALCHANNEL_H__
