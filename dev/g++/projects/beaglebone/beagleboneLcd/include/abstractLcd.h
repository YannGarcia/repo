/* Beaglebone Abstract LCD description
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
#if !defined(__ABSTRACTLCD_H__)
#define __ABSTRACTLCD_H__

#include <stdexcept> // used for std::runtime_error
#include <string>
#include <vector>

#include "digitalInOut.h"

namespace beagleboneLcd {

  class abstractLcd {
  public:
    enum LcdFamily : unsigned char {
      HD44780U = 0x00
    };

    enum LcdMode : unsigned char {
      mode_4bits = 0x00,
      mode_8bits = 0x01
    };

    enum LcdType : unsigned char {
      lcd20x4  = 0x00,
      lcd16x2B = 0x01,
      lcd16x2  = 0x02,
      lcd20x2  = 0x03
    };

    enum LcdPowerMode : unsigned char {
      mode5V = 0x00,
      mode3_3V = 0x01 // In case of 3.3V power, PWN is required
    };

  protected:
    std::vector<std::unique_ptr<digitalInOut>> _datas;
    abstractLcd::LcdMode _mode;
    abstractLcd::LcdType _type;
    abstractLcd::LcdPowerMode _powerMode;
    int _pwmVlcd;
    int _row;
    int _column;

  public:
    abstractLcd() : _datas() { 
      _mode = abstractLcd::mode_4bits;
      _type = abstractLcd::lcd16x2;
      _row = _column = 0;
      _powerMode = abstractLcd::mode5V;
      _pwmVlcd = 0xff;
    };
    virtual ~abstractLcd() { };

    virtual void setPowerMode(const abstractLcd::LcdPowerMode p_powerMode = abstractLcd::mode5V, const int p_pwmVlcd = 0xff) { _powerMode = p_powerMode; _pwmVlcd = p_pwmVlcd; };
    virtual int initialize(const abstractLcd::LcdMode p_mode, const abstractLcd::LcdType p_type, const std::vector<unsigned char> & p_datas, const unsigned char p_en, const unsigned char p_rs = 0xff, const unsigned char p_rw = 0xff) {  throw new std::runtime_error("Method shall be overwritten"); };
    virtual int uninitialize() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void clear() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void home() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void setCursor(const int p_row, const int p_column) {  throw new std::runtime_error("Method shall be overwritten"); };
    virtual void displayOff() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void displayOn() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void blinkOff() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void blinkOn() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void cursorOff() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void cursorOn() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void scrollDisplayLeft() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void scrollDisplayRight() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void leftToRight() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void rightToLeft() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void autoscrollOn() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void autoscrollOff() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual int write(const std::string & p_line) {  throw new std::runtime_error("Method shall be overwritten"); };

    virtual int loadFont(const std::vector<unsigned char> p_font) {  throw new std::runtime_error("Method shall be overwritten"); };

  protected:
    inline const int columns() const {
      switch (_type) {
      case abstractLcd::lcd20x4:
	// No break;
      case abstractLcd::lcd20x2:
	return 20;
      case abstractLcd::lcd16x2:
	// No break;
      case abstractLcd::lcd16x2B:
      default:
	return 16;
      }
    };
  
    inline const int rows() const {
      switch (_type) {
      case abstractLcd::lcd20x4:
	return 4;
      case abstractLcd::lcd16x2:
	// No break;
      case abstractLcd::lcd16x2B:
	// No break;
      case abstractLcd::lcd20x2:
	// No break;
    default:
      return 2;
      }
    };

  }; // End of class abstractLcd

} // End of namespace beagleboneLcd

using namespace beagleboneLcd;

#endif // __ABSTRACTLCD_H__
