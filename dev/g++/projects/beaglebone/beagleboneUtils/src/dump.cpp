/* Raspberry Dump packet used by all my developed program
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
#include "dump.h"

#define __LINE_LENGTH__ 93

 void dump::hexaDump(const unsigned char* p_buffer, int p_count, int p_offset) {

  int currentIdx = p_offset;
  unsigned short startAddress = ((unsigned short)(p_offset / 16)) * 16;

  // Display header
  printf(" HEX | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  : 0 1 2 3 4 5 6 7 8 9 A B C D E F \r\n");
  printf("-----|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-:--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\r\n");
  //      01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901 2
  //      0         1         2         3         4         5         6         7         8         9
  // Address offset padding
  char line[__LINE_LENGTH__ + 1];
  memset(line, 0x20, __LINE_LENGTH__);
  line[__LINE_LENGTH__] = 0x00; // NULL character
  sprintf(line, "%04x |", (unsigned short)startAddress);
  line[6] = 0x20; // Remove NULL character added by sprintf
  int idx = 0;
  int hexOffset = 7;
  int charOffset = 58;
  for ( ; idx < (int)(currentIdx % 16); idx++) {
    line[hexOffset] = 0x30;
    line[hexOffset + 1] = 0x30;
    hexOffset += 3;
    charOffset += 2;
  }
  // Fill line by line
  int endOfDump = p_count + p_offset;
  while(currentIdx < endOfDump) {
    for ( ; (idx < 16) && (currentIdx < endOfDump); idx++) {
      line[hexOffset] = dump::toHexDigit(*(p_buffer + currentIdx) >> 4);
      line[hexOffset + 1] = dump::toHexDigit(*(p_buffer + currentIdx) & 0x0f);
      line[charOffset] = dump::toCharDigit(*(p_buffer + currentIdx));
      // Prepare next byte
      hexOffset += 3;
      charOffset += 2;
      currentIdx += 1;
    }
    // Display the line
    line[56] = ':';
    line[__LINE_LENGTH__ - 1] = 0x0d;
    line[__LINE_LENGTH__] = 0x0a;    
    printf(line);
    if (currentIdx < endOfDump) { // Prepare next line, one line = 16 digits
      startAddress += 16;
      memset(line, 0x20, __LINE_LENGTH__);
      sprintf(line, "%04x |", (unsigned short)startAddress);
      line[6] = 0x20; // Remove NULL character added by sprintf
      idx = 0;
      hexOffset = 7;
      charOffset = 58;
    } else { // End of line padding
      break;
    }
  } // End of 'while' statement
} // End of method dump::hexaDump
