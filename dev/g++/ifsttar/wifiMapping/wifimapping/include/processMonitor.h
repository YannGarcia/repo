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
#if !defined(__IMONITOR_H__)
#define __IMONITOR_H__

#include <sstream> // Used std::ostringstream
#include <string>
#include <map>

#include <sys/types.h>
#include <signal.h>

namespace wifimapping {

  class processMonitor {
  protected:
    pid_t _processId;
    std::string _path;
    std::string _args;

  public:
    processMonitor(): _path(), _args() { _processId = -1; };
    virtual ~processMonitor() { std::clog << ">>> processMonitor::~processMonitor" << std::endl; };

    virtual int prepare(const std::map<std::string, std::string> & p_cfgMap, const std::string & p_processName);
    virtual int execute();
    virtual int terminate();

    inline virtual std::string toString() {
      std::ostringstream os;
      os << 
	"processMonitor: " << std::endl <<
	"  process id: " << _processId << 
	"  path      : " << _path << std::endl <<
	"  args      : " << _args << std::endl <<
	std::endl;
      return os.str();
    }
  }; // End of class processMonitor

} // End of namespace wifimapping

using namespace wifimapping;

#endif // __IMONITOR_H__
