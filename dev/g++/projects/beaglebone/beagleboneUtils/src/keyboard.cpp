#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "keyboard.h"

namespace beagleboneUtils {

  namespace keyboard {

    int keyboard::kbhit() {
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
	return 1;
      }
 
      return 0;
    }
      
  } // End of namespace keyboard

} // End of namespace beagleboneUtils
