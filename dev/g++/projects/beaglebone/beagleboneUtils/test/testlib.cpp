//============================================================================
// Name        : testLib.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : raspberryCommlibrary demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip> // Use for setprecision
#include <cstring> // Used std::strerror
#include <memory>

#include <unistd.h>
#include <signal.h>
#include <limits.h>

#include <signal.h> // Used execl, kill
#include <sys/wait.h> // Used waitpid

#include "getOpt.h"
#include "converter.h"
#include "archive.h"
#include "keyboard.h"
#include "dateTime.h"

#include "dump.h"

using namespace std;

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  std::cout << "Caught signal: " << signum << std::endl;

  // Terminate program
  exit(signum);
} // End of function signal_callback_handler

/***********************************************************************
  Main application part
***********************************************************************/

int main(const int argc, const char * argv[]) {
  // Register signal and signal handler
  if (signal (SIGINT, signal_callback_handler) == SIG_ERR) {
    std::cerr <<  "Cannot handle SIGINT" << std::endl;
    exit (EXIT_FAILURE);
  }
  if (signal (SIGTERM, signal_callback_handler) == SIG_ERR) {
    std::cerr << "Cannot handle SIGTERM" << std::endl;
    exit (EXIT_FAILURE);
  }

  // Parse command line arguments
  bool isDebugSet, autoMode;
  getOpt::getOpt opt(argc, argv);
  opt >> getOpt::Option('a', "autoMode", autoMode, true);
  opt >> getOpt::Option('d', "debug", isDebugSet, false);
  std::cout <<
    "Arguments: " << 
    " - autoMode:" << (const char *)((autoMode == true) ? "true" : "false") << 
    " - debug:" << (const char *)((isDebugSet == true) ? "true" : "false") << 
    std::endl;
  std::cout << std::endl;

  unique_ptr<dateTime> dt(new dateTime);
  cout << "Current time :" << dt->toString() << endl;

  cout << "Binary 0x0a -> BCD 0x" << hex << setw(2) << setfill('0') << (int)converter::getInstance().bin2bcd(0x0a) << endl;
  cout << "BCD 0x13 -> binary 0x" << hex << setw(2) << setfill('0') << (int)converter::getInstance().bcd2bin(0x13) << endl;

  std::string frombytes("Hello");
  std::vector<unsigned char> tobytes(converter::getInstance().string2bytes(frombytes));
  cout << "str->bytes->str(" << frombytes << "): " << converter::getInstance().bytes2string(tobytes) << endl;
  
  cout << "Swapping 0xcafe -> 0x" << hex << setw(4) << setfill('0') << (int)converter::getInstance().swap((const unsigned short)0xcafe) << endl;
  cout << "Swapping 51966 -> " << converter::getInstance().swap((const unsigned short)51966) << endl;
  cout << "Swapping 0xcafedeca -> 0x" << hex << setw(4) << setfill('0') << (int)converter::getInstance().swap((const unsigned int)0xcafedeca) << endl;
  cout << "Swapping 51966 -> " << converter::getInstance().swap((const unsigned int)3405700810) << endl;

  string str("Hello World!");
  string hexa(converter::getInstance().string2hexa(str));
  cout << "hexa('" << str << "'): " << hexa << endl;
  cout << "string(" << hexa << "): " << converter::getInstance().hexa2string(hexa) << endl;

  int ivalue = (int)0xcafedeca;
  vector<unsigned char> int2bytes = converter::getInstance().int2bytes(ivalue);
  cout << "int2bytes('" << dec << (int)ivalue << "'): " << hex << setw(2) << setfill('0') << (int)int2bytes[0] << " " << (int)int2bytes[1] << (int)int2bytes[2] << " " << (int)int2bytes[3] << endl;
  cout << "bytes2int: " << dec << (int)converter::getInstance().bytes2int(int2bytes) << endl;

  float fvalue = 3.1415;
  vector<unsigned char> float2bytes = converter::getInstance().float2bytes(fvalue);
  cout << "float2bytes('" << (float)fvalue << "'): " << hex << setw(2) << setfill('0') << (int)float2bytes[0] << " " << (int)float2bytes[1] << (int)float2bytes[2] << " " << (int)float2bytes[3] << endl;
  cout << "bytes2float: " << (float)converter::getInstance().bytes2float(float2bytes) << endl;

  time_t curTime = time(NULL);
  cout << "sizeof(time_t)=" << sizeof(time_t) << endl;
  vector<unsigned char> long2bytes = converter::getInstance().long2bytes(curTime);
  cout << "time_t2bytes('" << curTime << "'): " << hex << setw(2) << setfill('0') << (int)long2bytes[0] << " " << (int)long2bytes[1] << " " << (int)long2bytes[2] << " " << (int)long2bytes[3]/* << " " << (int)long2bytes[4] << " " << (int)long2bytes[5] << " " << (int)long2bytes[6] << " " << (int)long2bytes[7]*/ << endl;
  cout << "bytes2long: " << converter::getInstance().bytes2long(long2bytes) << endl;

  std::clog << "Serialize true, 235689, 3.141519, 4, 'cafedeca'O, false, " << curTime << std::endl;
  archive ar;
  ar.serialize(true);
  ar.serialize((const int)235689);
  ar.serialize((const float)3.141519);
  ar.serialize((const int)32);
  std::vector<unsigned char> bufferIn;
  bufferIn.push_back(0xca);bufferIn.push_back(0xfe);bufferIn.push_back(0xde);bufferIn.push_back(0xca);
  ar.serialize(bufferIn);
  ar.serialize(false);
  ar.serialize(curTime);
  std::vector<unsigned char> ser(ar.toBytes());
  dump::hexaDump(ser);
  std::clog << "Deserialize true, 235689, 3.141519, 4, 'cafedeca'O, false" << std::endl;
  bool start, stop;
  ar.clear();
  std::clog << "Deserialize start:" << std::endl;
  ar.deserialize(ser, &start); // true
  std::clog << "Deserialize int:" << std::endl;
  ar.deserialize(ser, &ivalue); // 235689
  std::clog << "Deserialize float:" << std::endl;
  ar.deserialize(ser, &fvalue); // 3.141519
  int ivalue32;
  std::clog << "Deserialize int:" << std::endl;
  ar.deserialize(ser, &ivalue32); // 32
  std::vector<unsigned char> bufferOut;
  std::clog << "Deserialize vector:" << std::endl;
  ar.deserialize(ser, bufferOut);
  std::clog << "Deserialize stop:" << std::endl;
  ar.deserialize(ser, &stop);
  curTime = 0;
  ar.deserialize(ser, &curTime);
  std::clog << "Result: " <<
    (char *)((start) ? "true" : "false") << ", " <<
    ivalue << ", " <<
    fvalue << ", " <<
    ivalue32 << ", " <<
    "'" << std::hex << std::setw(2) << std::setfill('0') << (int)bufferOut[0] << (int)bufferOut[1] << (int)bufferOut[2] << (int)bufferOut[3] << "'O, " <<
    (char *)((stop) ? "true" : "false") << ", " <<
    "curTime= " << curTime <<
    std::endl;

  std::clog << "Serialize GPS:" << std::endl;
  ar.clear();
  str.assign("140514 080842.000");
  ar.serialize(str);
  str.assign("4339.8072 N");
  ar.serialize(str);
  str.assign("00642.7134 E");
  ar.serialize(str);
  str.assign("0.03");
  ar.serialize(str);
  str.assign("551.5");
  ar.serialize(str);
  str.assign("106.20");
  ar.serialize(str);
  str.assign("");
  ar.serialize(str);
  std::vector<unsigned char> ser1(ar.toBytes());
  dump::hexaDump(ser1);
  ar.clear();
  std::clog << "Deserialize GPS:" << std::endl;
  ar.deserialize(ser1, str); 
  std::clog << "UTC date/time:" << str << std::endl;
  ar.deserialize(ser1, str); 
  std::clog << "Latitude:" << str << std::endl;
  ar.deserialize(ser1, str); 
  std::clog << "Longitude:" << str << std::endl;
  ar.deserialize(ser1, str); 
  std::clog << "Speed:" << str << std::endl;
  ar.deserialize(ser1, str); 
  std::clog << "Elevation:" << str << std::endl;
  ar.deserialize(ser1, str); 
  std::clog << "Heading:" << str << std::endl;
  ar.deserialize(ser1, str); 
  std::clog << "Yawrate:" << str << std::endl;

  str = "180614 16015423";
  curTime = dt->gps2utc(str);
  std::clog << "GPS -> UTC:" << std::dec << static_cast<unsigned long>(curTime) << std::endl;
  std::clog << "UTC -> LTC:" << std::dec << static_cast<unsigned long>(dt->utc2local(curTime)) << std::endl;
  std::clog << "NOW -> LTC:" << std::dec << static_cast<unsigned long>(dt->utc2local(time(NULL))) << std::endl;

  str = "   A  lot of  white spaces    ";
  std::string trimmedStr = converter::getInstance().trim(str);
  std::clog << "trim(" << str << ")= " << trimmedStr << std::endl; 
  str = "--host localhost --port 12345 --duration -1";
  std::vector<std::string> tokens = converter::getInstance().split_arguments_line(str);
  tokens.insert(tokens.begin(), "iperf3");
  std::clog << "Tokens: " << std::endl;
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    std::clog << "   " << *it << std::endl;
  }
  str = "-h localhost";
  tokens = converter::getInstance().split_arguments_line(str);
  tokens.insert(tokens.begin(), "iperf3");
  std::clog << "Tokens: " << std::endl;
  for (auto it = tokens.begin(); it != tokens.end(); ++it) {
    std::clog << "   " << *it << std::endl;
  }

  while (true) {

    usleep(50);

    if (keyboard::keyboard::kbhit() != 0) {
      std::cout << "key pressed was: '" << (char)getchar() << "'" << std::endl;
      break; // Exit from 'while' statement
    }

  } // End of 'while' statement

  return EXIT_SUCCESS;
}
