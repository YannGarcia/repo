/* BeagleBone child process monitor interface
 * Copyright (c) 2014 ygarcia, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include <cstring> // Used std::strerror

#include <unistd.h>
#include <errno.h>
#include <sys/types.h> // Used TERM
#include <signal.h> // Used execl, kill
#include <sys/wait.h> // Used waitpid

#include "converter.h" // Used split

#include "processMonitor.h"

namespace wifimapping {

  int processMonitor::prepare(const std::map<std::string, std::string> & p_cfgMap, const std::string & p_processName) {
    //    std::clog << ">>> processMonitor::prepare" << std::endl;

    // Sanity check
    if (p_processName.empty()) {
      std::cerr << "processMonitor::prepare: Wrong parameters" << std::endl;
      return -1;
    }

    std::string key(p_processName + ".path");
    if (p_cfgMap.find(key) == p_cfgMap.end()) {
      std::cerr << "processMonitor::prepare: Key not found: " << key << std::endl;
      _path = "/usr/local/bin/" + p_processName;
    } else {
      _path = p_cfgMap.at(key);
      key.assign(p_processName + ".args");
      if (p_cfgMap.find(key) != p_cfgMap.end()) {
	_args = p_cfgMap.at(key);
      }
    }

    return 0;
  }

  int processMonitor::execute() {
    //    std::clog << ">>> processMonitor::execute: _path=" << _path << " - _args=" << _args << std::endl;

    // Sanity check
    if (_path.empty()) {
      std::cerr << "processMonitor::execute: Invalid file to execute" << std::endl;
      return -1;
    }

    _processId = fork();
    std::clog << "processMonitor::execute: forked process id: " << (int)_processId << std::endl;
    if (_processId < 0) {
      std::cerr << "processMonitor::execute: Failed to execute processMonitor: " << strerror(errno) << std::endl;
      return -1;
    } else if (_processId == 0) {
      if (_args.empty()) { // No arguments
	char * args[2];
	args[0] = (char *)(_path.c_str());
	args[1] = NULL;
	if (execvp(_path.c_str(), args) == -1) {
	  std::clog << "processMonitor::execute(child): 'execl' returns: " << strerror(errno) << std::endl;
	}
      } else { // Arguments required
	// Spilt the arguments line
	std::vector<std::string> tokens = converter::getInstance().split_arguments_line(_args);
	// Insert first the process name
	tokens.insert(tokens.begin(), _path); // execv/argv[0] = 'Path name'
	// And the arguments
	std::vector<std::string> argv;
	for (auto it = tokens.begin(); it != tokens.end(); ++it) {
	  size_t whiteChar = (*it).find(" ");
	  if (whiteChar == std::string::npos) {
	    argv.push_back(converter::getInstance().trim(*it));
	  } else { // Need to split keyword and value
	    argv.push_back(converter::getInstance().trim((*it).substr(0, whiteChar)));
	    argv.push_back(converter::getInstance().trim((*it).substr(whiteChar + 1)));
	  }
	} // End of 'for' statement
	char * args[argv.size() + 1/* For the NULL*/];
	for (auto it = argv.begin(); it != argv.end(); ++it) { // Store string pointer into the array
	  args[static_cast<int>(it - argv.begin())] = (char *)(it->c_str());
	  //	  std::clog << "   " << argv[static_cast<int>(it - argv.begin())] << " - " << static_cast<int>(it - argv.begin()) << std::endl;
	} // End of 'for' statement
	args[argv.size()] = NULL;
	// Execute the process
	if (execvp(_path.c_str(), args) == -1) {
	  std::clog << "processMonitor::execute(child): 'execv' returns: " << strerror(errno) << std::endl;
	}
      }
    } else { // Parent process
      // Nothing to do
      std::clog << "processMonitor::execute(parent): continue" << std::endl;
      sleep(1);
    }

    return reinterpret_cast<int>(_processId); 
  }

  int processMonitor::terminate() {
    std::clog << ">>> processMonitor::terminate" << std::endl;

    // Sanity check
    if (_processId == -1) {
      std::cerr << "processMonitor::terminate: No process running" << std::endl;
      return -1;
    }
    // Send kill signal to the process
    std::clog << "processMonitor::terminate: kill process " << (int)_processId << std::endl;
    int result = kill(_processId, SIGINT);
    if (result != 0) {
      std::cerr << "processMonitor::terminate: Failed to termnate processMonitor: " << strerror(errno) << std::endl;
    } else {
      int status = 0;
      waitpid(_processId, &status, WNOHANG | WUNTRACED);
      std::clog << "processMonitor::terminate: Process " << _processId << " terminated with status " <<  status << std::endl;
    }
    _processId = -1;

    return result;
  }

} // End of namespace wifimapping

