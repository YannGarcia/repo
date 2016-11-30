#include "kmlStorage.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror

#include <unistd.h> // Used for usleep and close

namespace beagleboneStorage {

  kmlStorage::kmlStorage() : _file(), _storageDevice(), _kml(kmlImpl::getInstance()), _idCounter(1), _coordinates() {
    std::clog << ">>> beagleboneStorage::kmlStorage::kmlStorage" << std::endl;
    _family = abstractStorage::kmlFile;
  } // End of Constructor

  kmlStorage::~kmlStorage() {
  } // End of Destructor

  int kmlStorage::initialize(const std::string & p_storageDevice) {
    // Sanity check
    if (p_storageDevice.empty()) {
      std::cerr << "kmlStorage::initialize: Wrong parameters" << std::endl;
      return -1;
    }
    // FIXME Check path validity

    _storageDevice = p_storageDevice;

    return 0;
  }

  int kmlStorage::uninitialize() {
    return -1;
  }

  int kmlStorage::open() {
    _file.open(_storageDevice, std::ofstream::out | std::ofstream::app);
    if (!_file.is_open()) {
      std::cerr << "kmlStorage::open: Failed to open device: " << _storageDevice << std::endl;
      return -1;
    }

    // Create a LineStyleUrl, id = 1
    _kml.createLineStyle(_idCounter++, 5, static_cast<unsigned int>(0xffffffff));

    return 0;
  }

  void kmlStorage::close() {
    std::string str(_kml.toString());
    _file.write(str.c_str(), str.length());
    _file.flush();
    _file.close();
  }

  void kmlStorage::store(const std::vector<unsigned char> & p_data) {
    datum dt(p_data);
    store(dt);
  }

  void kmlStorage::store(const datum & p_datum) {
    std::clog << "kmlStorage::store: " << static_cast<unsigned int>(p_datum.get_timestamp()) << std::endl;

    _kml.createPlacemark(_idCounter++, p_datum.get_latitude(), p_datum.get_longitude(), p_datum.get_timestamp(), std::to_string(1), p_datum.get_xfer(), p_datum.get_bw(), p_datum.get_speed(), p_datum.get_elevation());
    /*_coordinates.push_back(p_datum);
    if (_coordinates.size() == 10) {
      
      for (std::vector<datum>::iterator it = _coordinates.begin() ; it != _coordinates.end(); ++it) {
	_kml.createPlacemark(_idCounter++, it->get_timestamp(), it->get_xfer(), it->get_bw(), it->get_latitude(), it->get_longitude(), it->get_speed(), it->get_elevation(), 0.97);
      } // End of 'for' statement
      _coordinates.clear();
      }*/
  }

} // End of namespace beagleboneStorage
