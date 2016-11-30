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

#include <iostream>
#include <sstream>

#include "geometry.h"

#include "kmlImpl.h"

namespace beagleboneKml {

  kmlImpl * kmlImpl::Instance = NULL;

  kmlImpl::kmlImpl() : _factory(kmldom::KmlFactory::GetFactory()), _document(_factory->CreateDocument()), _kml(_factory->CreateKml()), _placemarks(),  _coordinates() {
    _kml->set_feature(_document);
  } // End of Ctor

  kmlImpl & kmlImpl::getInstance() { 
    if (kmlImpl::Instance == NULL) {
      Instance = new kmlImpl();
    }

    return *Instance; 
  }

  void kmlImpl::reset() {
    _kml = _factory->CreateKml();
    _document = _factory->CreateDocument();
    _kml->set_feature(_document);
    _placemarks.clear();
    _coordinates.clear();
  }

  void kmlImpl::createLineStyle(const unsigned int p_id, const unsigned char p_width, const unsigned int p_color) {
    // Create <style>
    kmldom::StylePtr style = _factory->CreateStyle();
    style->set_id(static_cast<std::ostringstream *>( &(std::ostringstream() << p_id) )->str());
    // Create <linestyle>
    kmldom::LineStylePtr linestyle = _factory->CreateLineStyle();
    linestyle->set_width(static_cast<double>(p_width));
    kmlbase::Color32 color(static_cast<int>(p_color)); // Force usage of RGB-R
    linestyle->set_color(color);
    // Give _style <LineStyle>.
    style->set_linestyle(linestyle);
    // Give _document <LineStyle>.
    _document->add_styleselector(style);
  }

  void kmlImpl::createPlacemark(const unsigned int p_id, const std::string & p_latitude, const std::string & p_longitude, const time_t p_time, const std::string & p_lineStyle, const float p_xfer, const float p_bw, const float p_speed, const float p_elevation, const float p_heading, const float p_temp, const float p_acc) {
    //    std::clog << ">>> kmlImpl::createPlacemark" << std::endl;
    
    // Create <coordinates>
    kmldom::CoordinatesPtr coordinates = _factory->CreateCoordinates();
    // Convert string coordinates into float coordonates
    float lat = static_cast<float>(geometry::getInstance().nmea2decimal(p_latitude));
    float lon = static_cast<float>(geometry::getInstance().nmea2decimal(p_longitude));
    // Create <coordinates>decimal latitude, decimal longitude, altitude<coordinates>
    coordinates->add_latlngalt(lat, lon, p_elevation);
    // Create <Point> and give it <coordinates>.
    kmldom::PointPtr point = _factory->CreatePoint();
    point->set_coordinates(coordinates);  // point takes ownership
    point->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
    
    // Create <Placemark> and give it an <id>.
    kmldom::PlacemarkPtr placemark = _factory->CreatePlacemark();
    placemark->set_id(static_cast<std::ostringstream *>( &(std::ostringstream() << p_id) )->str());
    // Give it a <name>.
    placemark->set_name(static_cast<std::ostringstream *>( &(std::ostringstream() << p_time) )->str());
    // Add <LineStyleUrl>
    if (p_lineStyle.length() != 0) {
      if (p_lineStyle[0] != '#') {
	std::string str("#");
	str += p_lineStyle;
	placemark->set_styleurl(str);
      } else {
	placemark->set_styleurl(p_lineStyle);
      }
    }
    // Give it an extended data
    kmldom::DataPtr d = _factory->CreateData();
    //    d->set_name(std::string("kmldom::DataPtr::set_name"));
    d->set_displayname(std::ctime(&p_time));
    std::ostringstream os;
    os << 
      "Lat=" << p_latitude << std::endl << 
      "long=" << p_longitude << std::endl <<
      "speed=" << p_speed << std::endl <<
      "Xfer=" << p_xfer << std::endl <<
      "Bw=" << p_bw << std::endl;
    d->set_value(os.str());
    kmldom::ExtendedDataPtr ed = _factory->CreateExtendedData();
    ed->add_data(d);
    placemark->set_extendeddata(ed);
    // Give it a <Point>.
    placemark->set_geometry(point);  // placemark takes ownership
    // Give _document <Placemark>.
    _document->add_feature(placemark);  // document takes ownership.
  }

