/* Beaglebone GPS NMEA protocol analyzer description
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
#if !defined(__GPS_NMEA_ANALYZER_H__)
#define __GPS_NMEA_ANALYZER_H__

#include <regex.h> // Online regex evaluation: http://gskinner.com/RegExr/

#include "gpsProtocolsAnalyzer.h"

// TODO Use factory to create instance of GPS protocol analyzer
namespace beagleboneGps {

  class gpsNmeaAnalyzer : public gpsProtocolsAnalyzer {
    std::string _buffer;

    static std::string CmdPattern;
    /** Global Positioning System Fix Data
     */
    static std::string GgaPattern;
    /** Recommended minimum specific GPS/Transit data
     */
    static std::string RmcPattern;

    regex_t _regexCommand;
    regmatch_t *_matches;
    regex_t _regexGga;
    regex_t _regexRmc;

  public:
    gpsNmeaAnalyzer();
    virtual ~gpsNmeaAnalyzer();

    virtual int process(const std::vector<unsigned char> & p_data);
    virtual int process(const std::string & p_data);
    virtual void getResult(std::vector<unsigned char> & p_buffer);

  private:
    int processBuffer();
    int processGga(const std::string & p_payload);
    int processRmc(const std::string & p_payload);

  }; // End of class gpsNmeaAnalyzer

} // End of namespace beagleboneGps

using namespace beagleboneGps;

#endif // __GPS_NMEA_ANALYZER_H__
