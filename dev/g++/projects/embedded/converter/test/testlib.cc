/*!
 * \file      testlib.cpp
 * \brief     Types converter test suite.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <gtest.h>

#include "converter.hh"
#include "helper.hh"

using namespace std;

/**
 * @class Type converter test suite implementation
 */
class converter_test_suite : public ::testing::Test {
protected:
  virtual void SetUp() { };
  virtual void TearDown() { };
};

/**
 * @brief Test case for @see converter::bin_to_bcd
 */
TEST(converter_test_suite, bin_to_bcd) {
  ASSERT_TRUE((int)converter::get_instance().bin_to_bcd(0x00) == (int)0x00);
  ASSERT_TRUE((int)converter::get_instance().bin_to_bcd(0x01) == (int)0x01);
  ASSERT_TRUE((int)converter::get_instance().bin_to_bcd(0x0A) == (int)0x10);
  ASSERT_TRUE((int)converter::get_instance().bin_to_bcd(0x0F) == (int)0x15);
}

/**
 * @brief Test case for @see converter::bcd_to_bin
 */
TEST(converter_test_suite, bcd_to_bin) {
  ASSERT_TRUE((int)converter::get_instance().bcd_to_bin(0x00) == (int)0x00);
  ASSERT_TRUE((int)converter::get_instance().bcd_to_bin(0x01) == (int)0x01);
  ASSERT_TRUE((int)converter::get_instance().bcd_to_bin(0x13) == (int)0x0D);
  ASSERT_TRUE((int)converter::get_instance().bcd_to_bin(0x15) == (int)0x0F);
}

/**
 * @brief Test case for @see converter::string_to_bytes
 */
