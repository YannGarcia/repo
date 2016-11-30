#if !defined(__LCDMANAGER_H__)
#define __LCDMANAGER_H__

#include <stdexcept> // std::out_of_range
#include <memory> // Used for unique_ptr

#include "abstractLcd.h"

namespace beagleboneLcd {

  class lcdManager {

    static std::unique_ptr<lcdManager> _instance;

  public:
    inline static lcdManager & getInstance() { return *_instance; };
    
  private:
    lcdManager();
    
  public:
    ~lcdManager();
    
    int createLcdDevice(const abstractLcd::LcdFamily p_lcdFamily, std::unique_ptr<abstractLcd> & p_lcdInstance);
  }; // End of class lcdManager

} // End of namespace beagleboneLcd

using namespace beagleboneLcd;

#endif // __LCDMANAGER_H__
