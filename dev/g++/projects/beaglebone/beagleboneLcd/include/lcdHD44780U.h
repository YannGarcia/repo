#if !defined(__LCDHD44780U_H__)
#define __LCDHD44780U_H__

#include <memory> // Used for unique_ptr
#include <vector>

#include "abstractLcd.h"

#include "digitalOut.h"

namespace beagleboneLcd {

  /**
   * @desc 
   * \remark This implementation uses the wiringBbb library. It assumes this libary was set up before to instanciate this class 
   * \see http://www.8051projects.net/lcd-interfacing/lcd-4-bit.php
   */
  class lcdHD44780U : public abstractLcd {
    // Commands
    const unsigned char ClearDisplay = 0x01;
    const unsigned char CursorHome = 0x02;
    const unsigned char SetEntryMode = 0x04;
    const unsigned char DisplayMode = 0x08;
    const unsigned char ScrollMode = 0x10;
    // Flags for display on/off control
    const unsigned char DisplayOn = 0x04;
    const unsigned char CursorOn = 0x02;
    const unsigned char BlinkingOn = 0x01;
    // Flags for display entry mode
    const unsigned char EntryLeft = 0x02;
    const unsigned char EntryShiftInc = 0x01;
    // Flags for display/cursor shift
    const unsigned char DisplayMove = 0x08;
    const unsigned char CursorMove = 0x00;
    const unsigned char CursorMoveLeft = 0x00;
    const unsigned char CursorMoveRight = 0x04;
    const unsigned char DisplayOnCursorOnAndBlinkingOn = DisplayMode | DisplayOn | CursorOn | BlinkingOn;
    const unsigned char DisplayOnCursorOnAndBlinkingOff =  DisplayMode | DisplayOn | CursorOn;

    digitalOut *_en;
    digitalOut *_rs;
    digitalOut *_rw;

    unsigned char _displayStatus;

  public:
    lcdHD44780U();
    ~lcdHD44780U();

    virtual int initialize(const abstractLcd::LcdMode p_mode, const abstractLcd::LcdType p_type, const std::vector<unsigned char> & p_datas, const unsigned char p_en, const unsigned char p_rs = 0xff, const unsigned char p_rw = 0xff);
    virtual int uninitialize();

    void setPowerMode(const abstractLcd::LcdPowerMode p_powerMode = abstractLcd::mode5V, const int p_pwmVlcd = 0xff);

    inline void clear() { command(ClearDisplay); home(); };
    inline void home() { command(CursorHome); _row = _column = 0; };
    inline void setCursor(const int p_row, const int p_column) { _row = p_row; _column = p_column; };
    inline void displayOff() { _displayStatus &= ~DisplayOn; command(_displayStatus); };
    inline void displayOn() { _displayStatus |= DisplayOn; command(_displayStatus); };
    inline void blinkOff() { _displayStatus &= ~BlinkingOn; command(_displayStatus); };
    inline void blinkOn() { _displayStatus |= BlinkingOn; command(_displayStatus); };
    inline void cursorOff() { _displayStatus &= ~CursorOn; command(_displayStatus); };
    inline void cursorOn() { _displayStatus |= CursorOn; command(_displayStatus); };
    inline void scrollDisplayLeft() { command(ScrollMode | DisplayMode | CursorMoveLeft); };
    inline void scrollDisplayRight() { command(ScrollMode | DisplayMode | CursorMoveRight); };
    inline void leftToRight() { _displayStatus |= EntryLeft; command(SetEntryMode |_displayStatus); };
    inline void rightToLeft() { _displayStatus &= ~EntryLeft; command(SetEntryMode | _displayStatus); };
    inline void autoscrollOn() { _displayStatus |= EntryShiftInc; command(SetEntryMode | _displayStatus); };
    inline void autoscrollOff() { _displayStatus &= ~EntryShiftInc; command(SetEntryMode | _displayStatus); };
    int write(const std::string & p_line);

    virtual int loadFont(const std::vector<unsigned char> p_font) { return -1; };

  private:
    int configure();
    void command(const unsigned char p_command);

    /**
     * @desc Write data on the LCD port
     *       In 4bits mode: LSB contains 
     * \param 
     * \return 
     */
    int write(const unsigned char p_data, const bool p_waitBF = true);
    int write(const int p_row, const int p_column, const char p_char);

    unsigned char setAddress(const int p_row, const int column);

    /**
     * @desc Wait for BF (Busy Flag) unset 
     * \param 
     * \return 
     */
    int waitBF();

  private:
    inline void setRS() { _rs->write(1); };
    inline void unsetRS() { _rs->write(0); };
    inline void setRW() { _rw->write(1); };
    inline void unsetRW() { _rw->write(0); };
    inline void setEN() { _en->write(1); };
    inline void unsetEN() { _en->write(0); };

  }; // End of class lcdHD44780U
  
} // End of namespace beagleboneLcd

using namespace beagleboneLcd;

#endif // __LCDHD44780U_H__
