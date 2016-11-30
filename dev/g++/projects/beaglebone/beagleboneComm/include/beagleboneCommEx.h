#include <string>
#include <stdexcept>

class beagleboneCommEx : public std::runtime_error {
 public:
  explicit beagleboneCommEx(const std::string & msg) : std::runtime_error(msg) {};

};
