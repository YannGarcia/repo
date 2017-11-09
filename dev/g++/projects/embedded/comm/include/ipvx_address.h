/**
 * \file      ipvx_address.h
 * \brief     Header file for IPv4/IPv6 address abstraction.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <string>
#include <vector>

namespace comm {

  namespace network {

    /**
     * \class ipvx_address (abstract)
     * \brief This class implements abstraction for IPv4/IPv6 address management
     *
     * \see ipv4_address 
     * \see ipv6_address 
     */
    class ipvx_address {
    public:
      virtual ~ipvx_address() { };
      /**
       * \brief Indicate the IP address type
       * \return true if the IP address is IPv4, false otherwise
       */
      virtual const bool is_ipv4() const = 0;
      /**
       * \brief Indicate the IP address type
       * \return true if the IP address is IPv6, false otherwise
       */
      virtual const bool is_ipv6() const = 0;
      /**
       * \brief Indicate the IP address is a multicast address 
       * \return true if the IP address is a multicast address , false otherwise
       */
      virtual const bool is_multicast() const = 0;
      /**
       * \brief Indicate the IP address is a broadcast address 
       * \return true if the IP address is a broadcast address , false otherwise
       */
      virtual const bool is_broadcast() const = 0;
      /**
       * \brief Indicate the IP address is a localhost address
       * \return true if the IP address is a localhost address , false otherwise
       */
      virtual const bool is_localhost() const = 0;

      /**
       * \brief Retrieve the socket address information (struct in_address * for IPv4 or struct in6_address * for IPv6)
       * \return The socket address information on success, NULL otherwise
       */
      virtual const void * addr() const = 0;
      /**
       * \brief Retrieve the length of the socket address information data structure
       * \return The length of the socket address information data structure
       */
      virtual size_t length() const = 0;

      /**
       * \brief Retrieve the socket address in string format
       * \return The socket address in string format
       */
      virtual std::string to_string() const  = 0;
      /**
       * \brief Retrieve the socket address in byte format
       * \return The socket address in byte format
       */
      virtual std::vector<uint8_t> to_numeric() const  = 0;
    }; // End of class ipvx_address

  } // End of namespace network

} // End of namespace comm

using namespace comm::network;
