#include <string>
#include <stdexcept>

class beagleboneGpsEx : public std::runtime_error {
 public:
  explicit beagleboneGpsEx(const std::string & msg) : std::runtime_error(msg) {};

};
