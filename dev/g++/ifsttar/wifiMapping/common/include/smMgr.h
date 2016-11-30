/* Raspberry shared memory manager description
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
#if !defined(__SMMGRY_H__)
#define __SMMGR_H__

#include "sharedMemory.h"

namespace wifimapping {

  namespace ipc {

    class smMgr {
      /** sharedMemory object rerefene */
      sharedMemory *_sm;
      /** Physical segment address */
      unsigned char *_address;
      /** Segment identifier */
      int _segmentId;

    public:
      /** Default constructor */
      smMgr();
      /** Destructor */
      virtual ~smMgr();

      /**
       * @desc Create a segmeted shared memory 
       * @return 0 on success, -1 otherwise
       */
      int create();
      /**
       * @desc Open the shared memory and set the physical address to the specified segment
       * @param p_segmentId The segment identifier to be accessed
       * @return 0 on success, -2 for wrong parameters and -1 otherwise
       */
      int open(const int p_segmentId);
      /**
       * @desc Close the openned segmented shared memory
       * @return 0 on success, -1 otherwise
       */
      int close();

      inline unsigned char * getAddress() { return _address; };

      virtual int write(const std::vector<unsigned char> & p_buffer, const int p_length);
      virtual int read(std::vector<unsigned char> & p_buffer, const int p_length);

      std::string toString() const;
    }; // End of class smMgr

  } // End of namespace ipc

} // End of namespace wifimapping

using namespace wifimapping::ipc;

#endif // __SMMGR_H__
