/* Beaglebone SharedMemory description
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
#if !defined(__SHAREDMEMORY_H__)
#define __SHAREDMEMORY_H__

#include <iostream>
#include <vector>

#include <sys/shm.h>

namespace beagleboneComm {

  namespace ipc {

    /**
      @desc This class implements POSIX named shared memmory
     */
    class sharedMemory {
    public:
      enum AccessModes : unsigned char {
	ro = 0x00,
        rw = 0x01
      };

    protected:
      int _sm;
      unsigned char *_address;
      struct shmid_ds _ds;
      bool _isCreator;

    public:
      /**
	 @desc Open an existing named shared memory area
      */
      sharedMemory(const int p_key, const int p_size, const int p_offset, const AccessModes p_accessModes);
      /**
	 @desc Create a new named shared memory area
      */
      sharedMemory(const int p_key, const int p_size);
      virtual ~sharedMemory();

      inline unsigned char * getBaseAddress() const { return _address; };

      virtual int close();
      virtual int write(const std::vector<unsigned char> & p_buffer, const int p_length);
      virtual int read(std::vector<unsigned char> & p_buffer, const int p_length);
      virtual std::string toString() const;
    }; // End of class sharedMemory

  } // End of namespace ipc

} // End of namespace beagleboneComm

using namespace beagleboneComm::ipc;

#endif // __SHAREDMEMORY_H__
