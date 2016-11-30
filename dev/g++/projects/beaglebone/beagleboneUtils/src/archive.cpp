/* Raspberry data ONLY serialization implementation
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
#include "archive.h"
 
namespace beagleboneUtils {
 
  namespace serialization {
 
    int archive::getTypeSize(const archive::arType p_type) {
      int size;
      switch(p_type) {
      case archive::arBoolean:
	size = 1;
	break;
      case archive::arShort:
	size = sizeof(short);
	break;
      case archive::arInteger:
	size = sizeof(int);
	break;
      case archive::arFloat:
	size = sizeof(float);
	break;
      case archive::arTime_t:
	size = sizeof(time_t);
	break;
      case archive::arLong:
	size = sizeof(long);
	break;
      case archive::arCharacter:
	size = 1;
	break;
      } // End of 'switch' statement
 
      return size;
    } // End of method getTypeSize
 
  } // End of namespace serialization
 
} // End of namespace beagleboneUtils
