//============================================================================
// Name        : testLib.cpp
// Author      : Yann Garcia
// Version     :
// Copyright   : No copyright
// Description : beagleboneCommlibrary demonstrator/test application in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip> // Use for setprecision
#include <thread>
#include <cstdlib> // Used for memset
#include <cstring> // Used for memcpy, strerror

#include <unistd.h>
#include <signal.h>

#include "beagleboneCommInit.h"

#include "serialChannel.h"
#include "serialThread.h"
#include "networkThread.h"
#include "networkTcpThread.h"
#include "sharedMemory.h"
#include "messageQueue.h"
#include "ntpClient.h"
#include "httpClient.h"

using namespace std;

struct UserChoice {
  char choice;
  bool isShutdown;
};

// Serial devices
bool g_isUartServerRunning = false;
beagleboneUtils::thread::runnable *g_uartManager = NULL;
//std::string g_uart("/dev/ttyAMA0"); // Raspberry
std::string g_uart("/dev/ttyO2"); // Beaglebone
// UDP Server
bool g_isUdpServerRunning = false;
beagleboneUtils::thread::runnable *g_udpManager = NULL;
std::string g_host("172.28.0.20");
//std::string g_host("192.168.1.73");
//std::string g_host("127.0.0.1");
// TCP Server
bool g_isTcpServerRunning = false;
beagleboneUtils::thread::runnable *g_tcpManager = NULL;
//std::string g_listener("192.168.1.73");
std::string g_listener("10.7.6.41");
int port = 10000;
//std::string g_host("127.0.0.1");

// Processing variables
static bool g_exitRequested = false;
static UserChoice g_userChoice;

void DisplayMenuAndGetChoice() {
  // Display title 
  cout << endl << endl << "------------------------------------------------------------------------------------------------------" << endl;
  cout << "Test beagleboneComm library app V0.0.1" << endl;
  // Display the menu
  std::cout << "\tGPS read (RS232):" << std::endl;
  std::cout << "\t\tStart           :\t\t\ta" << std::endl;
  std::cout << "\t\tStop            :\t\t\tb" << std::endl;

  std::cout << "\tUDP Server:" << std::endl;
  std::cout << "\t\tStart           :\t\t\tc" << std::endl;
  std::cout << "\t\tStop            :\t\t\td" << std::endl;

  std::cout << "\tTCP Server (" << port << "):" << std::endl;
  std::cout << "\t\tStart           :\t\t\te" << std::endl;
  std::cout << "\t\tStop            :\t\t\tf" << std::endl;

  std::cout << "\tShared memory:" << std::endl;
  std::cout << "\t\tTest            :\t\t\tg" << std::endl;

  std::cout << "\tMessage queue:" << std::endl;
  std::cout << "\t\tTest            :\t\t\th" << std::endl;

  std::cout << "\tNTP Client:" << std::endl;
  std::cout << "\t\tTest            :\t\t\ti" << std::endl;

  std::cout << "\tHTTP Client:" << std::endl;
  std::cout << "\t\tTest            :\t\t\tj" << std::endl;
  std::cout << "\t\tThingSpeak      :\t\t\tk" << std::endl;

  std::cout << "Enter your choice: " << std::flush;
  // Get selection
  g_userChoice.choice = getchar();
  cin.ignore(1, '\n');
  cout << endl << "------------------------------------------------------------------------------------------------------" << endl << flush;
}

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(const int signum) { // FIXME Use one callback per signal
  cout << "Caught signal: " << signum << endl;
  // Cleanup and close up stuff here
  if (g_isUartServerRunning) {
    g_isUartServerRunning = false;
    g_uartManager->stop();
    delete g_uartManager;
  }
  if (g_isUdpServerRunning) {
    g_isUdpServerRunning = false;
    g_udpManager->stop();
    delete g_udpManager;
  }
  if (g_isTcpServerRunning) {
    g_isTcpServerRunning = false;
    g_tcpManager->stop();
    delete g_tcpManager;
  }

  // Terminate application
  exit(EXIT_SUCCESS);
} // End of function signal_callback_handler

