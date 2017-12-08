/*!
 * \file      key.t.h
 * \brief     Header file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
namespace security {

  /*!
   * \class key
   * \brief KEY-x class to be used to produce message digest based on KEY-x algorithm
   */
  template<class T1, class T2>
    key<T1, T2>::key(const sha_algorithms_t p_sha_algorithms): _algorithms(p_sha_algorithms), _key(), _key_vector() {
  }; // End of contructor
  
  /*!
   * \brief Destructor
   */
  template<class T1, class T2>
    key<T1, T2>::~key() {
  }; // End of destructor
  
  template<class T1, class T2>
    int32_t key<T1, T2>::generate() {
    
    // Pseudo random number generator
    CryptoPP::AutoSeededRandomPool prng;
    
    // Generate private key
    int32_t result = -1;
    _key.Initialize(prng, CryptoPP::ASN1::secp160r1()); // Use Koblitz curve 160 bits ellipctic curve
    // Check result
    if (_key.Validate(prng, 3)) {
      result = 0;
      const CryptoPP::Integer& i = _key.GetPrivateExponent();
      _key_vector.resize(i.ByteCount());
      i.Encode(_key_vector.data(), _key_vector.size());
    }
  }
    
  template<class T1, class T2>
    std::string key<T1, T2>::to_string() {
    std::ostringstream os;
    
    os << "Keys_pait dump: "
       << "\tDomain parameters: "
       << std::endl;
    
    return os.str();
  };
  
} // End of namespace
