/**
 * @file    socket_address.h
 * @brief   Header file for IPv4/IPv6 address facade.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "ipvx_address.h"

namespace comm {

  namespace network {

    /**
     * @class socket_address (implement ipvx_address)
     * @brief This class unifies the IP socket address management (facade for IPv4/IPv6 socket management) 
     *
     * @see ipv4_address
     * @see ipv6_address
     */
    class socket_address : public ipvx_address {
    protected:
      std::unique_ptr<ipvx_address> _address; /** The IP address structure */
      uint16_t _port;                         /** The port number */
      
    public:
      /**
       * @desc Default constructor
       */
      socket_address() : _address(nullptr), _port(-1) { };
      /**
       * @desc Create an IP socket address
       * @param p_host The host address
       * @param p_port The port number
       */
      socket_address(const std::string & p_host, const uint16_t p_port);
      /**
       * @desc Copy constructor
       * @param p_host The socket_address object to duplicate
       */
      socket_address(const socket_address & p_host);
      /**
       * @desc Free internal resources
       */
      virtual ~socket_address() { _address.reset(); };
      
      /**
       * @brief Indicate the IP address type
       * @return true if the IP address is IPv4, false otherwise
       */
      inline const bool is_ipv4() const { return (_address.get() == NULL) ? false : _address->is_ipv4(); };
      /**
       * @brief Indicate the IP address type
       * @return true if the IP address is IPv6, false otherwise
       */
      virtual inline const bool is_ipv6() const { return (_address.get() == NULL) ? false : _address->is_ipv6(); };
      /**
       * @brief Indicate the IP address is a multicast address 
       * @return true if the IP address is a multicast address , false otherwise
       */
      virtual const bool is_multicast() const { return (_address.get() == NULL) ? false : _address->is_multicast(); };
      /**
       * @brief Indicate the IP address is a broadcast address 
       * @return true if the IP address is a broadcast address , false otherwise
       */
      virtual const bool is_broadcast() const { return (_address.get() == NULL) ? false : _address->is_broadcast(); };
      /**
       * @brief Indicate the IP address is a localhost address
       * @return true if the IP address is a localhost address , false otherwise
       */
      virtual const bool is_localhost() const { return (_address.get() == NULL) ? false : _address->is_localhost(); };
      
      /**
       * @brief Retrieve the socket address information (struct in_address * for IPv4 or struct in6_address * for IPv6)
       * @return The socket address information on success, NULL otherwise
       */
      inline const void * addr() const { return (_address.get() == NULL) ? NULL : _address->addr(); };
      
      /**
       * @brief Retrieve the port number. Default: -1
       * @return The port number
       */
      inline const uint16_t port() const { return _port; };
      
      /**
       * @brief Retrieve the length of the socket address information data structure
       * @return The length of the socket address information data structure
       */
      inline size_t length() const { return (_address.get() == NULL) ? 0 : _address->length(); };

      /**
       * @brief Retrieve the socket address in string format
       * @return The socket address in string format
       */
      inline std::string to_string() const { return (_address.get() == NULL) ? std::string() : _address->to_string(); };
      /**
       * @brief Retrieve the socket address in byte format
       * @return The socket address in byte format
       */
      inline std::vector<uint8_t> to_numeric() const { return (_address.get() == NULL) ? std::vector<uint8_t>() : _address->to_numeric(); };

      /**
       * @brief Parse the IP address
       * @param p_address The IP address to parse
       * @return An new instance of socket_address class for the parsed address on success, NULL otherwise
       */
      static std::unique_ptr<socket_address> parse(const std::string & p_address, const uint16_t p_port);
      /**
       * @brief Parse the IP address
       * @param p_address The IP address to parse
       * @param p_result The new instance of socket_address on success, NULL otherwise
       * @return true on success, false otherwise
       */
      static bool try_parse(const std::string & p_address, const uint16_t p_port, std::unique_ptr<socket_address> & p_result);

    }; // End of class socket_address

  } // End of namespace network

} // End of namespace comm

using namespace comm::network;
