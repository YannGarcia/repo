/* Simplified access to basic RTCC RTC + Memory device (I2C)
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
#if !defined(__RTCC_I2C_H__)
#define __RTCC_I2C_H__

#include <vector>
#include <string>
#include <ctime>

#include "wiringBbb.h" // wiringBbb library

#include "debug.h" // Debug primitives. See DebugLibrary

namespace beagleboneHw {

  namespace i2c {

    class rtcc_i2c {
    protected:
      
      static unsigned char SecondsRegisterAddress;
      static unsigned char MinutesRegisterAddress;
      static unsigned char HoursRegisterAddress;
      static unsigned char DayRegisterAddress;
      static unsigned char DateRegisterAddress;
      static unsigned char MonthRegisterAddress;
      static unsigned char YearRegisterAddress;
      static unsigned char ControlRegisterAddress;

      unsigned char _deviceId;
      unsigned char _memoryBaseAddress;
      unsigned char _ctrlRegister;
      unsigned char _dateRegister;
      unsigned char _dayRegister;
      unsigned char _monthRegister;
      unsigned char _yearRegister;
      unsigned char _hoursRegister;
      unsigned char _minutesRegister;
      unsigned char _secondsRegister;

      int _fd;
    
    public:
      enum HourMode {
	mode12H,
	mode24H
      };
    
      enum SqweFrequencies : unsigned char {
	_1Hz    = 0x00,
	_4096Hz = 0x01,
	_8192Hz = 0x02,
	_32768Hz = 0x03	
      };

      enum OutLevel : unsigned char {
	Low = 0x00,
	High = 0xff
      };
    
    public:
      rtcc_i2c();
      rtcc_i2c(const time_t p_time);
      virtual ~rtcc_i2c();

      void enableClock(const bool p_enable = true);

      virtual void setTime(const time_t p_time);
      virtual struct tm getTime();

      virtual void setHourMode(const HourMode p_hourMode = mode24H);

      virtual void enableSqwe(const bool p_enable = true);
      virtual void setOutLevel(const OutLevel p_level);
      virtual void setSqweFrequency(const SqweFrequencies p_frequency = _1Hz);

      virtual int writeMemory(const unsigned char p_address, const unsigned char p_data);
      virtual int writeMemory(const unsigned char p_address, const std::string & p_string);
      virtual int writeMemory(const unsigned char p_address, const std::vector<unsigned char> & p_datas);
      virtual int writeMemory(const unsigned char p_address, const char *p_datas, const int length);
      virtual void readMemory(const unsigned char p_address, unsigned char *p_data);
      virtual void readMemory(const unsigned char p_address, const std::string & p_string);
      virtual void readMemory(const unsigned char p_address, const std::vector<unsigned char> & p_datas);
      virtual void readMemory(const unsigned char p_address, char *p_data, const int length);
      
      virtual inline void createAlarm() { };
      virtual inline void cancelAlarm() { };

    private:
      HourMode _hourMode;

    }; // End of class rtcc_i2c
    
  } // End of namespace i2c
  
} // End of namespace beagleboneHw 

using namespace beagleboneHw::i2c;

#endif // __RTCC_2CI_H__
