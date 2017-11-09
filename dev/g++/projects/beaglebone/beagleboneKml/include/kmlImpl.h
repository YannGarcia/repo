/* Beaglebone KML wrapper description
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
#if !defined(__KMLIMPL_H__)
#define __KMLIMPL_H__

#include <iostream>
#include <string>
#include <ctime>
#include <map>

#include "dom.h"

namespace beagleboneKml {

  /** This class provides a wrapper to the Google libkml. V0.0.0.1
   *
   * \author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
   */
  class kmlImpl {

    static kmlImpl * Instance;

    kmldom::KmlFactory * _factory;
    kmldom::DocumentPtr _document;
    kmldom::KmlPtr _kml;
    std::map<unsigned int, kmldom::PlacemarkPtr> _placemarks;
    std::map<unsigned int, kmldom::CoordinatesPtr> _coordinates;

    kmlImpl();

  public:
    virtual ~kmlImpl() { delete kmlImpl::Instance; kmlImpl::Instance = NULL; };
    
    static kmlImpl & getInstance();

    void reset();
    void createPlacemark(const unsigned int p_id, const std::string & p_latitude, const std::string & p_longitude, const time_t p_time = time(NULL), const std::string & p_lineStyle = std::string(""), const float p_xfer = 0.0, const float p_bw = 0.0, const float p_speed = 0.0, const float p_elevation = 0.0, const float p_heading = 0.0, const float p_temp = 0.0, const float p_acc = 0.0);

    void createLinePlacemark(const unsigned int p_id, const time_t p_time = time(NULL), const std::string & p_lineStyle = std::string(""));
    void addLinePlacemark(const unsigned int p_id, const float p_xfer, const float p_bw, const std::string & p_latitude, const std::string & p_longitude, const float p_speed = 0.0, const float p_elevation = 0.0, const float p_heading = 0.0, const float p_temp = 0.0, const float p_acc = 0.0);
    void finalyseLinePlacemark(const int p_id);


    /**
     * @desc
     * \param p_color Color value in AABBGGRR format
     */
    void createLineStyle(const unsigned int p_id, const unsigned char p_width, const unsigned int p_color);
    
    inline const std::string toString() const {
      return std::string(kmldom::SerializePretty(_kml));
    };
    
  }; // End of class kmlImpl

} // End of namespace beagleboneKml

using namespace beagleboneKml;

#endif // __KMLIMPL_H__

