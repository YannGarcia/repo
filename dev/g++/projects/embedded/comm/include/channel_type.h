/**
 * @file      channel_type.h
 * @brief     Header file for communication channel type enumerated.
 * @author    garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license   This project is released under the MIT License
 * @version   0.1
 */
#pragma once

namespace comm {
  
  /**
   * @enum protocol_t
   * @brief List of authorized protocols
   */
  enum class channel_type : unsigned char { // TODO Rename channel_type into protocol_t
    udp = 0x00,     /** UDP protocol */
    tcp = 0x01,     /** TCP protocol */
    sctp = 0x02,    /** SCTP protocol */
    raw = 0x03      /** Undefined protocol */
  }; // End of enum class protocol_t

} // End of namespace comm

using namespace comm;