int main() {
  // Register signal and signal handler
  if (signal (SIGINT, signal_callback_handler) == SIG_ERR) {
    cerr <<  "Cannot handle SIGINT!" << endl;
    exit (EXIT_FAILURE);
  }
  if (signal (SIGTERM, signal_callback_handler) == SIG_ERR) {
    fprintf (stderr, "Cannot handle SIGTERM!\n");
    exit (EXIT_FAILURE);
  }

  // Process user entries
  while (!g_exitRequested) {
    DisplayMenuAndGetChoice();
    switch (g_userChoice.choice) {
    case 'a': { // Start UART daemon
        g_uartManager = dynamic_cast<beagleboneUtils::thread::runnable *>(new serialThread(g_uart, 9600));
        g_uartManager->start();
        g_isUartServerRunning = true;
      }
      break;
    case 'b': // Stop UART daemon
      g_isUartServerRunning = false;
      g_uartManager->stop();
      delete g_uartManager;
      g_uartManager = NULL;
      break;
    case 'c': { // Start UDP Server
        g_udpManager = dynamic_cast<beagleboneUtils::thread::runnable *>(new networkThread(g_host, 13001, 13001));
        g_udpManager->start();
        g_isUdpServerRunning = true;
      }
      break;
    case 'd': // Stop UDP Server
      g_isUdpServerRunning = false;
      g_udpManager->stop();
      delete g_udpManager;
      g_udpManager = NULL;
      break;
    case 'e': { // Start TCP Server
        g_tcpManager = dynamic_cast<beagleboneUtils::thread::runnable *>(new networkTcpThread(g_listener, port));
        g_tcpManager->start();
        g_isTcpServerRunning = true;
      }
      break;
    case 'f': // Stop TCP Server
      g_isTcpServerRunning = false;
      g_tcpManager->stop();
      delete g_tcpManager;
      g_tcpManager = NULL;
      break;
    case 'g': { // Shared memory
        sharedMemory smServer(0xcafedeca, 1024);
	cout << "smServer: " << smServer.toString();
        sharedMemory smClient1(0xcafedeca, 1024, 128, sharedMemory::ro);
	cout << "smClient1: " << smClient1.toString();
        sharedMemory smClient2(0xcafedeca, 1024, 128, sharedMemory::ro);
	cout << "smClient2: " << smClient2.toString();
	vector<unsigned char> data;
	data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
	smClient2.write(data, data.size());
	clog << "Client2 wrote 0xcafedeca" << endl;
	sleep(1);
	data.clear();
	smClient1.read(data, 2);
	clog << "Client1 read: " << hex << setw(2) << setfill('0') << (int)data[0] << " - " << (int)data[1] << endl;
      }
      break;
    case 'h': { // Message queue
        messageQueue mqServer(0xcafedeca, 1);
	cout << "mqServer: " << mqServer.toString();
        messageQueue mqClient(0xcafedeca, 1, messageQueue::ro);
	cout << "mqClient: " << mqClient.toString();
	vector<unsigned char> data;
	data.push_back(0xca); data.push_back(0xfe); data.push_back(0xde); data.push_back(0xca); 
	mqServer.send(data, data.size());
	std::clog << "Waiting" << std::endl;
	sleep(1);
	vector<unsigned char> result;
	mqClient.recv(result, 4);
	clog << "Read: " << hex << setw(2) << setfill('0') << (int)result[0] << " - " << (int)result[1] << endl;
      }
      break;
    case 'i': { // NTP client
      //          network::ntpClient ntp(std::string("134.59.1.5")); // ntp.unice.fr
          network::ntpClient ntp(
				 std::string("ntp.unice.fr"),
				 g_host
				 );
          ntp.setTime();
      }
      break;
    case 'j': { // HTTP client
          network::httpClient http;
	  std::string data;
          http.get(std::string("http://developer.mbed.org/media/uploads/donatien/hello.txt"), data);
	  std::clog << "HTTP Response is " << data << std::endl;
      }
    case 'k': { // ThingSpeak test
          network::httpClient http;
	  std::string url("http://api.thingspeak.com/update?key=TBCRYSUBMABGA8QT");
	  std::cout << "Enter temperature value: ";
	  std::string temp;
	  std::getline(std::cin, temp);
	  std::cout << std::endl;
	  if (!temp.empty()) {
	    url.append("&field1=");
	    url.append(temp);
	    std::string data;
	    http.get(url, data);
	    std::clog << "HTTP Response is " << data << std::endl;
	  }
      }
      break;
    } // End of 'switch' statement

  } // End of 'while' statement

  cout << "Terminate application" << endl;
  return EXIT_SUCCESS;
} // End of 'main' function 

