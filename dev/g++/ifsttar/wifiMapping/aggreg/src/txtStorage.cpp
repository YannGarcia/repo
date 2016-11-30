#include "txtStorage.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror

#include <unistd.h> // Used for usleep and close

namespace beagleboneStorage {

  txtStorage::txtStorage() : _file(), _storageDevice() {
    std::clog << ">>> beagleboneStorage::txtStorage::txtStorage" << std::endl;
    _family = abstractStorage::txtFile;
  } // End of Constructor

  txtStorage::~txtStorage() {
  } // End of Destructor

  int txtStorage::initialize(const std::string & p_storageDevice) {
    // Sanity check
    if (p_storageDevice.empty()) {
      std::cerr << "txtStorage::initialize: Wrong parameters" << std::endl;
      return -1;
    }
    // FIXME Check path validity

    _storageDevice = p_storageDevice;

    return 0;
  }

  int txtStorage::uninitialize() {
    return -1;
  }

  int txtStorage::open() {
    _file.open(_storageDevice, std::ofstream::out | std::ofstream::app);
    if (!_file.is_open()) {
      std::cerr << "txtStorage::open: Failed to open device: " << _storageDevice << std::endl;
      return -1;
    }

    return 0;
  }

  void txtStorage::close() {
    _file.close();
  }

  void txtStorage::store(const std::vector<unsigned char> & p_data) {
    datum dt(p_data);
    store(dt);
  }

  void txtStorage::store(const datum & p_datum) {
    std::string str;
    str.assign(p_datum.toString());
    //    std::clog << "txtStorage::store: '" << str << std::endl;
    _file.write(str.c_str(), str.length());
    _file.flush();
  }

} // End of namespace beagleboneStorage
