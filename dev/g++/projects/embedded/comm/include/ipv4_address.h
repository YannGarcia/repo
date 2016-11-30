/**
 * @file    ipv4_address.h
 * @brief   Header file for IPv4 address abstraction.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <memory>

#include <arpa/inet.h> // Used for inet_xxx
#include <netinet/in.h> // Used for struct in_addr

#include "ipvx_address.h"

namespace comm {

  namespace network {

    /**
     * @class ipv4_address
     * @brief This class implements IPv4 address management
     */
    class ipv4_address : public ipvx_address {
    protected:
      struct in_addr _in_addr; /** The IPv4 address structure */

    public:
      /**
       * @desc Default constructor
       */
      ipv4_address();
      /**
       * @desc Create an IPv6 address
       * @param p_host_ipv4 Host address. "0.0.0.0" for server (INADDR_ANY)
       */
      ipv4_address(const std::string & p_address);
      ipv4_address(const struct in_addr * p_in_addr);
      /**
       * @desc Copy constructor
       * @param p_host The ipv4_address object to duplicate
       */
      ipv4_address(const ipv4_address & p_address);
      /**
       * @desc Free internal resources
       */
      virtual ~ipv4_address() { };

      /**
       * @brief Indicate the IP address type
       * @return true
       */
      virtual inline const bool is_ipv4() const { return true; };
      /**
       * @brief Indicate the IP address type
       * @return false
       */
      virtual inline const bool is_ipv6() const { return false; };
      /**
       * @brief Indicate the IP address is a multicast address 
       * @return true if the IP address is a multicast address , false otherwise
       */
      virtual const bool is_multicast() const { return IN_MULTICAST(ntohl(_in_addr.s_addr)); };
      /**
       * @brief Indicate the IP address is a broadcast address 
       * @return true if the IP address is a broadcast address , false otherwise
       */
      virtual const bool is_broadcast() const { return _in_addr.s_addr == INADDR_NONE; };
      /**
       * @brief Indicate the IP address is a localhost address (127.0.0.1 to 127.255.255.255)
       * @return true if the IP address is a localhost address , false otherwise
       */
      virtual const bool is_localhost() const { return (ntohl(_in_addr.s_addr) & 0xff000000) == 0x7f000000 /* 127.x.x.x*/; };

      /**
       * @brief Retrieve the socket address information (struct in_address *)
       * @return The socket address information on success, NULL otherwise
       */
      virtual inline const void * addr() const { return (const void *)&_in_addr; };
      /**
       * @brief Retrieve the length of the socket address information data structure
       * @return The length of the socket address information data structure
       */
      virtual inline size_t length() const { return sizeof(struct in_addr); };

      /**
       * @brief Retrieve the socket address in string format
       * @return The socket address in string format
       */
      virtual inline std::string to_string() const {
	char buffer[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, static_cast<const void *>(&_in_addr), buffer, INET_ADDRSTRLEN);
	std::string ip(static_cast<const char *>(buffer));
	return ip;
      };
      /**
       * @brief Retrieve the socket address in byte format
       * @return The socket address in byte format
       */
      virtual inline std::vector<uint8_t> to_numeric() const {
	std::vector<uint8_t> buffer(reinterpret_cast<const uint8_t *>(&_in_addr.s_addr), reinterpret_cast<const uint8_t *>(&_in_addr.s_addr) + sizeof(_in_addr.s_addr) / sizeof(uint8_t));
	return buffer;
      };
      
      /**
       * @brief Parse the IPv4 address
       * @param p_address The IPv4 address to parse
       * @return An new instance of ipv4_address class for the parsed address on success, NULL otherwise
       */
      static std::unique_ptr<ipv4_address> parse(const std::string & p_address);
      /**
       * @brief Parse the IPv4 address
       * @param p_address The IPv4 address to parse
       * @param p_result The new instance of ipv4_address on success, NULL otherwise
       * @return true on success, false otherwise
       */
      static bool try_parse(const std::string & p_address, std::unique_ptr<ipv4_address> & p_result);
      
    }; // End of class ipv4_address

  } // End of namespace network

} // End of namespace comm

using namespace comm::network;
