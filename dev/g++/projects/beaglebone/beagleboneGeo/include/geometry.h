/* Beaglebone geometry converstion tools
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
#if !defined(__GEOMETRY_H__)
#define __GEOMETRY_H__

#include <sstream>
#include <iomanip> // Used for std::setfill, std::setprecision
#include <cmath> // Used fo ceilf

#undef PI
#define PI           3.14159265358979323846
#define EARTH_RADIUS 6366710 /** The mean radius of the Earth in meters. Equatorial = 6378137, polar = 6356752. */

namespace beagleboneGeo {

  /** 
   * @desc This class provides a set of convertion methods. V0.0.0.1
   * \see http://home.online.no/~sigurdhu/Deg_formats.htm
   * \see http://software.geocaching-sk.info/thailon/converter
   *
   * \author Yann Garcia (Don't hesitate to contact me: garcia.yann@gmail.com)
   */
  class geometry {
    static geometry * Instance;

    geometry() {};
    ~geometry() { if (Instance != NULL) { delete Instance; Instance = NULL; } };

  public:
    inline static geometry & getInstance() { if (Instance == NULL) Instance = new geometry(); return *Instance; };

  public:
    inline const double nmea2decimal(const std::string & p_coordinates) const {
      bool neg = ((p_coordinates[p_coordinates.length() - 1] == 'S') || (p_coordinates[p_coordinates.length() - 1] == 'W'));
      
      std::string coordinates = p_coordinates.substr(0, p_coordinates.length() - 2);
      double degree;
      if ((p_coordinates[p_coordinates.length() - 1]  == 'E') || (p_coordinates[p_coordinates.length() - 1]  == 'W')) { // Longitude
	degree = static_cast<double>((p_coordinates[0] - '0') * 100 + (p_coordinates[1] - '0') * 10 + (p_coordinates[2] - '0'));
	coordinates = coordinates.erase(0, 3);
      } else { // Latitude
	degree = static_cast<double>((coordinates[0] - '0') * 10 + (coordinates[1] - '0'));
	coordinates = coordinates.erase(0, 2);
      }
      double Md = atof(coordinates.c_str());
      double result = degree + Md / 60;
      
      return (neg) ? -1.0 * result : result;
    }; // End of nmea2decimal
    
    inline const std::string decimal2nmea(const double p_decimal, const unsigned int p_num_dec_places = 2) const {
      bool neg(false) ;
      double decimal(p_decimal);
      if (decimal < 0.0) {
	neg = true;
	decimal = -p_decimal;
      }
      
      int degree = static_cast<int>(p_decimal);
      double minutes = 60.0 * (decimal - static_cast<double>(degree));
      //      minutes = ceilf(minutes * 1000000) / 1000000;
      std::ostringstream os;
      os << std::setfill('0') << std::setw(p_num_dec_places) << degree << std::setprecision(7) << minutes;
      if (p_num_dec_places == 2) {
	os << static_cast<const char *>((neg == false) ? " N" : " S");
      } else {
	os << static_cast<const char *>((neg == false) ? " E" : " W");
      }
      
      return os.str();
    }; // End of decimal2dms
    

    inline const double degrees2radians(const double p_degrees){ return p_degrees * PI / 180.0; };
    inline const double radians2degrees(const double p_radians) { return p_radians * 180.0 / PI; } ;
    inline const double meters2radians(const double p_meters) { return p_meters / EARTH_RADIUS; };
    inline const double radians2meters(const double p_radians) { return p_radians * EARTH_RADIUS; };
    
  }; // End of class geometry
  
} // End of namespace beagleboneGeo

using namespace beagleboneGeo;

#endif // __GEOMETRY_H__
