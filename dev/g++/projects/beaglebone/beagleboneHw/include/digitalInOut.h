/* Raspberry PI simplified access to digital port in input/output mode
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
#if !defined(__DIGITAL_INOUT_H__)
#define __DIGITAL_INOUT_H__

#include "wiringBbb.h"

namespace beagleboneHw {

  namespace digitalInOut {

    class digitalInOut {
      unsigned char _port;
      unsigned char _value;
      unsigned char _pullMode;

    public:
      digitalInOut(const unsigned char p_port) {
	_port = p_port;
	::pinMode(_port, OUTPUT);
	_pullMode = PUD_OFF;
	::pullUpDnControl(_port, _pullMode);
	_value = 0x00;
      }; // End of Constructor

      virtual ~digitalInOut() {
	::pinMode(_port, INPUT);
      }; // End of Destructor

      inline unsigned char read() {
	::pinMode(_port, INPUT);
	_value = static_cast<unsigned char>(::digitalRead(_port));
	return _value;
      };

      inline void write(const unsigned char p_value) {
	::pinMode(_port, OUTPUT);
	if (p_value == 0) {
	  _value = LOW;
	} else if (p_value == 1) {
	  _value = HIGH;
	} // else, nothing to do
	::digitalWrite(_port, static_cast<int>(_value));
      };

      inline void mode(const unsigned char p_pullMode) {
	::pinMode(_port, INPUT);
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

    }; // End of class digitalInOut

  } // End of namespace digitalInOut

} // End of namespace beagleboneHw

using namespace beagleboneHw::digitalInOut;

#endif //  __DIGITAL_INOUT_H__
