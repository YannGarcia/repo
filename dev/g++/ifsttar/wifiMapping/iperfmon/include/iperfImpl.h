/* Beaglebone iperf-3.0 client implementation
 * Copyright (c) 2013 ygarcia, MIT License
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
#if !defined(__IPERF_H__)
#define __IPERF_H__

#include <iostream>
#include <vector>
#include <regex.h>

namespace iperfmon {

  class iperfImpl {
    std::string _cmd;
    FILE *_pipe;
    regex_t _regex;
    regmatch_t *_pmatches;
    std::vector<float> _values;
    std::vector<float> _xfers;
    std::vector<float> _bandwidths;

    void setResult(const std::string & p_line);
    
  public:
    iperfImpl(const std::string & p_client, const std::string & p_host, const int p_port, const int p_duration);
    virtual ~iperfImpl();

    int clear();
    int close();
    int acquire();
    void getResult(std::vector<unsigned char> & p_result);

  private:
    static std::string Pattern;

  }; // End of class iperfImpl
  
} // End of namespace iperfmon

using namespace iperfmon;

#endif // __IPERF_H__
