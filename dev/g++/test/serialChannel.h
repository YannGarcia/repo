#if !defined(__SERIALCHANNEL_H__)
#define __SERIALCHANNEL_H__

#include "abstractChannel.h"

namespace raspberryComm {

  namespace serial{

    class serialChannel : public abstractChannel {
      int _fd;

    public:
      serialChannel(const std::string & p_device, const int speed);
      virtual ~serialChannel();

      int connect();

      int disconnect() { return -1; };

      int write(const std::vector<unsigned char> & p_buffer);
      int read(std::vector<unsigned char> & p_buffer);
      char read();

    }; // End of class serialChannel

  } // End of namespace serial

} // End of namespace raspberryComm

#endif // __SERIALCHANNEL_H__
