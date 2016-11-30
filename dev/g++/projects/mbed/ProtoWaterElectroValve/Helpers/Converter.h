/* mbed sconverstion tools
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
#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>

#include <string>
#include <vector>
#include <algorithm>

#include <ctime>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h> // time_t, struct tm, difftime, time, mktime

namespace Helpers {

    class CConverter {
    
        struct convert2upper {
           convert2upper() { }
           void operator() (char & c) { c = toupper((unsigned char)c); }
        };
        
        struct HexaFormat {
            HexaFormat() {  }
            void operator()(unsigned char p_byte) { ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(p_byte); }
            std::string toString() { return std::string(ss.str()); }
            
            std::stringstream ss;
        };
        static CConverter * Instance;
    
        CConverter() {};
        ~CConverter() {
            if (Instance != NULL) {
                delete Instance;
                Instance = NULL;
            }
        };
    
    public:
        inline static CConverter & getInstance() {
            if (Instance == NULL) Instance = new CConverter();
            return *Instance;
        };
    
    public:
        inline unsigned char string2bcd(const std::string & p_value) {
            return bin2bcd(static_cast<unsigned char>(CConverter::getInstance().string2int(p_value)));
        };
        inline unsigned char bcd2bin(const unsigned char p_value) {
            return ((p_value / 16 * 10) + (p_value % 16));
        };
        inline unsigned char bin2bcd(const unsigned char p_value) {
            return ((p_value / 10 * 16) + (p_value % 10));
        };
    
    public:
        unsigned short swap(const unsigned short p_value);
        inline short swap(const short p_value) {
            return static_cast<short>(swap(static_cast<unsigned short>(p_value)));
        };
        unsigned int swap(const unsigned int p_value);
        inline int swap(const int p_value) {
            return static_cast<int>(swap(static_cast<unsigned int>(p_value)));
        };
    
    public:
        std::string string2hexa(const std::string & p_value);
        std::string hexa2string(const std::string & p_value);
        std::string hexa2string(const std::vector<unsigned char> & p_value);
        
        std::string time2string(const time_t p_time);
        std::string time2string(const struct tm & p_time);
        
    public:
        inline std::vector<unsigned char> int2bytes(const int p_value) const {
            unsigned char bytes[sizeof(p_value)];
            std::copy(
                static_cast<const unsigned char *>(static_cast<const void *>(&p_value)),
                static_cast<const unsigned char *>(static_cast<const void *>(&p_value)) + sizeof(p_value),
                bytes
            );
            std::vector<unsigned char> result(bytes, bytes + sizeof(bytes) / sizeof(unsigned char));
            return result;
        }; // End of int2bytes
        inline int bytes2int(const std::vector<unsigned char> & p_value) const {
            return *((int *)(&p_value[0]));
        }; // End of bytes2int
    
        inline std::vector<unsigned char> long2bytes(const long p_value) const {
            unsigned char bytes[sizeof(p_value)];
            std::copy(
                static_cast<const unsigned char *>(static_cast<const void *>(&p_value)),
                static_cast<const unsigned char *>(static_cast<const void *>(&p_value)) + sizeof(p_value),
                bytes
            );
            std::vector<unsigned char> result(bytes, bytes + sizeof(bytes) / sizeof(unsigned char));
            return result;
        }; // End of long2bytes
        inline int bytes2long(const std::vector<unsigned char> & p_value) const {
            return *((long *)(&p_value[0]));
        }; // End of bytes2long
    
        inline std::vector<unsigned char> float2bytes(const float p_value) const {
            unsigned char bytes[sizeof(p_value)];
            std::copy(
                static_cast<const unsigned char *>(static_cast<const void *>(&p_value)),
                static_cast<const unsigned char *>(static_cast<const void *>(&p_value)) + sizeof(p_value),
                bytes
            );
            std::vector<unsigned char> result(bytes, bytes + sizeof(bytes) / sizeof(unsigned char));
            return result;
        }; // End of float2bytes
        inline float bytes2float(const std::vector<unsigned char> & p_value) const {
            return *((float *)(&p_value[0]));
        }; // End of bytes2float
    
        inline std::vector<unsigned char> string2bytes(const std::string & p_value) const {
            return std::vector<unsigned char>(p_value.begin(), p_value.end());
        }; // End of string2bytes
        inline std::string bytes2string(const std::vector<unsigned char> & p_value) const {
            return std::string(p_value.begin(), p_value.end());
        }; // End of bytes2string
    
    public:
        inline int string2int(const std::string & p_value) const {
            //return std::stoi(p_value);
            return atoi(p_value.c_str());
        }; // End of string2bytes
        inline std::string int2string(const int & p_value) const {
            std::ostringstream ss;
            ss << p_value;
            return ss.str();
         }; // End of string2bytes
        
    public:
        std::string trim(const std::string& str, const std::string& whitespace = " \t");
        std::vector<std::string> split_arguments_line(const std::string & p_value);
                  
    }; // End of class Converter

} // End of namespace Helpers

using namespace Helpers;
