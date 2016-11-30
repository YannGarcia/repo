/**
 * @file    keyboard.cpp
 * @brief   Main implementation file for the keyboard key stricked helper class.
 * @author garciay.yann@gmail.com
 * @copyright Copyright (c) 2015 ygarcia. All rights reserved
 * @license This project is released under the MIT License
 * @version 0.1
 */
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
//#include <unistd.h>
#define STDIN_FILENO 0 // Standard input

#include "keyboard.h"

namespace helpers {

  uint32_t keyboard::kbhit() {
    struct termios oldt = { 0 }, newt = { 0 };
    int ch;
    int oldf;
 
    ::tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    ::tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    ::fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
    ch = ::getchar();
 
    ::tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    ::fcntl(STDIN_FILENO, F_SETFL, oldf);
 
    if (ch != EOF) {
      ::ungetc(ch, stdin);
      return static_cast<uint32_t>(ch);
    }
 
    return 0;
  }

} // End of namespace helpers
