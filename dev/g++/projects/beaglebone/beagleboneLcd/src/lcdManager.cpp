#include <cstring> // Used for strerror

#include "lcdManager.h"

#include "lcdHD44780U.h"

namespace beagleboneLcd {

  std::unique_ptr<lcdManager> lcdManager::_instance(new lcdManager());

  lcdManager::lcdManager() {
    // Nothing to do
  } // End of constructor

  lcdManager::~lcdManager() {
    // Nothing to do
  } // End of destructor

  int lcdManager::createLcdDevice(const abstractLcd::LcdFamily p_lcdFamily, std::unique_ptr<abstractLcd> & p_lcdInstance) {

    switch (p_lcdFamily) {
    case abstractLcd::HD44780U:
      p_lcdInstance.reset(new lcdHD44780U());
      break;
    } // End of 'switch' statement

    return 0;

  } // End of method createLcdDevice

} // End of namespace beagleboneLcd
