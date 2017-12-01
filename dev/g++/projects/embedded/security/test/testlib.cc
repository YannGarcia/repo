/*!
 * \file    testlib.cpp
 * \brief   Logger test suite.
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

#include "logger_factory.hh"

#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/oids.h>

#include "sha.hh"
#include "keys_pair.hh"
#include "key.hh"

using namespace std;

/*!
 * \class logger test suite implementation
 */
class security_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};
	
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
 * \sa http://www.sha1-online.com/
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

TEST(security_test_suite, key_sha265) {
  security::key<CryptoPP::ECP, CryptoPP::SHA256> kp(security::sha_algorithms_t::sha256);
  ASSERT_TRUE(kp.generate() == 0);
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
