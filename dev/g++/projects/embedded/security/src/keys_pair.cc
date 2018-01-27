/*!
 * \file      keys_pair.cc
 * \brief     Implementation file for the lightweight security library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <sstream>

#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/oids.h>

#include "keys_pair.hh"

#include "logger.hh"

namespace security {

  keys_pair::keys_pair(const sha_algorithms_t p_sha_algorithms): _algorithms(p_sha_algorithms), _private_key(nullptr), _public_key(nullptr), _private_key_v(), _public_key_v_x(), _public_key_v_y() {
    switch (_algorithms) {
    case sha1:
      _private_key.reset(new CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PrivateKey, [p_sha_algorithms](const void* p_key){keys_pair::private_key_ptr_deleter(p_key, p_sha_algorithms);});
      _public_key.reset(new CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PublicKey, [p_sha_algorithms](const void* p_key){keys_pair::public_key_ptr_deleter(p_key, p_sha_algorithms);});
      break;
    case sha256:
      _private_key.reset(new CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey, [p_sha_algorithms](const void* p_key){keys_pair::private_key_ptr_deleter(p_key, p_sha_algorithms);});
      _public_key.reset(new CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey, [p_sha_algorithms](const void* p_key){keys_pair::public_key_ptr_deleter(p_key, p_sha_algorithms);});
      break;
    case sha384:
      _private_key.reset(new CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PrivateKey, [p_sha_algorithms](const void* p_key){keys_pair::private_key_ptr_deleter(p_key, p_sha_algorithms);});
      _public_key.reset(new CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PublicKey, [p_sha_algorithms](const void* p_key){keys_pair::public_key_ptr_deleter(p_key, p_sha_algorithms);});
      break;
    } // End of 'switch' statement
  } // End of contructor

  keys_pair::~keys_pair() {
    _private_key.reset();
    _public_key.reset();
  } // End of destructor

  void keys_pair::private_key_ptr_deleter(const void* p_key, const sha_algorithms_t p_sha_algorithms) {
    switch (p_sha_algorithms) {
    case sha1:
      delete (CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PrivateKey*)p_key;
      break;
    case sha256:
      delete (CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey*)p_key;
      break;
    case sha384:
      delete (CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PrivateKey*)p_key;
      break;
    } // End of 'switch' statement
  } // End of private_key_ptr_deleter

  void keys_pair::public_key_ptr_deleter(const void* p_key, const sha_algorithms_t p_sha_algorithms) {
    switch (p_sha_algorithms) {
    case sha1:
      delete (CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PublicKey*)p_key;
      break;
    case sha256:
      delete (CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey*)p_key;
      break;
    case sha384:
      delete (CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PublicKey*)p_key;
      break;
    } // End of 'switch' statement
  } // End of public_key_ptr_deleter

  int32_t keys_pair::generate() {
    // Sanity check
    if ((_private_key.get() == nullptr) || (_public_key.get() == nullptr)) {
      return -1;
    }

    // Pseudo random number generator
    CryptoPP::AutoSeededRandomPool prng;

    // Generate private key
    int32_t result = -1;
    switch (_algorithms) {
    case sha1:
      ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PrivateKey*)_private_key.get())->Initialize(prng, CryptoPP::ASN1::secp160r1()); // Use Koblitz curve 160 bits ellipctic curve
      // Check result
      if (((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PrivateKey*)_private_key.get())->Validate(prng, 3)) {
        result = 0;
        const CryptoPP::Integer& i = ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey*)_private_key.get())->GetPrivateExponent();
        _private_key_v.resize(i.ByteCount());
        i.Encode(_private_key_v.data(), _private_key_v.size());
      }
      break;
    case sha256:
      ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey*)_private_key.get())->Initialize(prng, CryptoPP::ASN1::secp256r1()); // Use NIST P-256, P-256, prime256v1 ECC curve
      if (((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey*)_private_key.get())->Validate(prng, 3)) {
        result = 0;
        const CryptoPP::Integer& i = ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey*)_private_key.get())->GetPrivateExponent();
        _private_key_v.resize(i.ByteCount());
        i.Encode(_private_key_v.data(), _private_key_v.size());
      }
      break;
    case sha384:
      ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PrivateKey*)_private_key.get())->Initialize(prng, CryptoPP::ASN1::secp384r1()); // Use NIST P-384, P-384, prime384v1 ECC curve
      if (((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PrivateKey*)_private_key.get())->Validate(prng, 3)) {
        result = 0;
        const CryptoPP::Integer& i = ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PrivateKey*)_private_key.get())->GetPrivateExponent();
        _private_key_v.resize(i.ByteCount());
        i.Encode(_private_key_v.data(), _private_key_v.size());
      }
      break;
    } // End of 'switch' statement

    if (result == -1) {
      return -1;
    }

    // Geerate public key
    result = -1;
    switch (_algorithms) {
    case sha1:
      ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PrivateKey*)_private_key.get())->MakePublicKey(*((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PublicKey*)_public_key.get()));
      // Check result
      if (((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PublicKey*)_public_key.get())->Validate(prng, 3)) {
        result = 0;
        const CryptoPP::ECPPoint& e = ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA1>::PublicKey*)_public_key.get())->GetPublicElement();
        _public_key_v_x.resize(e.x.ByteCount());
        e.x.Encode(_public_key_v_x.data(), _public_key_v_x.size());
        _public_key_v_y.resize(e.y.ByteCount());
        e.y.Encode(_public_key_v_y.data(), _public_key_v_y.size());
      }
      break;
    case sha256:
      ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey*)_private_key.get())->MakePublicKey(*((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey*)_public_key.get()));
      // Check result
      if (((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey*)_public_key.get())->Validate(prng, 3)) {
        result = 0;
        const CryptoPP::ECPPoint& e = ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey*)_public_key.get())->GetPublicElement();
        _public_key_v_x.resize(e.x.ByteCount());
        e.x.Encode(_public_key_v_x.data(), _public_key_v_x.size());
        _public_key_v_y.resize(e.y.ByteCount());
        e.y.Encode(_public_key_v_y.data(), _public_key_v_y.size());
      }
      break;
    case sha384:
      ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PrivateKey*)_private_key.get())->MakePublicKey(*((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PublicKey*)_public_key.get()));
      // Check result
      if (((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PublicKey*)_public_key.get())->Validate(prng, 3)) {
        result = 0;
        const CryptoPP::ECPPoint& e = ((CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA384>::PublicKey*)_public_key.get())->GetPublicElement();
        _public_key_v_x.resize(e.x.ByteCount());
        e.x.Encode(_public_key_v_x.data(), _public_key_v_x.size());
        _public_key_v_y.resize(e.y.ByteCount());
        e.y.Encode(_public_key_v_y.data(), _public_key_v_y.size());
      }
      break;
    } // End of 'switch' statement

    // Separate the public key in two parts
    
    return 0;
  }
  std::string keys_pair::to_string(){
    std::ostringstream os;

    os << "Keys_pait dump: "
       << "\tDomain parameters: "
       << std::endl;

    return os.str();
  }
  
} // End of namespace security

