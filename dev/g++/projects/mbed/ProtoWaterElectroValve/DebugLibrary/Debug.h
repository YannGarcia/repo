/* mbed Debug library used by all my developed program
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
#if !defined(__DEBUG_H__)
#define __DEBUG_H__

#include "mbed.h"

/** The steps below describe how to use this library:
 * 1. Import this library to your project 'As file', because you will need to modify this file as described in step 2
 * 2. Edit this library
 * 3. Remove comment from line 66 (search for '//#define __DEBUG' in this file) to get the DEBUG macro defimed properly. By default, __DEBUG flahg is undef
 * 4. Rebuild this library and use the debug macro as decribe in sample code
 *
 * IMPORTANT: If you modify this libray, please keep this comment up to date for future users
 *
 * Please refer to handbook (http://mbed.org/handbook/Debugging) for mbed Debugging support
 *
 * @code
 * #include "Debug.h"                          // This header alway includes mbed.h file
 * 
 * DigitalOut myled(LED1);
 * 
 * int main() {
 *     DEBUG_ENTER("main")                     // Log the entry of the C function 'main'
 *
 *     std::string str("This is a sample for heaxdecimal dump using DebugLibrary");
 *     DEBUG(">>> Example:");
 *     HEXADUMP((unsigned char *)str.c_str(), str.length());
 *     DEBUG("===");
 *     HEXADUMP_OFFSET((unsigned char *)str.c_str(), str.length() - 19, 19);
 *     DEBUG("<<<");
 *
 *     int counter = 0;
 *     while(1) {
 *         DEBUG("In loop [%d]", counter++)    // A sample message
 *         myled = 1;
 *         wait(0.2);
 *         myled = 0;
 *         wait(0.2);
 *         DEBUG_BREAK()                       // Wait for any key pressed to continue
 *     } // End of 'while' statement
 *     DEBUG_LEAVE("main, never reached")      // Log the end of the C function 'main'
 * }
 * @endcode
 */
//#undef __DEBUG //<! Undefined debug flah, default value
#define __DEBUG //<! Uncomment this line to activate debug macros

// Undefined DEBUG symbols to be sure to use mine
#undef DEBUG_ENTER
#undef DEBUG_LEAVE
#undef DEBUG
#undef HEXADUMP
#undef HEXADUMP_OFFSET
#undef DEBUG_ERROR
#undef DEBUG_WARNING
#undef DEBUG_BREAK
#undef DEBUG_FATAL

#ifdef __DEBUG

/** This class implements debug functionalities based on USB console interface. V0.0.0.8
 *
 * Note that this class is based on Helper pattern
 */
class DebugHelper
{
    /** Convert the specified digit into hexadecimal number (0x30..0x39 (0..9), 0x47..x4c (A..F))
     *
     * @param p_digit The digit to convert
     * @return An hexadecimal digit (0..9-A..F)
     */
    static inline unsigned char ToHexDigit(unsigned char p_digit) { return ((p_digit < 10) ? (p_digit + 0x30) : (p_digit + 0x37)); };
    /** Convert the specified hexadecimal digit into a character if it is printable, or replace by a '.' otherwise
     *
     * @param p_digit The hexadecimal digit to convert
     * @return A character is it's printable, '.' otherwise
     */
    static inline char ToCharDigit(unsigned char p_digit) { return (((p_digit < 0x20) || (p_digit > 0x80)) ? '.' : (char)p_digit); };
public:
    /** Standard log method
     * @param p_format Format string compliant with C 'printf' format string
     */
    static void Debug(const char* p_format, ...);
    /** Log an hexadecimal buffer
     *
     * Note that parameters 'p_offset' and 'p_length' are not supported yet
     *
     * @param p_buffer The buffer to dump
     * @param p_count Number of bytes to dump
     * @param p_offset Offset to start the dump. Default: 0
     */
    static void HexaDump(unsigned char* p_buffer, int p_count, int p_offset = 0);
    /** Break point method based on getchar() C function
     */
    static void BreakPoint(const char* p_file, int p_line);
}; // End of class DebugHelper

/** Used to log function/method entry (>> )
 *
 * Note that \ for multiline macro is not supported yet
 */
#define DEBUG_ENTER(...) do { DebugHelper::Debug(">> "); DebugHelper::Debug(__VA_ARGS__); DebugHelper::Debug("\r\n"); } while(false);
/** Used to log function end of function/method (<< )
 *
 * Note that \ for multiline macro is not supported yet
 */
#define DEBUG_LEAVE(...) do { DebugHelper::Debug("<< "); DebugHelper::Debug(__VA_ARGS__); DebugHelper::Debug("\r\n"); } while(false);
/** Used to log a standard message
 *
 * Note that \ for multiline macro is not supported yet
 */
#define DEBUG(...) do { DebugHelper::Debug(__VA_ARGS__); DebugHelper::Debug("\r\n"); } while(false);
/** Used to dump an hexadecimal buffer
 *
 * Note that \ for multiline macro is not supported yet
 */
#define HEXADUMP(p_buffer, p_count) DebugHelper::HexaDump(p_buffer, p_count);
/** Used to dump an hexadecimal buffer with an offset
 *
 * Note that \ for multiline macro is not supported yet
 */
#define HEXADUMP_OFFSET(p_buffer, p_count, p_offset) DebugHelper::HexaDump(p_buffer, p_count, p_offset);
/** Used to log an error message (?? )
 *
 * Note that \ for multiline macro is not supported yet
 */
#define DEBUG_ERROR(...) do { DebugHelper::Debug("?? "); DebugHelper::Debug(__VA_ARGS__); DebugHelper::Debug("\r\n"); } while(false);
/** Used to log a warning message (!! )
 *
 * Note that \ for multiline macro is not supported yet
 */
#define DEBUG_WARNING(...) do { DebugHelper::Debug("!! "); DebugHelper::Debug(__VA_ARGS__); DebugHelper::Debug("\r\n"); } while(false);

/** Break point macro
 */
#define DEBUG_BREAK() DebugHelper::BreakPoint(__FILE__, __LINE__);

/** Used to stop program on fatal error
 */
#define DEBUG_FATAL(cause) error(cause);

#else // __DEBUG

/** Undefine DEBUG macro, used when __DEBUG is undefined
 */
#define DEBUG_ENTER(...)
/** Undefine DEBUG macro, used when __DEBUG is undefined
 */
#define DEBUG_LEAVE(...)
/** Undefine DEBUG macro, used when __DEBUG is undefined
 */
#define DEBUG(...)
/** Undefine DEBUG macro, used when __DEBUG is undefined
 *
 * Note that \ for multiline macro is not supported yet
 */
#define HEXADUMP(p_buffer, p_count)
/** Undefine DEBUG macro, used when __DEBUG is undefined
 *
 * Note that \ for multiline macro is not supported yet
 */
#define HEXADUMP_OFFSET(p_buffer, p_count, p_offset)
/** Undefine DEBUG_ERROR macro, used when __DEBUG is undefined
 */
#define DEBUG_ERROR(...)
/** Undefine DEBUG_WARNING macro, used when __DEBUG is undefined
 */
#define DEBUG_WARNING(...)

/** Undefine DEBUG_BREAK macro, used when __DEBUG is undefined
 */
#define DEBUG_BREAK()

/** Undefine DEBUG_FATAL macro, used when __DEBUG is undefined
 */
#define DEBUG_FATAL(cause)

#endif // __DEBUG
  
#endif // __DEBUG_H__
