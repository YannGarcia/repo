/* Raspberry Debug library used by all my developed program
 * Copyright (c) 2010-2012 ygarcia, MIT License
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
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include "debug.h"

#ifdef _DEBUG

#define __LINE_LENGTH__ 93

void debugHelper::debug(const char* p_format, ...) {
  va_list argp;
  
  va_start(argp, p_format);
  vprintf(p_format, argp);
  va_end(argp);
} // End of method debugHelper::debug

void debugHelper::breakPoint(const char* p_file, int p_line) {
  printf("Stop in %s at line %d\r\n", p_file, p_line);
  fflush(stdout);
  getchar();
  fflush(stdin);
} // End of method debugHelper::breakPoint

#endif // _DEBUG
