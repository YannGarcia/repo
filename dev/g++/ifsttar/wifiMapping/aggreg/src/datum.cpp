#include <iostream>
#include <iomanip>
#include <sstream> // Used for std::ostringstream
#include <stdexcept> // Used for std::invalid_argument 

#include "ipcCommon.h"

#include "datum.h"

namespace beagleboneStorage {

  datum::datum(const std::vector<unsigned char> & p_data) : _archive(), _xfer(0.0), _bw(0.0), _atemp(0.0), _dutyCycle(0.0), _elevation(0.0), _speed(0.0), _heading(), _latitude(""), _longitude(""), _gpsDateTime(0), _timestamp(0), _bssids() {
    processArchive(p_data);
  } // End of Constructor

  datum::~datum() {
  } // End of Destructor


  void datum::processArchive(const std::vector<unsigned char> & p_data) {
    // Sanity check
    if (p_data.size() == 0) {
      std::cerr << "datum::processArchive: Wrong parameters" << std::endl;
      return;
    }

    // Extract shared memory segment identifer
    unsigned char segmentId;
    std::vector<unsigned char> data(p_data);
    do {
      _archive.deserialize(data, reinterpret_cast<char *>(&segmentId));
      processSegmentId(data, segmentId);
      //      std::clog << "datum::processArchive: current size=" << (int)data.size() << std::endl;
    } while (data.size() != 0);
  }

  void datum::processSegmentId(std::vector<unsigned char> & p_data, unsigned char p_segmentId) {
    //    std::clog << ">>> datum::processSegmentId: " << static_cast<unsigned int>(p_segmentId) << std::endl;

    switch (p_segmentId) {
    case IPERF_SEGMENT_ID:
      _archive.deserialize(p_data, &_xfer);
      _archive.deserialize(p_data, &_bw);
      break;
    case GPS_SEGMENT_ID:
      deserializeGpsData(p_data);
      break;
    case ACC_SEGMENT_ID:
      break;
    case TEMP_SEGMENT_ID:
      _archive.deserialize(p_data, &_atemp);
      _archive.deserialize(p_data, &_dutyCycle);
      break;
    case RTC_SEGMENT_ID: {
        unsigned char status;
        _archive.deserialize(p_data, reinterpret_cast<char *>(&status));
        _archive.deserialize(p_data, &_timestamp);
      }
      break;
    case WLAN_SEGMENT_ID:
      deserializeWlanData(p_data);
      break;
    default:
      std::cerr << "datum::processSegmentId: Corrupted archive: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(p_segmentId) << std::endl;
    } // End of 'switch' statement

    //    std::clog << "<<< datum::processSegmentId" << std::endl;
  }

  void datum::deserializeGpsData(std::vector<unsigned char> & p_data) {
    //    std::clog << ">>> datum::deserializeGpsData" << std::endl;

    std::string str;
    _archive.deserialize(p_data, str);
    //    std::clog << "datum::processSegmentId: date/time='" << str << "'" << std::endl;
    // TODO Convert GPS date/time into time_t
    _gpsDateTime = time(NULL);
    /*if (_timestamp == 0) {
      _timestamp = _gpsDateTime;
      }*/
    //    std::clog << "datum::processSegmentId: _timestamp='" << _timestamp << "'" << std::endl;
    _archive.deserialize(p_data, _latitude);
    //	std::clog << "datum::processSegmentId: latitude='" << _latitude << "'" << std::endl;
    _archive.deserialize(p_data, _longitude);
    //	std::clog << "datum::processSegmentId: longitude='" << _longitude << "'" << std::endl;
    // Speed
    _archive.deserialize(p_data, str);
    //	std::clog << "datum::processSegmentId: speed='" << str << "'" << std::endl;
    if (str.length() != 0) { 
      try {
      _speed = 1.852/*knots to km/h*/ * std::stof(str); // Convert into float 
      } catch (std::invalid_argument & i) {
	_speed = 0.0;
      }
    }
    // Elevation
    _archive.deserialize(p_data, str);
    //	std::clog << "datum::processSegmentId: elevation='" << str << "'" << std::endl;
    if (str.length() != 0) { 
      try {
      _elevation = std::stof(str); // Convert into float 
      } catch (std::invalid_argument & i) {
	_elevation = 0.0;
      }
    }
    // Heading
    _archive.deserialize(p_data, str);
    //	std::clog << "datum::processSegmentId: heading='" << str << "'" << std::endl;
    if (str.length() != 0) { 
      try {
      _heading = std::stof(str); // Convert into float 
      } catch (std::invalid_argument & i) {
	_heading = 0.0;
      }
    }
    // Yawrate
    _archive.deserialize(p_data, str);
    //	std::clog << "datum::processSegmentId: yawrate='" << str << "'" << std::endl;
    //	if (str.length() != 0) { 
    //	  _yawrate = std::stof(str); // Convert into float 
    //	}

    //    std::clog << "<<< datum::deserializeGpsData" << std::endl;
  }

  void datum::deserializeWlanData(std::vector<unsigned char> & p_data) {
    // Number of BSSID detected
    unsigned char bssidsNum = 0x00;
    _archive.deserialize(p_data,  reinterpret_cast<char *>(&bssidsNum));
    if (bssidsNum != 0x00) {
      //      std::clog << "datum::deserializeWlanData: #bssid=" << static_cast<unsigned int>(bssidsNum) << std::endl;
      for (unsigned char i = 0x00; i < bssidsNum; i++) { // Get BSSIDs
	unsigned char fieldsNum = 0x00;
	_archive.deserialize(p_data, reinterpret_cast<char *>(&fieldsNum));
	//	std::clog << "datum::deserializeWlanData: #field=" << static_cast<unsigned int>(fieldsNum) << std::endl;
	std::vector<std::string> v(fieldsNum);
	for (unsigned char j = 0x00; j < fieldsNum; j++) { // Get BSSIDs description
	  std::string str;
	  _archive.deserialize(p_data, str);
	  v[j] = str;
	} // End of 'for' statement
	//	std::clog << "datum::deserializeWlanData: key=" << v[0] << std::endl;
	_bssids.insert(std::pair<std::string, std::vector<std::string> >(v[0], v));
      } // End of 'for' statement
    }
  }

  const std::string & datum::toString() const {
    static std::string str;

    std::ostringstream os;
    os << 
      "timestamp=" << static_cast<unsigned int>(_timestamp) <<"," << 
      "xfer=" << _xfer << "," <<
      "bw=" << _bw << "," <<
      "atemp=" << _atemp << "," <<
      "dutyCycle=" << static_cast<unsigned int>(_dutyCycle) << "," << 
      "latitude=" << _latitude << "," << 
      "longitude=" << _longitude << "," << 
      "elevation=" << _elevation << "," << 
      "speed=" << _speed << "," << 
      "heading=" << _heading;
    //	"yawrate     = " << _yawrate << "," << 
    unsigned int counter = 0;
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = _bssids.begin(); it != _bssids.end(); ++it) {
      os << ",bssid#" << counter++ << "=" << it->first << ",";
      os << "  datarate=" << it->second[IDX_DATARATE] << ",";
      os << "  channelId=" << it->second[IDX_CHANNEL] << ",";
      os << "  rssid=" << it->second[IDX_RSSID] << ",";
      os << "  essid=" << it->second[IDX_ESSID];
    } // End of 'for' statement
    os << std::endl;
    str = os.str();
 
    return str;
  }

} // End of namespace beagleboneStorage
