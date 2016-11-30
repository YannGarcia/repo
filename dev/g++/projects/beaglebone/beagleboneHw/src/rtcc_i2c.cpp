/* Simplified access to basic RTCC + Memory device (I2C)
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
#include <iostream>
#include <iomanip>
#include <cstring>

#include <sys/ioctl.h>
#include <linux/i2c.h> // Use direct I2C driver
#include <linux/i2c-dev.h>
#include <linux/fs.h> // Used for BLKFLSBUF (clear buffer)
#include <unistd.h> // Used for usleep and close

#include "rtcc_i2c.h"
#include "wiringBbbI2C.h" // Using I2C functions
#include "beagleboneHwEx.h"

#include "converter.h" // beagleboneUtils library

namespace beagleboneHw {

  namespace i2c {

    unsigned char rtcc_i2c::SecondsRegisterAddress = 0x00;
    unsigned char rtcc_i2c::MinutesRegisterAddress = 0x01;
    unsigned char rtcc_i2c::HoursRegisterAddress = 0x02;
    unsigned char rtcc_i2c::DayRegisterAddress = 0x03;
    unsigned char rtcc_i2c::DateRegisterAddress = 0x04;
    unsigned char rtcc_i2c::MonthRegisterAddress = 0x05;
    unsigned char rtcc_i2c::YearRegisterAddress =0x06;
    unsigned char rtcc_i2c::ControlRegisterAddress =0x07;

    rtcc_i2c::rtcc_i2c() : _deviceId(0xff), _memoryBaseAddress(0xff) {
      // Setup I2C support
      if ((_fd = wiringBbbI2CSetup(_deviceId)) == -1) {
	std::cerr << "Failed to initialize adress 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)_deviceId << " (DS1307)" << std::endl;
	throw beagleboneHwEx("rtcc_i2c: wiringBbbI2CSetup failure");
      }
      std::clog << "rtcc_i2c::rtcc_i2c: Device address=0x" << std::hex << std::setw(2) << std::setfill('0') << (int)_deviceId << " - fd=" << std::dec << _fd << std::endl;

      // TODO Declare wiringBbb node

      // Read registers from device
      _ctrlRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::ControlRegisterAddress);
      _dateRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::DateRegisterAddress);
      _dayRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::DayRegisterAddress);
      _monthRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::MonthRegisterAddress);
      _yearRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::YearRegisterAddress);
      _hoursRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::HoursRegisterAddress);
      if ((_hoursRegister & (1 << 6)) == (1 << 6)) { // Check 12/24H mode 
	_hourMode = rtcc_i2c::mode12H; // When high, the 12-hour mode is selected
      } else {
	_hourMode = rtcc_i2c::mode24H;
      }
      _minutesRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::MinutesRegisterAddress);
      _secondsRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::SecondsRegisterAddress);

      std::clog << "Ctor: DS1307 Time: " 
		<< std::hex << std::setw(2) << std::setfill('0')
		<< (int)converter::getInstance().bin2bcd(_hoursRegister & 0x3f) << ":"
		<< std::hex << std::setw(2) << std::setfill('0')
		<< (int)converter::getInstance().bin2bcd(_minutesRegister) << ":"
		<< std::hex << std::setw(2) << std::setfill('0')
		<< (int)converter::getInstance().bin2bcd(_secondsRegister & 0x7f) << std::endl;
    } // End of Constructor

    rtcc_i2c::rtcc_i2c(const time_t p_time) : rtcc_i2c::rtcc_i2c() {
      //TODO: To be continued
    } // End of Constructor

    rtcc_i2c::~rtcc_i2c() {
      if (_fd != -1) {
	close(_fd);
      }
    } // End of Destructor

    void rtcc_i2c::enableClock(const bool p_enable) {
      // Read Seconds register
      _secondsRegister = static_cast<unsigned char>(wiringBbbI2CReadReg8(_fd, rtcc_i2c::SecondsRegisterAddress));
      // Set CH bit properly
      if (!p_enable) { // See DS1307 64 x 8, Serial, I2C Real-Time Clock Table 2. Timekeeper Registers
	_secondsRegister |= 0x80;
      } else {
	_secondsRegister &= 0x7f;
      }
      // Write Seconds register
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::SecondsRegisterAddress, _secondsRegister);
    }

    void rtcc_i2c::setTime(const time_t p_time) {
      // Convert time_t into struct tm
      struct tm *tm_ = localtime(&p_time);
      // Set date/time register
      _dayRegister = static_cast<unsigned char>(tm_->tm_wday + 1) & 0x07; // tm_wday is in range [0, 6]
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::DayRegisterAddress, _dayRegister);
      _dateRegister = static_cast<unsigned char>(tm_->tm_mday) & 0x3f; 
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::DateRegisterAddress, converter::getInstance().bin2bcd(_dateRegister));
      _monthRegister |= static_cast<unsigned char>(converter::getInstance().bin2bcd(tm_->tm_mon + 1)) & 0x1f; // tm_mon is in range [0, 11]
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::MonthRegisterAddress, _monthRegister);
      _yearRegister = static_cast<unsigned char>(converter::getInstance().bin2bcd(tm_->tm_year));
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::YearRegisterAddress, _yearRegister);
      if (_hourMode == rtcc_i2c::mode12H) {
	if (tm_->tm_hour < 12) { // AM
	  _hoursRegister |= static_cast<unsigned char>(converter::getInstance().bin2bcd(tm_->tm_hour)) & 0x1f;
	} else {
	  _hoursRegister |= (static_cast<unsigned char>(converter::getInstance().bin2bcd(tm_->tm_hour - 12)) & 0x1f) | (1 << 5);
	}
      } else {
	_hoursRegister |= static_cast<unsigned char>(converter::getInstance().bin2bcd(tm_->tm_hour)) & 0x3f;
      }
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::HoursRegisterAddress, _hoursRegister);
      _minutesRegister |= static_cast<unsigned char>(converter::getInstance().bin2bcd(tm_->tm_min)) & 0x7f;
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::MinutesRegisterAddress, _minutesRegister);
      _secondsRegister |= static_cast<unsigned char>(converter::getInstance().bin2bcd(tm_->tm_sec)) & 0x7f;
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::SecondsRegisterAddress, _secondsRegister);

      std::clog << "setTime: Struct tm Time: " << std::setw(2) << std::setfill('0')
	   << tm_->tm_hour << ":"
	   << tm_->tm_min << ":"
	   << tm_->tm_sec << std::endl;
      std::clog << "setTime: DS1307 Time: " << std::hex << std::setw(2) << std::setfill('0')
	   << (int)(_hoursRegister & 0x3f) << ":"
	   << (int)_minutesRegister << ":"
	   << (int)(_secondsRegister & 0x7f) << std::endl;
    }

    struct tm rtcc_i2c::getTime() {
      struct tm tm_ = { 0 };
      // Read date/time register
      _dayRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::DayRegisterAddress);
      tm_.tm_wday = _dayRegister - 1;
      _dateRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::DateRegisterAddress);
      tm_.tm_mday = converter::getInstance().bcd2bin(_dateRegister);
      _monthRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::MonthRegisterAddress);
      tm_.tm_mon = converter::getInstance().bcd2bin(_monthRegister & 0x1f) - 1; // tm_mon is in range [0, 11]
      _yearRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::YearRegisterAddress);
      tm_.tm_year = converter::getInstance().bcd2bin(_yearRegister);
      _hoursRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::HoursRegisterAddress);
      if (_hourMode == rtcc_i2c::mode12H) {
	tm_.tm_hour = converter::getInstance().bcd2bin(_hoursRegister & 0x1f);
	if ((_hoursRegister & (1 << 5)) == (1 << 5)) {
	  tm_.tm_hour += 12;
	}
      } else { // 24H mode
	tm_.tm_hour = converter::getInstance().bcd2bin(_hoursRegister & 0x3f);
      }
      _minutesRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::MinutesRegisterAddress);
      tm_.tm_min = converter::getInstance().bcd2bin(_minutesRegister & 0x7f);
      _secondsRegister = ::wiringBbbI2CReadReg8(_fd, rtcc_i2c::SecondsRegisterAddress);
      tm_.tm_sec = converter::getInstance().bcd2bin(_secondsRegister & 0x7f);

      std::clog << "getTime: DS1307 Time: " << std::hex << std::setw(2) << std::setfill('0')
	   << (int)(_hoursRegister & 0x3f) << ":"
	   << (int)_minutesRegister << ":"
	   << (int)(_secondsRegister & 0x7f) << std::endl;
      std::clog << "getTime: Struct tm Time: " << std::setw(2) << std::setfill('0')
	   << tm_.tm_hour << ":"
	   << tm_.tm_min << ":"
	   << tm_.tm_sec << std::endl;

      return tm_;
    }

    void rtcc_i2c::setHourMode(const HourMode p_hourMode) {
      // Read Hours register
      _hoursRegister = static_cast<unsigned char>(wiringBbbI2CReadReg8(_fd, rtcc_i2c::HoursRegisterAddress));
      // Set CH bit properly
      _hourMode = p_hourMode;
      if (_hourMode == rtcc_i2c::mode12H) { // See DS1307 64 x 8, Serial, I2C Real-Time Clock Table 2. Timekeeper Registers
	_hoursRegister |= (1 << 6); // When high, the 12-hour mode is selected
      } else { // mode24H
	_hoursRegister &= ~(1 << 6);
      }
      // Write Hours register
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::HoursRegisterAddress, _hoursRegister);
    }

    void rtcc_i2c::enableSqwe(const bool p_enable) {
      // Set SQWE bit properly
      if (p_enable == true) { // See DS1307 64 x 8, Serial, I2C Real-Time Clock Clause CONTROL REGISTER
	_ctrlRegister |= (1 << 4);
      } else { 
	_ctrlRegister &= ~(1 << 4);
      }
      // Write Control register
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::ControlRegisterAddress, _ctrlRegister);
    }

    void rtcc_i2c::setOutLevel(const OutLevel p_level) {
      if (p_level == rtcc_i2c::High) { // See DS1307 64 x 8, Serial, I2C Real-Time Clock Clause CONTROL REGISTER
	_ctrlRegister |= 0x80;
      } else { 
	_ctrlRegister &= 0x7f;
      }
      // And unset SQWE
      _ctrlRegister &= ~(1 <<4);
      // Write Control register
      ::wiringBbbI2CWriteReg8(_fd, rtcc_i2c::ControlRegisterAddress, _ctrlRegister);
    }

    void rtcc_i2c::setSqweFrequency(const SqweFrequencies p_frequency) {
      _ctrlRegister &= 0xfc;
      switch(p_frequency) {
      case rtcc_i2c::_1Hz: // RS<0,1>=00
	// Nothing to do
	break;
      case rtcc_i2c::_4096Hz: // RS<0,1>=01
	_ctrlRegister |= static_cast<unsigned char>(rtcc_i2c::_4096Hz);
	break;
      case rtcc_i2c::_8192Hz: // RS<0,1>=10
	_ctrlRegister |= static_cast<unsigned char>(rtcc_i2c::_8192Hz);
	break;
      case rtcc_i2c::_32768Hz: // RS<0,1>=11
	_ctrlRegister |= static_cast<unsigned char>(rtcc_i2c::_32768Hz);
	break;
      } // End of 'switch' statement
    }

    int rtcc_i2c::writeMemory(const unsigned char p_address, const unsigned char p_data) {
      return ::wiringBbbI2CWriteReg8(_fd, _memoryBaseAddress + p_address, p_data);
    }

    void rtcc_i2c::readMemory(const unsigned char p_address, unsigned char *p_data) {
      *p_data = ::wiringBbbI2CReadReg8(_fd, _memoryBaseAddress + p_address);
    }

    int rtcc_i2c::writeMemory(const unsigned char p_address, const std::string & p_string) {
      return writeMemory(p_address, (const char *)p_string.c_str(), p_string.length());
    } // End of 'writeMemory std::string' method
    
    void rtcc_i2c::readMemory(const unsigned char p_address, const std::string & p_string) {
      // TODO To be implemented
    }

    int rtcc_i2c::writeMemory(const unsigned char  p_address, const std::vector<unsigned char> & p_datas) {
      int length = p_datas.size();
      unsigned char array[length];
      std::copy(p_datas.begin(), p_datas.end(), array); // FIXME Use data() method
      return writeMemory(p_address, (const char *)array, length);
    } // End of 'write std::vector' method
    
    void rtcc_i2c::readMemory(const unsigned char p_address, const std::vector<unsigned char> & p_data) {
      // TODO To be implemented
    }

    int rtcc_i2c::writeMemory(const unsigned char  p_address, const char *p_datas, const int length) {
      // 1.Prepare buffer
      unsigned char i2cBuffer[1 + length];
      // 1.1. Memory address
      i2cBuffer[0] = (unsigned char)(p_address & 0xff);
      // 1.2. Datas
      for (int i = 0; i < length; i++) {
	i2cBuffer[1 + i] = *(p_datas + i);
      }
      // 2. Send I2C start + I2C address + Memory Address + Datas + I2C stop
      struct i2c_rdwr_ioctl_data i2cMsgRdwr = { 0 };
      struct i2c_msg i2cMsg[1];
      i2cMsgRdwr.msgs = i2cMsg;
      i2cMsgRdwr.nmsgs = 1;
      i2cMsg[0].addr = _deviceId;
      i2cMsg[0].flags = 0;
      i2cMsg[0].len  = 1 + length;
      i2cMsg[0].buf = (unsigned char *)i2cBuffer;
      
      int result = ioctl(_fd, I2C_RDWR, &i2cMsgRdwr);
      if (result < 0) {
	std::cerr << "rtcc_i2c::write (char *) failed: " << strerror(errno) << std::endl;
	return -1;
      }
      usleep(200);

      return result;
    } // End of 'writeMemory char *' method
    
    void rtcc_i2c::readMemory(const unsigned char p_address, char *p_data, const int length) {
      // TODO To be implemented
    }

  } // End of namespace i2c

} // End of namespace beagleboneHw