TEST(converter_test_suite, string_to_bytes) {
  std::string frombytes("Hello");
  std::vector<uint8_t> to_bytes(converter::get_instance().string_to_bytes(frombytes));
  ASSERT_TRUE(to_bytes.size() == 5);
  uint8_t expected_result[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::bytes_to_string
 */
TEST(converter_test_suite, bytes_to_string) {
  std::vector<uint8_t> from_bytes = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
  ASSERT_TRUE(converter::get_instance().bytes_to_string(from_bytes).compare("Hello") == 0);
}

/**
 * @brief Test case for @see converter::swap
 */
TEST(converter_test_suite, swap) {
  ASSERT_TRUE(converter::get_instance().swap(static_cast<const uint16_t>(0xcafe)) == 0xfeca);
  ASSERT_TRUE(converter::get_instance().swap(static_cast<const uint32_t>(0xcafedeca)) == 0xcadefeca);
}

/**
 * @brief Test case for @see converter::hexa_to_bytes
 */
TEST(converter_test_suite, hexa_to_bytes) {
  string str("5f83af3bfb6c5a8c5ef0794cc6d4cff418ef4cbb09b2785801477a92907ba828");
  std::vector<uint8_t> to_bytes = converter::get_instance().hexa_to_bytes(str);
  ASSERT_TRUE(to_bytes.size() == 32);
  uint8_t expected_result[] = { 0x5f, 0x83, 0xaf, 0x3b, 0xfb, 0x6c, 0x5a, 0x8c, 0x5e, 0xf0, 0x79, 0x4c, 0xc6, 0xd4, 0xcf, 0xf4, 0x18, 0xef, 0x4c, 0xbb, 0x09, 0xb2, 0x78, 0x58, 0x01, 0x47, 0x7a, 0x92, 0x90, 0x7b, 0xa8, 0x28 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::hexa_to_bytes
 */
TEST(converter_test_suite, bytes_to_hexa) {
  std::vector<uint8_t> from_bytes = { 0x5f, 0x83, 0xaf, 0x3b, 0xfb, 0x6c, 0x5a, 0x8c, 0x5e, 0xf0, 0x79, 0x4c, 0xc6, 0xd4, 0xcf, 0xf4, 0x18, 0xef, 0x4c, 0xbb, 0x09, 0xb2, 0x78, 0x58, 0x01, 0x47, 0x7a, 0x92, 0x90, 0x7b, 0xa8, 0x28 };
  string str = converter::get_instance().bytes_to_hexa(from_bytes);
  ASSERT_TRUE(str.compare("5f83af3bfb6c5a8c5ef0794cc6d4cff418ef4cbb09b2785801477a92907ba828") == 0);
}

/**
 * @brief Test case for @see converter::int_to_bytes
 */
TEST(converter_test_suite, int_to_bytes_1) {
  vector<uint8_t> to_bytes = converter::get_instance().int_to_bytes(3405700810);
  ASSERT_TRUE(to_bytes.size() == sizeof(int));
  uint8_t expected_result[] = { 0xca, 0xfe, 0xde, 0xca };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::int_to_bytes
 */
TEST(converter_test_suite, int_to_bytes_2) {
  vector<uint8_t> to_bytes = converter::get_instance().int_to_bytes(2434158052);
  ASSERT_TRUE(to_bytes.size() == sizeof(int));
  uint8_t expected_result[] = { 0x91, 0x16, 0x4d, 0xe4 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::int_to_bytes
 */
TEST(converter_test_suite, int_to_bytes_3) {
  vector<uint8_t> to_bytes = converter::get_instance().int_to_bytes(3830257297);
  ASSERT_TRUE(to_bytes.size() == sizeof(int));
  uint8_t expected_result[] = { 0xe4, 0x4d, 0x16, 0x91 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::bytes_to_int
 */
TEST(converter_test_suite, bytes_to_int_1) {
  vector<uint8_t> from_bytes = { 0xca, 0xfe, 0xde, 0xca };
  ASSERT_TRUE(converter::get_instance().bytes_to_int(from_bytes) == static_cast<int>(3405700810));
}

/**
 * @brief Test case for @see converter::bytes_to_int
 */
TEST(converter_test_suite, bytes_to_int_2) {
  vector<uint8_t> from_bytes = { 0x91, 0x16, 0x4d, 0xe4 };
  ASSERT_TRUE(converter::get_instance().bytes_to_int(from_bytes) == static_cast<int>(2434158052));
}

/**
 * @brief Test case for @see converter::bytes_to_int
 */
TEST(converter_test_suite, bytes_to_int_3) {
  vector<uint8_t> from_bytes = { 0xe4, 0x4d, 0x16, 0x91 };
  ASSERT_TRUE(converter::get_instance().bytes_to_int(from_bytes) == static_cast<int>(3830257297));
}

/**
 * @brief Test case for @see converter::long_to_bytes
 */
TEST(converter_test_suite, long_to_bytes) {
  vector<uint8_t> to_bytes = converter::get_instance().long_to_bytes(static_cast<int64_t>(4621188755229004256));
  ASSERT_TRUE(to_bytes.size() == sizeof(int64_t));
  uint8_t expected_result[] = { 0x40, 0x21, 0xc2, 0xb0, 0x53, 0x74, 0x51, 0xe0 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::bytes_to_long
 */
TEST(converter_test_suite, bytes_to_long) {
  vector<uint8_t> from_bytes = { 0x40, 0x21, 0xc2, 0xb0, 0x53, 0x74, 0x51, 0xe0 };
  ASSERT_TRUE(converter::get_instance().bytes_to_long(from_bytes) == static_cast<int64_t>(4621188755229004256));
}

/**
 * @brief Test case for @see converter::float_to_bytes
 */
TEST(converter_test_suite, float_to_bytes_1) {
  vector<uint8_t> to_bytes = converter::get_instance().float_to_bytes(3.141519);
  ASSERT_TRUE(to_bytes.size() == sizeof(float));
  uint8_t expected_result[] = { 0xa6, 0x0e, 0x49, 0x40 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::float_to_bytes
 */
TEST(converter_test_suite, float_to_bytes_2) {
  vector<uint8_t> to_bytes = converter::get_instance().float_to_bytes(-3.141519);
  ASSERT_TRUE(to_bytes.size() == sizeof(float));
  uint8_t expected_result[] = { 0xa6, 0x0e, 0x49, 0xc0 };
  ASSERT_TRUE(std::equal(to_bytes.begin(), to_bytes.end(), expected_result));
}

/**
 * @brief Test case for @see converter::bytes_to_float
 */
TEST(converter_test_suite, bytes_to_float_1) {
  vector<uint8_t> from_bytes = { 0xa6, 0x0e, 0x49, 0x40 };
  ASSERT_TRUE(converter::get_instance().bytes_to_float(from_bytes) == static_cast<float>(3.141519));
}

/**
 * @brief Test case for @see converter::bytes_to_float
 */
TEST(converter_test_suite, bytes_to_float_2) {
  vector<uint8_t> from_bytes = { 0xa6, 0x0e, 0x49, 0xc0 };
  ASSERT_TRUE(converter::get_instance().bytes_to_float(from_bytes) == static_cast<float>(-3.141519));
}

TEST(converter_test_suite, time_to_string_1) {
  time_t current_time = 1489755780;
  std::string str = converter::get_instance().time_to_string(current_time);
  std::string expected_result_1("Fri, 17 Mar 2017 14:03:00 +0100");
  std::string expected_result_2("Fri, 17 Mar 2017 13:03:00 +0000");
  ASSERT_TRUE((str.compare(expected_result_1) == 0) || (str.compare(expected_result_2) == 0)); // http://www.unixtimestamp.com/
}

/**
 * @brief Test case for @see converter::trim
 */
TEST(converter_test_suite, trim) {
  std::string str = "   A  lot of  white spaces    ";
  std::string trimmedStr = converter::get_instance().trim(str);
  ASSERT_TRUE(trimmedStr.compare("A  lot of  white spaces") == 0);
}

/**
 * @brief Test case for @see converter::split_arguments_line
 */
TEST(converter_test_suite, split_arguments_line_1) {
  std::string str = "--host localhost --port 12345 --duration -1";
  std::vector<std::string> tokens = converter::get_instance().split_arguments_line(str);
  std::clog << "Tokens: " << std::endl;
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    std::clog << "   " << *it << std::endl;
  }
  ASSERT_TRUE(tokens.size() == 4);
}

/**
 * @brief Test case for @see converter::string_to_base64
 */
TEST(converter_test_suite, string_to_base64_1) {
  std::string plain_text = "Decode from Base64 format";
  std::string expected_result = "RGVjb2RlIGZyb20gQmFzZTY0IGZvcm1hdA==";
  std::string base64 = converter::get_instance().string_to_base64(plain_text);
  ASSERT_TRUE(base64.compare(expected_result) == 0);
}

/**
 * @brief Test case for @see converter::string_to_base64
 */
TEST(converter_test_suite, string_to_base64_2) {
  std::string plain_text = "In computer science, Base64 is a group of binary-to-text encoding schemes that represent binary data in an ASCII string format by translating it into a radix-64 representation. The term Base64 originates from a specific MIME content transfer encoding. Each Base64 digit represents exactly 6 bits of data. Three 8-bit bytes (i.e., a total of 24 bits) can therefore be represented by four 6-bit Base64 digits.\n\nCommon to all binary-to-text encoding schemes, Base64 is designed to carry data stored in binary formats across channels that only reliably support text content. Base64 is particularly prevalent on the World Wide Web[1] where its uses include the ability to embed image files or other binary assets inside textual assets such as HTML and CSS files.\n";
  std::string expected_result = "SW4gY29tcHV0ZXIgc2NpZW5jZSwgQmFzZTY0IGlzIGEgZ3JvdXAgb2YgYmluYXJ5LXRvLXRleHQgZW5jb2Rpbmcgc2NoZW1lcyB0aGF0IHJlcHJlc2VudCBiaW5hcnkgZGF0YSBpbiBhbiBBU0NJSSBzdHJpbmcgZm9ybWF0IGJ5IHRyYW5zbGF0aW5nIGl0IGludG8gYSByYWRpeC02NCByZXByZXNlbnRhdGlvbi4gVGhlIHRlcm0gQmFzZTY0IG9yaWdpbmF0ZXMgZnJvbSBhIHNwZWNpZmljIE1JTUUgY29udGVudCB0cmFuc2ZlciBlbmNvZGluZy4gRWFjaCBCYXNlNjQgZGlnaXQgcmVwcmVzZW50cyBleGFjdGx5IDYgYml0cyBvZiBkYXRhLiBUaHJlZSA4LWJpdCBieXRlcyAoaS5lLiwgYSB0b3RhbCBvZiAyNCBiaXRzKSBjYW4gdGhlcmVmb3JlIGJlIHJlcHJlc2VudGVkIGJ5IGZvdXIgNi1iaXQgQmFzZTY0IGRpZ2l0cy4KCkNvbW1vbiB0byBhbGwgYmluYXJ5LXRvLXRleHQgZW5jb2Rpbmcgc2NoZW1lcywgQmFzZTY0IGlzIGRlc2lnbmVkIHRvIGNhcnJ5IGRhdGEgc3RvcmVkIGluIGJpbmFyeSBmb3JtYXRzIGFjcm9zcyBjaGFubmVscyB0aGF0IG9ubHkgcmVsaWFibHkgc3VwcG9ydCB0ZXh0IGNvbnRlbnQuIEJhc2U2NCBpcyBwYXJ0aWN1bGFybHkgcHJldmFsZW50IG9uIHRoZSBXb3JsZCBXaWRlIFdlYlsxXSB3aGVyZSBpdHMgdXNlcyBpbmNsdWRlIHRoZSBhYmlsaXR5IHRvIGVtYmVkIGltYWdlIGZpbGVzIG9yIG90aGVyIGJpbmFyeSBhc3NldHMgaW5zaWRlIHRleHR1YWwgYXNzZXRzIHN1Y2ggYXMgSFRNTCBhbmQgQ1NTIGZpbGVzLgo=";
  std::string base64 = converter::get_instance().string_to_base64(plain_text);
  ASSERT_TRUE(base64.compare(expected_result) == 0);
}

/**
 * @brief Test case for @see converter::.base64_to_string
 */
TEST(converter_test_suite,  base64_to_string_1) {
  std::string expected_result = "Decode from Base64 format";
  std::string base64 = "RGVjb2RlIGZyb20gQmFzZTY0IGZvcm1hdA==";
  std::string plain_text = converter::get_instance().base64_to_string(base64);
  ASSERT_TRUE(plain_text.compare(expected_result) == 0);
}

/**
 * @brief Test case for @see converter::.base64_to_string
 */
TEST(converter_test_suite, base64_to_string_2) {
  std::string expected_result = "In computer science, Base64 is a group of binary-to-text encoding schemes that represent binary data in an ASCII string format by translating it into a radix-64 representation. The term Base64 originates from a specific MIME content transfer encoding. Each Base64 digit represents exactly 6 bits of data. Three 8-bit bytes (i.e., a total of 24 bits) can therefore be represented by four 6-bit Base64 digits.\n\nCommon to all binary-to-text encoding schemes, Base64 is designed to carry data stored in binary formats across channels that only reliably support text content. Base64 is particularly prevalent on the World Wide Web[1] where its uses include the ability to embed image files or other binary assets inside textual assets such as HTML and CSS files.\n";
  std::string base64 = "SW4gY29tcHV0ZXIgc2NpZW5jZSwgQmFzZTY0IGlzIGEgZ3JvdXAgb2YgYmluYXJ5LXRvLXRleHQgZW5jb2Rpbmcgc2NoZW1lcyB0aGF0IHJlcHJlc2VudCBiaW5hcnkgZGF0YSBpbiBhbiBBU0NJSSBzdHJpbmcgZm9ybWF0IGJ5IHRyYW5zbGF0aW5nIGl0IGludG8gYSByYWRpeC02NCByZXByZXNlbnRhdGlvbi4gVGhlIHRlcm0gQmFzZTY0IG9yaWdpbmF0ZXMgZnJvbSBhIHNwZWNpZmljIE1JTUUgY29udGVudCB0cmFuc2ZlciBlbmNvZGluZy4gRWFjaCBCYXNlNjQgZGlnaXQgcmVwcmVzZW50cyBleGFjdGx5IDYgYml0cyBvZiBkYXRhLiBUaHJlZSA4LWJpdCBieXRlcyAoaS5lLiwgYSB0b3RhbCBvZiAyNCBiaXRzKSBjYW4gdGhlcmVmb3JlIGJlIHJlcHJlc2VudGVkIGJ5IGZvdXIgNi1iaXQgQmFzZTY0IGRpZ2l0cy4KCkNvbW1vbiB0byBhbGwgYmluYXJ5LXRvLXRleHQgZW5jb2Rpbmcgc2NoZW1lcywgQmFzZTY0IGlzIGRlc2lnbmVkIHRvIGNhcnJ5IGRhdGEgc3RvcmVkIGluIGJpbmFyeSBmb3JtYXRzIGFjcm9zcyBjaGFubmVscyB0aGF0IG9ubHkgcmVsaWFibHkgc3VwcG9ydCB0ZXh0IGNvbnRlbnQuIEJhc2U2NCBpcyBwYXJ0aWN1bGFybHkgcHJldmFsZW50IG9uIHRoZSBXb3JsZCBXaWRlIFdlYlsxXSB3aGVyZSBpdHMgdXNlcyBpbmNsdWRlIHRoZSBhYmlsaXR5IHRvIGVtYmVkIGltYWdlIGZpbGVzIG9yIG90aGVyIGJpbmFyeSBhc3NldHMgaW5zaWRlIHRleHR1YWwgYXNzZXRzIHN1Y2ggYXMgSFRNTCBhbmQgQ1NTIGZpbGVzLgo=";
  std::string plain_text = converter::get_instance().base64_to_string(base64);
  ASSERT_TRUE(plain_text.compare(expected_result) == 0);
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
