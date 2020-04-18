/*!
 * \file      main.cpp
 * \brief     Demo application of TCP echo.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <cstdlib>

#include "logger_factory.hh"

#include "tcp_echo.hh"
#include "tcp_echo_server.hh"
#include "keyboard.hh"
#include "runnable.hh" // Thread implementation

static int g_index = 0;
std::vector<int8_t> g_wait_cursor { '|', '/', '-', '\\', '|', '/', '-', '\\' };

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

  // Launch the TCP echo server
  tcp_echo_server server(std::string("0.0.0.0"), address, port, logger_factory::get_instance().get_logger(s));
  server.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  /***
  // Wait for stop keyword
  while (static_cast<char>(keyboard::kbhit()) == 0) {
    std::clog << g_wait_cursor[g_index] << '\b';
    g_index = (g_index + 1) % g_wait_cursor.size();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  };
  ***/

  // Send a TCP message to the server
  int32_t result;
  tcp_echo client(address.c_str(), port, logger_factory::get_instance().get_logger(s));
  result = client.send(std::string("This is a message from the client side"));
  // Wait for the response
  std::string response;
  result = client.receive(response);
  logger_factory::get_instance().get_logger(s).info("Receive from server: %s", response.c_str());
  // Wait some few seconds
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Stop the server
  server.stop();

  return 0; // Succeed
} // End of main function
