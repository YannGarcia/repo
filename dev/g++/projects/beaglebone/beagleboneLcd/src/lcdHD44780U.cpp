#include <iostream>
#include <iomanip>
#include <sstream> // std::ostringstream
#include <stdexcept> // std::out_of_range
#include <cstring> // Used std::strerror

#include <unistd.h> // Used for usleep and close

#include "lcdHD44780U.h"

#include "wiringBbb.h"

namespace beagleboneLcd {

  lcdHD44780U::lcdHD44780U() {
    _en = NULL;
    _rs = NULL;
    _rw = NULL;
  } // End of Constructor

  lcdHD44780U::~lcdHD44780U() {
    uninitialize();
  } // End of Destructor

  void lcdHD44780U::setPowerMode(const abstractLcd::LcdPowerMode p_powerMode, const int p_pwmVlcd) {
    abstractLcd::setPowerMode(p_powerMode, p_pwmVlcd);
    // Sanity checks
    if (_powerMode == abstractLcd::mode5V) {
      return; // Nothing to do
    }
    if ((_powerMode == abstractLcd::mode3_3V) && (_pwmVlcd == 0xff)) {
      return; // Nothing to do
    }

    ::pinMode(_pwmVlcd, PWM_OUTPUT);
    ::pwmSetMode(PWM_MODE_MS);
    ::pwmWrite(_pwmVlcd, 30); // Duty cycle = 30%
  } // End of method setPowerMode

  int lcdHD44780U::initialize(const abstractLcd::LcdMode p_mode, const abstractLcd::LcdType p_type, const std::vector<unsigned char> & p_datas, const unsigned char p_en, const unsigned char p_rs, const unsigned char p_rw) {
    // Sanity checks
    if ((p_datas.size() != 4) && (p_datas.size() != 8)) {
      std::ostringstream os;
      os << "lcdHD44780U::initialize: Wrong parameters: " << std::strerror(errno) << std::endl;
      return -1;
    }
    if (p_en == 0xff) {
      std::ostringstream os;
      os << "lcdHD44780U::initialize: Wrong port parameters: " << std::strerror(errno) << std::endl;
      return -1;
    }
    
    // Set default ports settings
    if (p_rs != 0xff) {
      _rs = new digitalOut(p_rs);
      _rs->write(0);
    }
    if (p_rw != 0xff) {
      _rw = new digitalOut(p_rw);
      _rw->write(0);
    }
    _en = new digitalOut(p_en);
    _en->write(0);

    // Data ports
    _datas.clear();
    for (auto it = p_datas.begin(); it != p_datas.end(); ++it) {
      //      printf("lcdHD44780U::initialize: Create port for %d\n", *it);
      _datas.push_back(std::unique_ptr<digitalInOut>(new digitalInOut(*it)));
    } // End of 'for' statement

    // Initialize the LCD
    _mode = p_mode;
    _type = p_type;
    configure();

    return 0;
  }

  int lcdHD44780U::uninitialize() {
    if (_en != NULL) {
      delete _en;
      _en = NULL;
      if (_rs != NULL) {
	delete _rs;
	_rs = NULL;
      }
      if (_rw != NULL) {
	delete _rw;
	_rw = NULL;
      }
      for (auto it = _datas.begin(); it != _datas.end(); ++it) {
	it->reset(nullptr);
      } // End of 'for' statement
      
      _datas.clear();
    }

    return 0;
  }

