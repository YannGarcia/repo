/* Raspberry PI simplified access to digital port in input mode
 * Copyright (c) 2014 ygarcia, MIT License
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
#if !defined(__DIGITAL_IN_H__)
#define __DIGITAL_IN_H__

#include "wiringBbb.h"

namespace beagleboneHw {

  namespace digitalInOut {

    class digitalIn {
      unsigned char _port;
      unsigned char _pullMode;

    public:
      digitalIn(const unsigned char p_port) {
	_port = p_port;
	::pinMode(_port, INPUT);
	_pullMode = PUD_OFF;
	::pullUpDnControl(_port, _pullMode);
      }; // End of Constructor

      virtual ~digitalIn() {
      }; // End of Destructor

      inline unsigned char read() {
	return static_cast<unsigned char>(::digitalRead(_port));
      };

      inline void mode(const unsigned char p_pullMode) {
	if (p_pullMode == 0) {
	  _pullMode = PUD_OFF;
	} else if (p_pullMode == 1) {
	  _pullMode = PUD_UP;
	} else if (p_pullMode == 2) {
	  _pullMode = PUD_DOWN;
	} // else, nothing to do 
	::pullUpDnControl(_port, _pullMode);
      };

      inline const unsigned char getPort() const { return _port; };

    }; // End of class digitalIn

  } // End of namespace digitalInOut

} // End of namespace beagleboneHw

using namespace beagleboneHw::digitalInOut;

#endif //  __DIGITAL_IN_H__
