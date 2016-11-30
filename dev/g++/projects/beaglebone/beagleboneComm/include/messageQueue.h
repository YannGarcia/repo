/* Beaglebone MessageQueue description
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
#if !defined(__MESSAGEQUEUE_H__)
#define __MESSAGEQUEUE_H__

#include <iostream>
#include <vector>

#include <sys/msg.h>

namespace beagleboneComm {

  namespace ipc {

    /**
      @desc This class implements POSIX named shared memmory
     */
    class messageQueue {
      long _msgType; /** Message type, must be > 0 */

    public:
      enum AccessModes : unsigned char {
	ro = 0x00,
        rw = 0x01
      };

    protected:
      int _mq;
      struct msqid_ds _ds;
      bool _isCreator;

    public:
      /**
	 @desc Open an existing message queue area
      */
      messageQueue(const int p_key, const long p_msgType, const messageQueue::AccessModes p_accessModes);
      /**
	 @desc Create a new message queue area
      */
      messageQueue(const int p_key, const long p_msgType);
      virtual ~messageQueue();

      virtual int close();
      virtual int send(const std::vector<unsigned char> & p_buffer, const int p_length);
      virtual int recv(std::vector<unsigned char> & p_buffer, const int p_length);
      virtual std::string toString() const;
    }; // End of class messageQueue

  } // End of namespace ipc

} // End of namespace beagleboneComm

using namespace beagleboneComm::ipc;

#endif // __MESSAGEQUEUE_H__