  int lcdHD44780U::configure() {
    // RW and RS ports are unset 
    unsigned char cmd;
    // Initiate the reset sequence for 4 bits mode
    // Wait least 40ms after power rises above 2.7V
    usleep(45000);
    // Send 0x30 + wait time > 4.1ms
    cmd = 0x30;
    write(cmd, false);
    usleep(45000);
    // Send 0x30 + wait time > 100us
    write(cmd, false);
    usleep(45000);
    // Send 0x30 + 150us
    write(cmd, false);
    usleep(150);
    // Select bus width (0x30 - for 8-bit and 0x20 for 4-bit) + 1ms
    if (_mode == abstractLcd::mode_4bits) {
      cmd = 0x20; // 4 bits mode
    } else {      // 8 bits mode
      cmd = 0x30;
    }
    write(cmd, false);
    usleep(150);

    /* At ths point, the LCD is ready to accept command/data in 4 or 8 bits format */

    // Function set: DL=0: 4-bits mode, N=1: 2-line display, F=0: 5*8 dots
    cmd = 0x28;
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(cmd & 0xf0);        // MSB part of the command byte is stored into the MSB quartet
      write((cmd << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(cmd);
    }
    // Set display off
    cmd = 0x08;
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(cmd & 0xf0);        // MSB part of the command byte is stored into the MSB quartet
      write((cmd << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(cmd);
    }
    // Clear screen
    cmd = ClearDisplay;
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(cmd & 0xf0);        // MSB part of the command byte is stored into the MSB quartet
      write((cmd << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(cmd);
    }
    // Set Entry mode: 1/ Increment cursor to the right when writing and 2/ Don’t shift screen
    cmd = SetEntryMode;
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(cmd & 0xf0);        // MSB part of the command byte is stored into the MSB quartet
      write((cmd << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(cmd);
    }
    // Display ON, Cursor ON and Blinking ON    
    _displayStatus = DisplayOnCursorOnAndBlinkingOff;
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(_displayStatus & 0xf0);        // MSB part of the command byte is stored into the MSB quartet
      write((_displayStatus << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(_displayStatus);
    }

    return 0;
  } // End of method configure

  int lcdHD44780U::write(const unsigned char p_data, const bool p_waitBF) {
    //printf("write: %02x\n", p_data);

    if (p_waitBF) waitBF();

    unsigned char flag = 0x80;
    for (auto it = _datas.begin(); it != _datas.end(); ++it) {
      if ((p_data & flag) != 0) { // bit set
	//printf("lcdHD44780U::write: set port %d to 1\n", (int)(*it)->getPort());
	(*it)->write(1);
      } else { // bit unset
	//printf("lcdHD44780U::write: set port %d to 0\n", (int)(*it)->getPort());
	(*it)->write(0);
      }
      flag >>= 1;
    } // End of 'for' statement

    usleep(1);
    _en->write(1); // Enable LCD
    usleep(1);     // Enable pulse must be > 450ns
    _en->write(0); // Disable LCD

    return 0;
  } // End of method write

  int lcdHD44780U::waitBF() {

    if (_rw != NULL) { // R/W supported
      _rw->write(1); // RW = 1

      do { // Wait until Busy Flag (DB7) was unset
	usleep(39); // Minimun execution time is 38us
      } while (_datas[0]->read() == 1); 

      _rw->write(0);
    } else { // R/W not supported, just wait
      //      printf("lcdHD44780U::waitBF: waiting\n");
      usleep(5000);
    }
    return 0;
  } // End of method waitBF

 void lcdHD44780U::command(const unsigned char p_command) {
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(p_command & 0xf0);        // MSB part of the command byte is stored into the MSB quartet
      write((p_command << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(p_command);
    }
  }

  int lcdHD44780U::write(const std::string & p_line) {
    //    printf("write: %s\n", p_line.c_str());

    for (auto it = p_line.cbegin(); it != p_line.cend(); ++it) {
      if (*it == '\n') {
	_column = 0;
        _row++;
        if (_row >= rows()) {
	  _row = 0;
        }
	// And then skip it
      } else {
	// Write the character
	write(_row, _column, *it);
	// Upgrade the row/column
        _column++;
        if (_column >= columns()) {
	  _column = 0;
	  _row++;
	  if (_row >= rows()) {
	    _row = 0;
	  }
        }
      }
    } // End of 'for' statement

    return 0;
  } // End of method write

  int lcdHD44780U::write(const int p_row, const int p_column, const char p_char) {
    //    printf("write: %d, %d, %02x\n", p_row, p_column, p_char);

    // Set cursor position
    unsigned char cmd = setAddress(p_row, p_column);
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(cmd & 0xf0);        // MSB part of the command byte is stored into the MSB quartet
      write((cmd << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(cmd);
    }
    // Write text
    if (_rs != NULL) _rs->write(1); // Set RS to write data
    usleep(1);
    if (_mode == abstractLcd::mode_4bits) { // mode_4bits: The LCD read only MSB part each time EN pin is pulsed
      write(static_cast<unsigned char>(p_char) & 0xf0); // MSB part of the command byte is stored into the MSB quartet
      write((static_cast<unsigned char>(p_char) << 4) & 0xf0); // LSB part of the command byte is stored into the part in MSB quartet
    } else { // mode_8bits
      write(static_cast<unsigned char>(p_char));
    }
    if (_rs != NULL) _rs->write(0); // Unset RS to write data
    usleep(100);

    return 0;
  } // End of method write

  unsigned char lcdHD44780U::setAddress(const int p_row, const int p_column) {
    switch (_type) {
    case abstractLcd::lcd20x4:
      switch (p_row) {
      case 0:
	return 0x80 + p_column;
      case 1:
	return 0xc0 + p_column;
      case 2:
	return 0x94 + p_column;
      case 3:
	return 0xd4 + p_column;
      }
    case abstractLcd::lcd16x2B:
      return 0x80 + (p_row * 40) + p_column;
    case abstractLcd::lcd16x2:
      // No break;
    case abstractLcd::lcd20x2:
      // No break;
    default:
      return 0x80 + (p_row * 0x40) + p_column;
    } // End of 'switch' statement

  } // End of method setAddress 
 
} // End of namespace beagleboneLcd
