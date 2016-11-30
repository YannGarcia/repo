/* Beaglebone Dump packet used by all my developed program
 * Copyright (c) 2010-2013 ygarcia, MIT License
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
#if !defined(__DUMP_H__)
#define __DUMP_H__

#include <vector>

/** This class implements debug functionalities based on USB console interface. V0.0.0.1
 *
 * Note that this class is based on Helper pattern
 */
class dump
{
  /** Convert the specified digit into hexadecimal number (0x30..0x39 (0..9), 0x47..x4c (A..F))
   *
   * @param p_digit The digit to convert
   * @return An hexadecimal digit (0..9-A..F)
   */
  static inline unsigned char toHexDigit(unsigned char p_digit) { return ((p_digit < 10) ? (p_digit + 0x30) : (p_digit + 0x37)); };
  /** Convert the specified hexadecimal digit into a character if it is printable, or replace by a '.' otherwise
   *
   * @param p_digit The hexadecimal digit to convert
   * @return A character is it's printable, '.' otherwise
   */
  static inline char toCharDigit(unsigned char p_digit) { return (((p_digit < 0x20) || (p_digit > 0x80)) ? '.' : (char)p_digit); };

 public:
  /** Log an hexadecimal buffer
   *
   * Note that parameters 'p_offset' and 'p_length' are not supported yet
   *
   * @param p_buffer The buffer to dump
   * @param p_count Number of bytes to dump
   * @param p_offset Offset to start the dump. Default: 0
   */
  static void hexaDump(const unsigned char* p_buffer, int p_count, int p_offset = 0);
  /** Log an hexadecimal buffer
   *
   * Note that parameters 'p_offset' and 'p_length' are not supported yet
   *
   * @param p_buffer The buffer to dump
   * @param p_count Number of bytes to dump
   * @param p_offset Offset to start the dump. Default: 0
   */
  inline static void hexaDump(const std::vector<unsigned char> p_buffer, int p_offset = 0) { dump::hexaDump(p_buffer.data(), p_buffer.size(), p_offset); };

}; // End of class dump
  
#endif // __DUMP_H__
