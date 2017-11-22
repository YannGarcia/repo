/*!
 * \file      main.cpp
 * \brief     Demo application of UDP echo.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <cstdlib>

#include "logger_factory.hh"

#include "udp_echo.hpph"

int32_t main(const int32_t p_argc, const char** p_argv) {
  // Create logger instance
  std::string s("logger1");
  std::string path(std::getenv("HOME_TMP") + std::string("/") + s + ".log");
  logger_factory::get_instance().add_logger(s, path);

  // Parse comand line
  bool is_debug_set;
  std::string address;
  uint16_t port;
  get_opt::get_opt opt(p_argc, p_argv);
  opt >> get_opt::option('d', "debug", is_debug_set, false);
  opt >> get_opt::option('a', "address", address, "");
  opt >> get_opt::option('p', "port", port, (uint16_t)0);

  logger_factory::get_instance().get_logger(s).info("Command line args: %x, %s, %ud", is_debug_set, address.c_str(), port);

  
  return 0; // Succeed
} // End of main function
