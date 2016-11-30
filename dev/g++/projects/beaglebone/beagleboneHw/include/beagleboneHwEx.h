#include <string>
#include <stdexcept>

class beagleboneHwEx : public std::runtime_error {
 public:
  explicit beagleboneHwEx(const std::string & msg) : std::runtime_error(msg) {};

}; // End of class beagleboneHwEx
