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

#include "udp_echo.hh"
#include "udp_echo_server.hh"
#include "runnable.hh" // Thread implementation

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
  opt >> get_opt::option('d', "debug", is_debug_set, true);
  opt >> get_opt::option('a', "address", address, "localhost");
  opt >> get_opt::option('p', "port", port, (uint16_t)12345);

  logger_factory::get_instance().get_logger(s).info("Command line args: %x, %s, %u", is_debug_set, address.c_str(), port);

  // Launch the UDP echo server
  udp_echo_server server(std::string("0.0.0.0"), address, port, logger_factory::get_instance().get_logger(s));
  server.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Send a UDP message to the server
  int32_t result;
  udp_echo client(address.c_str(), port, logger_factory::get_instance().get_logger(s));
  result = client.send(std::string("This is a message from the client side"));

  // Stop the server
  server.stop();

  return 0; // Succeed
} // End of main function
