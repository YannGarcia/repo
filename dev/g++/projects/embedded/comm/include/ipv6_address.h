/**
 * @file    ipv6_address.h
 * @brief   Header file for IPv6 address abstraction.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <sstream> // Used istringstream
#include <memory>

#include <arpa/inet.h> // Used for inet_xxx
#include <netinet/in.h> // Used for struct in6_addr

#include "ipvx_address.h"

namespace comm {

  namespace network {

    /**
     * @class ipv4_address
     * @brief This class implements IPv6 address management
     */
    class ipv6_address : public ipvx_address {
    protected:
      struct in6_addr _in_addr; /** The IPv6 address structure */

    public:
      /**
       * @desc Default constructor
       */
      ipv6_address();
      /**
       * @desc Create an IPv6 address
       * @param p_host_ipv4 Host address. "::1" for server (IN6ADDR_ANY)
       */
      ipv6_address(const std::string & p_address);
      ipv6_address(const struct in6_addr * p_in_addr);
      /**
       * @desc Copy constructor
       * @param p_host The ipv6_address object to duplicate
       */
      ipv6_address(const ipv6_address & p_address);
      /**
       * @desc Free internal resources
       */
      virtual ~ipv6_address() { }

      /**
       * @brief Indicate the IP address type
       * @return false
       */
      virtual inline const bool is_ipv4() const { return false; };
      /**
       * @brief Indicate the IP address type
       * @return true
       */
      virtual inline const bool is_ipv6() const { return true; };
      /**
       * @brief Indicate the IP address is a multicast address 
       * @return true if the IP address is a multicast address , false otherwise
       */
      virtual const bool is_multicast() const { return (*(reinterpret_cast<const uint32_t *>(&_in_addr.s6_addr)) & htonl(0xFF000000)) == htonl(0xFF000000); };
      /**
       * @brief Indicate the IP address is a broadcast address 
       * @return true if the IP address is a broadcast address , false otherwise
       */
      virtual const bool is_broadcast() const { return false; };
      /**
       * @brief Indicate the IP address is a localhost address
       * @return true if the IP address is a localhost address , false otherwise
       */
      virtual const bool is_localhost() const { return IN6_IS_ADDR_LOOPBACK(&_in_addr); };
//      virtual const bool is_localhost() const { return htons(_in_addr.__in6_u.__u6_addr16[7]) == htons(0x0100); };
      /**
       * @brief Retrieve the socket address information (struct in6_address *)
       * @return The socket address information on success, NULL otherwise
       */
      virtual inline const void * addr() const { return (const void *)&_in_addr; };
      /**
       * @brief Retrieve the length of the socket address information data structure
       * @return The length of the socket address information data structure
       */
      virtual inline size_t length() const { return sizeof(struct in6_addr); };

      /**
       * @brief Retrieve the socket address in string format
       * @return The socket address in string format
       */
      virtual inline std::string to_string() const {
	char buffer[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, static_cast<const void *>(&_in_addr), buffer, INET6_ADDRSTRLEN);
	std::string ip(static_cast<const char *>(buffer));	
	return ip;
      };
      /**
       * @brief Retrieve the socket address in byte format
       * @return The socket address in byte format
       */
      virtual inline std::vector<uint8_t> to_numeric() const {
	return std::vector<unsigned char>(_in_addr.s6_addr, _in_addr.s6_addr + sizeof(_in_addr.s6_addr));
      };
      
      /**
       * @brief Parse the IPv6 address
       * @param p_address The IPv6 address to parse
       * @return An new instance of ipv6_address class for the parsed address on success, NULL otherwise
       */
      static std::unique_ptr<ipv6_address> parse(const std::string & p_address);
      /**
       * @brief Parse the IPv6 address
       * @param p_address The IPv6 address to parse
       * @param p_result The new instance of ipv6_address on success, NULL otherwise
       * @return true on success, false otherwise
       */
      static bool try_parse(const std::string & p_address, std::unique_ptr<ipv6_address> & p_result);
      
    }; // End of class ipv6_address

  } // End of namespace network

} // End of namespace comm
