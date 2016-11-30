/* Raspberry Abstract channel description
 * Copyright (c) 2013 ygarcia, MIT License
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
#if !defined(__ABSTRACTCHANNEL_H__)
#define __ABSTRACTCHANNEL_H__

#include <iostream>
#include <vector>
#include "socket.h"

namespace raspberryComm {

  class abstractChannel {
  public:
    enum ChannelType : unsigned char {
      udp = 0x00,
      tcp = 0x01,
      sctp = 0x02,
      raw = 0x03,
      serial = 0x04
    };

  protected:
    network::socket *_socket;

  public:
    abstractChannel() { _socket = NULL; }
    virtual ~abstractChannel() { 
      std::clog << "abstractChannel::~abstractChannel" << std::endl; 
      if (_socket != NULL) {
	delete _socket;
	_socket = NULL;
      }
    }

    virtual int connect() = 0;
    virtual int disconnect() = 0;
    virtual int write(const std::vector<unsigned char> & p_buffer) = 0;
    virtual int read(std::vector<unsigned char> & p_buffer) = 0;
    virtual char read() = 0;
  }; // End of class abstractChannel

} // End of namespace raspberryComm

#endif // __ABSTRACTCHANNEL_H__
