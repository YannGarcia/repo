/* mbed converstion tools
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

#include <cctype> // Used for toupper

#include "Converter.h"

namespace Helpers {

    CConverter * CConverter::Instance = NULL;
    
    unsigned short CConverter::swap(const unsigned short p_value)
    {
        unsigned char *ptr = (unsigned char *)&p_value;
        return (ptr[0] << 8) | ptr[1];
    }
    
    unsigned int CConverter::swap(const unsigned int p_value)
    {
        unsigned char *ptr = (unsigned char *)&p_value;
        return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
    }
    
    std::string CConverter::string2hexa(const std::string & p_value)
    {
        static const char * const lut = "0123456789ABCDEF";
        unsigned int length = p_value.length();

        std::string input(p_value);
        std::for_each(input.begin(), input.end(), convert2upper());
        
        std::string output;
        output.reserve(2 * length);
        for (unsigned int i = 0; i < length; ++i) {
            const unsigned char c = input[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        } // End of 'for' statement
    
        return output;
    }
    
    std::string CConverter::hexa2string(const std::string & p_value)
    {
        static const char * const lut = "0123456789ABCDEF";
        unsigned int length = p_value.length();
        std::string output = "";
        if (length & 1) {
            return output;
        }
    
        output.reserve(length / 2);
        for (unsigned int i = 0; i < length; i += 2) {
            char msbDigit = p_value[i];
            const char *msbIndex = std::lower_bound(lut, lut + 16, msbDigit);
            if (*msbIndex != msbDigit) {
                output = "";
                return output;
            }
            char lsbDigit = p_value[i + 1];
            const char* lsbIndex = std::lower_bound(lut, lut + 16, lsbDigit);
            if (*lsbIndex != lsbDigit) {
                output = "";
                return output;
            }
            output.push_back(((msbIndex - lut) << 4) | (lsbIndex - lut));
        } // End of 'for' statement
    
        return output;
    }

    std::string CConverter::hexa2string(const std::vector<unsigned char> & p_value)
    {
//        HexaFormat hexa = std::for_each(p_value.begin(), p_value.end(), HexaFormat()); 
//        return hexa.toString();

        std::stringstream ss;
        //for (std::vector<unsigned char>::iterator it = p_value.begin() ; it != p_value.end(); ++it)
        for(int i = 0; i < p_value.size(); i += 1) { 
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(p_value[i]);
        } // End of 'for' statement
        
        return std::string(ss.str());
    }
    
    std::string CConverter::time2string(const time_t p_time) {
        struct tm * t = std::localtime(&p_time);
        return time2string(*t);
    }
    
    std::string CConverter::time2string(const struct tm & p_time) {
        char buffer[32] = { 0 };
        // Format: Mo, 15.06.2009 20:20:00
        std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", &p_time);
        return std::string(buffer);
    }
    
    std::string CConverter::trim(const std::string& str, const std::string& whitespace) {
        size_t strBegin = str.find_first_not_of(whitespace);
        if (strBegin == std::string::npos)
            return ""; // no content
     
        size_t strEnd = str.find_last_not_of(whitespace);
        size_t strRange = strEnd - strBegin + 1;
     
        return str.substr(strBegin, strRange);
    }
    
    std::vector<std::string> CConverter::split_arguments_line(const std::string & p_value) {
        std::vector<std::string> output;
        std::string line = trim(p_value);
        if (!line.empty() &&  (line[0] == '-')) { // Valid command line
            size_t current = 0;
            size_t next = (size_t)-1;
            size_t pos = 0;
            do {
                if (line[pos + 1] == '-') { // --
                    current = pos + 2;
                } else {
                    current = pos + 1;
                }
                next = line.find("-", current);
                output.push_back(line.substr(pos, next - pos));
                pos = next;
            } while (next != std::string::npos);
        } // else, invalid command line
        return output;
    }

} // End of namespace Helpers
