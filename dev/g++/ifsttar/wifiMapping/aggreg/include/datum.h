#if !defined(__DATUM_H__)
#define __DATUM_H__

#include <vector>
#include <map>

#include "archive.h"

namespace beagleboneStorage {

  class datum {
    archive _archive;

    float _xfer, _bw;
    float _atemp, _dutyCycle, _elevation, _speed, _heading;
    std::string _latitude;
    std::string _longitude;
    time_t _gpsDateTime;
    time_t _timestamp;
    std::map<std::string, std::vector<std::string> > _bssids;

  public:
    datum(const std::vector<unsigned char> & p_data);
    virtual ~datum();

    inline const float get_timestamp() const { return _timestamp; };
    inline const float get_xfer() const { return _xfer; };
    inline const float get_bw() const { return _bw; };
    inline const float get_atemp() const { return _atemp; };
    inline const float get_elevation() const { return _elevation; };
    inline const float get_speed() const { return _speed; };
    inline const std::string & get_latitude() const { return _latitude; };
    inline const std::string & get_longitude() const { return _longitude; };

    const std::string & toString() const;

  private:
    void processArchive(const std::vector<unsigned char> & p_data);
    void processSegmentId(std::vector<unsigned char> & p_data, unsigned char p_segmentId);
    void deserializeGpsData(std::vector<unsigned char> & p_data);
    void deserializeWlanData(std::vector<unsigned char> & p_data);
  }; // End of class datum

} // End of namespace beagleboneStorage

using namespace beagleboneStorage;

#endif //  __DATUM_H__

