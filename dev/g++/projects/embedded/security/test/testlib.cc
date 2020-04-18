/*!
 * \file    testlib.cpp
 * \brief   Security test suite.
 * \author garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license This project is released under the MIT License
 * \version 0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <gtest.h>
#define ASSERT_TRUE_MSG(exp1, msg) ASSERT_TRUE(exp1) << msg

#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/oids.h>

#include "rnd.hh"
#include "sha.hh"
#include "keys_pair.hh"
#include "key.hh"

using namespace std;

/*!
 * \class security test suite implementation
 */
class security_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/*!
 * \brief Test case for @see security::rnd
 */
TEST(security_test_suite, rnd) {
  std::vector<uint8_t> iv;
  ASSERT_TRUE(security::rnd::get_instance().generate(32, iv) == 32);
  ASSERT_TRUE(security::rnd::get_instance().generate(48, iv) == 48);
  ASSERT_TRUE(security::rnd::get_instance().generate(2048, iv) == 2048);
}

/*!
 * \brief Test case for @see security::sha / SHA-1
 * \sa http://www.sha1-online.com/
 */
TEST(security_test_suite, sha1) {
  security::sha sha1(security::sha_algorithms_t::sha1);
  std::vector<uint8_t> data;
  data.push_back('a');
  data.push_back('b');
  data.push_back('c');
  std::vector<uint8_t> result;
  ASSERT_TRUE(sha1.hash(data, result) == 0);
  uint8_t b[] = { 0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D };
  std::vector<uint8_t> expected_result(b, b + sizeof(b));
  ASSERT_TRUE(std::equal(expected_result.cbegin(), expected_result.cend(), result.cbegin()));
}

/*!
 * \brief Test case for @see security::sha / SHA-256
 * \sa http://www.convertstring.com/Hash/SHA256
 */
TEST(security_test_suite, sha256) {
  security::sha sha256(security::sha_algorithms_t::sha256);
  std::vector<uint8_t> data;
  data.push_back('a');
  data.push_back('b');
  data.push_back('c');
  std::vector<uint8_t> result;
  ASSERT_TRUE(sha256.hash(data, result) == 0);
  uint8_t b[] = { 0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23, 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD };
  std::vector<uint8_t> expected_result(b, b + sizeof(b));
  ASSERT_TRUE(result.size() == 32);
  ASSERT_TRUE(expected_result.size() == 32);
  ASSERT_TRUE(std::equal(expected_result.cbegin(), expected_result.cend(), result.cbegin()));
}

/*!
 * \brief Test case for @see security::sha / SHA-256
 * \sa http://www.convertstring.com/Hash/SHA256
 */
TEST(security_test_suite, sha384) {
  security::sha sha384(security::sha_algorithms_t::sha384);
  std::vector<uint8_t> data;
  data.push_back('a');
  data.push_back('b');
  data.push_back('c');
  std::vector<uint8_t> result;
  ASSERT_TRUE(sha384.hash(data, result) == 0);
  uint8_t b[] = { 0xCB, 0x00, 0x75, 0x3F, 0x45, 0xA3, 0x5E, 0x8B, 0xB5, 0xA0, 0x3D, 0x69, 0x9A, 0xC6, 0x50, 0x07, 0x27, 0x2C, 0x32, 0xAB, 0x0E, 0xDE, 0xD1, 0x63, 0x1A, 0x8B, 0x60, 0x5A, 0x43, 0xFF, 0x5B, 0xED, 0x80, 0x86, 0x07, 0x2B, 0xA1, 0xE7, 0xCC, 0x23, 0x58, 0xBA, 0xEC, 0xA1, 0x34, 0xC8, 0x25, 0xA7 };
  std::vector<uint8_t> expected_result(b, b + sizeof(b));
  ASSERT_TRUE(std::equal(expected_result.cbegin(), expected_result.cend(), result.cbegin()));
}

/*!
 * \brief Test case for @see security::keys_pair / SHA-1
 * \sa http://www.sha1-online.com/
 */
TEST(security_test_suite, kp_sha1) {
  security::keys_pair kp(security::sha_algorithms_t::sha1);
  ASSERT_TRUE(kp.generate() == 0);
  std::clog << "Keys pair indo: " << kp.to_string() << std::endl;
}

TEST(security_test_suite, key_sha256) {
  security::key<CryptoPP::ECP, CryptoPP::SHA256> kp(security::sha_algorithms_t::sha256);
  ASSERT_TRUE(kp.generate() == 0);
  std::clog << "Keys pair indo: " << kp.to_string() << std::endl;
}

TEST(security_test_suite, key_sha384) {
  security::key<CryptoPP::ECP, CryptoPP::SHA384> kp(security::sha_algorithms_t::sha384);
  ASSERT_TRUE(kp.generate() == 0);
  std::clog << "Keys pair indo: " << kp.to_string() << std::endl;
}

/**
 * @brief Main test program
 * @param[in] p_argc Number of argumrnt
 * @param[in] p_argv List of the arguments
 */
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
