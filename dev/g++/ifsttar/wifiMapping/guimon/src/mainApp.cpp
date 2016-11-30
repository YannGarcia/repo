/* Beaglebone guimon application implementation
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
#include "mainApp.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include <unistd.h>

#include "beagleboneCommEx.h"

#include "beagleboneHwInit.h"

#include "converter.h"

#include "hwconfig.h"

namespace guimon {

  mainApp::mainApp() : _guiMgr(), _adcMgr(), _lcd(nullptr), _adc(nullptr), _channels(), _samplePeriod(400000) {
    std::clog << ">>> guimon::mainApp::mainApp" << std::endl;

    // Open the message queue (GUI/LCD communication)
    if (_guiMgr.open(mqMgr::guiMq) != 0) {
      std::ostringstream os;
      os << "guimon::mainApp::initialize: Failed to open the GUI message queue: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Open the message queue (GUI/ADC communication)
    if (_adcMgr.open(mqMgr::adcMq) != 0) {
      std::ostringstream os;
      os << "guimon::mainApp::initialize: Failed to open the GUI message queue: " << std::strerror(errno) << std::endl;
      throw new beagleboneCommEx(os.str());
    }

    // Initialize the wiringBbb library
    beagleboneHwInit::getInstance().initialize();
 
    // Initialize the LCD
    std::vector<unsigned char> dataPorts;
    dataPorts.push_back(LCD_DB7);
    dataPorts.push_back(LCD_DB6);
    dataPorts.push_back(LCD_DB5);
    dataPorts.push_back(LCD_DB4);
    lcdManager::getInstance().createLcdDevice(abstractLcd::HD44780U, _lcd);
    _lcd->setPowerMode();
    _lcd->initialize(abstractLcd::mode_4bits, abstractLcd::lcd20x4, dataPorts, LCD_EN, LCD_RS);
    //    _lcd->autoscrollOn();
    std::clog << "guimon::mainApp::mainApp: LCD initialized" << std::endl;
    _lcd->clear();
    _lcd->write(std::string("Wifi_Mesh coverage!"));
    _lcd->setCursor(1, 3);
    _lcd->write(std::string("Starting..."));

    // Initialize ADC
    _adc.reset(new mcp320x_spi(SPI_CS1_ADC, mcp320x_spi::_3208));
    _channels[(mcp320x_spi::Mcp320xChannels)ADC_CHANNEL_VBUTTON] = 0.0; // TODO How to manage channels to be checked
    /*    _channels[(mcp320x_spi::Mcp320xChannels)(ADC_CHANNEL_VBUTTON - 1)] = 0.0;
    _channels[(mcp320x_spi::Mcp320xChannels)(ADC_CHANNEL_VBUTTON - 2)] = 0.0;
    _channels[(mcp320x_spi::Mcp320xChannels)(ADC_CHANNEL_VBUTTON - 3)] = 0.0;
    _channels[(mcp320x_spi::Mcp320xChannels)(ADC_CHANNEL_VBUTTON - 4)] = 0.0;
    _channels[(mcp320x_spi::Mcp320xChannels)(ADC_CHANNEL_VBUTTON - 5)] = 0.0;
    _channels[(mcp320x_spi::Mcp320xChannels)(ADC_CHANNEL_VBUTTON - 6)] = 0.0;*/
    std::clog << "guimon::mainApp::mainApp: ADC initialized" << std::endl;
  }

  mainApp::~mainApp() {
    std::clog << ">>> guimon::mainApp::~mainApp" << std::endl;
    _adcMgr.close();
    _guiMgr.close();
    // Uninitialize LCD
    _lcd->clear();
    _lcd->cursorOff();
    _lcd->uninitialize();
    _lcd.reset(nullptr);
    // Uninitialize ADC
    _adc.reset(nullptr);

    // Uninitialize the wiringBbb library: done by beagleboneHwInit destructor
  }

  void mainApp::stop() {
    std::clog << ">>> guimon::mainApp::stop" << std::endl;

    // Terminate main loop
    runnable::stop();

    // Reset LCD
    _lcd->setCursor(1, 3);
    std::string str("Stopping...");
    _lcd->write(str);
  }

  void mainApp::run() {
    std::clog << ">>> guimon::mainApp::run" << std::endl;

    _running = true;
    std::vector<unsigned char> buffer;
    while(_running == true) {
      // Check awaiting commands
      int result = _guiMgr.read(buffer, GUI_MQ_SIZE);
      if (result > 0) {
	//	std::clog << "guimon::mainApp::run: Receive " << buffer.size() << " bytes" << std::endl;
	processCommand(buffer);
      }
      buffer.clear();
      // Check inputs
      processAdcChannels();
      // Pause
      usleep(_samplePeriod);
    } // End of 'while' statement

    std::clog << "<<< guimon::mainApp::run" << std::endl;
  }

  void mainApp::processCommand(const std::vector<unsigned char> & p_buffer) {
    //    std::clog << ">>> guimon::mainApp::processCommand" << std::endl;

    std::string cmd(converter::getInstance().bytes2string(p_buffer));
    size_t pos = cmd.find(';');
    if (pos != std::string::npos) {
      // Process the fisrt commands
      do {
	std::string str = cmd.substr(0, pos);
	processLcdCommand(str);
	cmd.erase(cmd.begin(), cmd.begin() + pos + 1);
	//	std::clog << "guimon::mainApp::processCommand: next commands: " << cmd << std::endl;
	pos = cmd.find(';');
      }  while (pos != std::string::npos);
      // cmd contains the last command
    }
    // Process the last command
    processLcdCommand(cmd);
  }

  void mainApp::processLcdCommand(const std::string & p_cmd) {
    //    std::clog << ">>> guimon::mainApp::processLcdCommand: " << p_cmd << std::endl;

    if (p_cmd.compare("CLEAR") == 0) {
      _lcd->clear();
    } else if (p_cmd.compare("HOME") == 0) {
      _lcd->home();
    } else {
      size_t pos = p_cmd.find(',');
      if (pos != std::string::npos) {
	if (p_cmd.substr(0, 5).compare("PRINT") == 0) {
	  //	  std::clog << "guimon::mainApp::processLcdCommand: write " << p_cmd.substr(pos + 1) << std::endl;
	  _lcd->write(p_cmd.substr(pos + 1));
	} else if (p_cmd.substr(0, 5).compare("SETCU") == 0) {
	  // Extract line and column values
	  std::string str(p_cmd.substr(pos + 1)); // Got line,column
	  //	  std::clog << "guimon::mainApp::processLcdCommand: cmd= " << str << std::endl;
	  pos = str.find(','); // Line/column separator
	  int line = std::stoi(str.substr(0, pos).c_str()); // line
	  //	  std::clog << "guimon::mainApp::processLcdCommand: line= " << line << std::endl;
	  //	  std::clog << "guimon::mainApp::processLcdCommand: debug= pos+1=" << pos + 1 << " - " << (int)(str.length() - pos - 1) << std::endl;
	  //	  std::clog << "guimon::mainApp::processLcdCommand: debug=" << str.substr(pos + 1, str.length() - pos - 1).c_str() << std::endl;
	  int column = std::stoi(str.substr(pos + 1).c_str()); // column
	  //	  std::clog << "guimon::mainApp::processLcdCommand: setCursor(" << line << ", " << column << ")" << std::endl;
	  _lcd->setCursor(line, column);
	} else {
	  // Ignore the command
	}
      } else {
	// Ignore the command
      }
    } 
  }

  void mainApp::processAdcChannels() {
    //    std::clog << ">>> guimon::mainApp::processAdcChannels" << std::endl;

    // Read button channel
    for (std::map<mcp320x_spi::Mcp320xChannels, float>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
      it->second = _adc->read(it->first);
      if ((it->first == ADC_CHANNEL_VBUTTON) && (abs(it->second - 0.0001) > 0.0)) {
	unsigned char switchId = 0xff;
	std::clog << "guimon::mainApp::processAdcChannels: Channel #" << (int)it->first << " = " << std::setprecision(5) << (float)it->second << std::endl;
	if (((it->second - V_DELTA) < V_PUSHBUTTON_1) && ((it->second + V_DELTA) > V_PUSHBUTTON_1)) {
	  std::clog << "    ==> SW1 " << std::endl;
	  switchId = PUSHBUTTON_1_PUSHED;
	} else if (((it->second - V_DELTA) < V_PUSHBUTTON_2) && ((it->second + V_DELTA) > V_PUSHBUTTON_2)) {
	  std::clog << "    ==> SW2 " << std::endl;
	  switchId = PUSHBUTTON_2_PUSHED;
	} else if (((it->second - V_DELTA) < V_PUSHBUTTON_3) && ((it->second + V_DELTA) > V_PUSHBUTTON_3)) {
	  std::clog << "    ==> SW3 " << std::endl;
	  switchId = PUSHBUTTON_3_PUSHED;
	} else if (((it->second - V_DELTA) < V_PUSHBUTTON_4) && ((it->second + V_DELTA) > V_PUSHBUTTON_4)) {
	  std::clog << "    ==> SW4 " << std::endl;
	  switchId = PUSHBUTTON_4_PUSHED;
	} else if (((it->second - V_DELTA) < V_PUSHBUTTON_12) && ((it->second + V_DELTA) > V_PUSHBUTTON_12)) {
	  std::clog << "    ==> SW1,2 " << std::endl;
	  switchId = PUSHBUTTON_12_PUSHED;
	} else { 
	  std::clog << "guimon::mainApp::processAdcChannels: Voltage=" << std::setprecision(5) << (float)it->second << std::endl;
	}
	if (switchId != 0xff) {
	  std::vector<unsigned char> buffer;
	  buffer.push_back(GUI_DEVICE_PHB_ID);
	  buffer.push_back(switchId);
	 if (_adcMgr.write(buffer, buffer.size()) < 0) {
	    std::cerr << "guimon::mainApp::processAdcChannels: Failed to write into the message queue: " << std::strerror(errno) << std::endl;
	 }
	}
      } else {
	//	std::clog << "guimon::mainApp::processAdcChannels: Channel #" << (int)it->first << " = " << std::setprecision(5) << (float)it->second << std::endl;
	// TODO To be continued
      }
    } // End of 'for' statement

  }

} // End of namespace guimon
