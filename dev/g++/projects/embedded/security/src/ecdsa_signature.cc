#include "ecdsa_signature.hh"

namespace security {

  ecdsa_signature::ecdsa_signature(const keys_pair& p_keys) {
  }
    
  ecdsa_signature::~ecdsa_signature() {
  }

  int ecdsa_signature::sign(const std::vector<unit8_t> p_message, std::vector<unit8_t> p_signature) {
    return -1;
  }
  
  bool ecdsa_signature::verify_sign(const std::vector<unit8_t> p_message, const std::vector<unit8_t> p_signature) {
    return false;
  }

  std::string ecdsa_signature::to_string() {
  }
    
} // End of namespace security