  void kmlImpl::createLinePlacemark(const unsigned int p_id, const time_t p_time, const std::string & p_lineStyle) {
    // Sanity check
    if (_placemarks.find(p_id) != _placemarks.end()) {
      std::cerr << "kmlImpl::createLinePlacemark: Wrong parameters" << std::endl;
      return;
    }
    if (_coordinates.find(p_id) != _coordinates.end()) {
      std::cerr << "kmlImpl::createLinePlacemark: Wrong parameters" << std::endl;
      return;
    }
    
    // Create <Placemark> and give it an <id>.
    kmldom::PlacemarkPtr placemark = _factory->CreatePlacemark();
    placemark->set_id(static_cast<std::ostringstream *>( &(std::ostringstream() << p_id) )->str());
    // Give it a <name>.
    placemark->set_name(static_cast<std::ostringstream *>( &(std::ostringstream() << p_time) )->str());
    // Add <LineStyleUrl>
    if (p_lineStyle.length() != 0) {
      if (p_lineStyle[0] != '#') {
	std::string str("#");
	str += p_lineStyle;
	placemark->set_styleurl(str);
      } else {
	placemark->set_styleurl(p_lineStyle);
      }
    }
    // Give it an extended data
    kmldom::DataPtr d = _factory->CreateData();
    //    d->set_name(std::string("kmldom::DataPtr::set_name"));
    d->set_displayname(std::ctime(&p_time));
    d->set_value("kmlImpl::createLinePlacemark");
    kmldom::ExtendedDataPtr ed = _factory->CreateExtendedData();
    ed->add_data(d);
    placemark->set_extendeddata(ed);
    _placemarks.insert(std::pair<unsigned int, kmldom::PlacemarkPtr>(p_id, placemark));
    // Create <coordinates>
    kmldom::CoordinatesPtr coordinates = _factory->CreateCoordinates();
    _coordinates.insert(std::pair<unsigned int, kmldom::CoordinatesPtr>(p_id, coordinates));
  }

  void kmlImpl::addLinePlacemark(const unsigned int p_id, const float p_xfer, const float p_bw, const std::string & p_latitude, const std::string & p_longitude, const float p_speed, const float p_elevation, const float p_heading, const float p_temp, const float p_acc) {
    // Sanity check
    std::map<const unsigned int, kmldom::CoordinatesPtr>::const_iterator it = _coordinates.find(p_id);
    if (it == _coordinates.end()) {
      std::cerr << "kmlImpl::addLinePlacemark: Wrong parameters" << std::endl;
      return;
    }
    
    // Convert string coordinates into float coordonates
    float lat = static_cast<float>(geometry::getInstance().nmea2decimal(p_latitude));
    float lon = static_cast<float>(geometry::getInstance().nmea2decimal(p_longitude));
    // Create <coordinates>decimal latitude, decimal longitude, altitude<coordinates>
    it->second->add_latlngalt(lat, lon, p_elevation);
  }

  void kmlImpl::finalyseLinePlacemark(const int p_id) {
    // Sanity check
    std::map<unsigned int, kmldom::PlacemarkPtr>::const_iterator it = _placemarks.find(p_id);
    if (it == _placemarks.end()) {
      std::cerr << "kmlImpl::addLinePlacemark: Wrong parameters" << std::endl;
      return;
    }
    std::map<unsigned int, kmldom::CoordinatesPtr>::const_iterator it1 = _coordinates.find(p_id);
    if (it1 == _coordinates.end()) {
      std::cerr << "kmlImpl::addLinePlacemark: Wrong parameters" << std::endl;
      return;
    }    

    // Create <LineString> and give it <coordinates>.
    kmldom::LineStringPtr ls = _factory->CreateLineString();
    ls->set_coordinates(it1->second);  // ls takes ownership
    ls->set_tessellate(true);
    // Give it a <Coordinates array>.
    it->second->set_geometry(ls);  // placemark takes ownership
    // Give _document <Placemark>.
    _document->add_feature(it->second);  // document takes ownership.
    // Erase items
    _placemarks.erase(it);
    _coordinates.erase(it1);
  }

} // End of namespace beagleboneKml

