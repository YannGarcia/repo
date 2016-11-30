#include "fileStorage.h"

#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror

#include <unistd.h> // Used for usleep and close

namespace beagleboneStorage {

  fileStorage::fileStorage() : _file(), _storageDevice() {
    std::clog << ">>> beagleboneStorage::fileStorage::fileStorage" << std::endl;
    _family = abstractStorage::binFile;
  } // End of Constructor

  fileStorage::~fileStorage() {
  } // End of Destructor

  int fileStorage::initialize(const std::string & p_storageDevice) {
    // Sanity check
    if (p_storageDevice.empty()) {
      std::cerr << "fileStorage::initialize: Wrong parameters" << std::endl;
      return -1;
    }
    // FIXME Check path validity

    _storageDevice = p_storageDevice;

    return 0;
  }

  int fileStorage::uninitialize() {
    return -1;
  }

  int fileStorage::open() {
    _file.open(_storageDevice, std::ofstream::binary | std::ofstream::out | std::ofstream::app);
    if (!_file.is_open()) {
      std::cerr << "fileStorage::open: Failed to open device: " << _storageDevice << std::endl;
      return -1;
    }

    return 0;
  }

  void fileStorage::close() {
    _file.close();
  }

  void fileStorage::store(const std::vector<unsigned char> & p_data) {
    _file.write(reinterpret_cast<const char *>(p_data.data()), p_data.size());
    _file.flush();
  }

} // End of namespace beagleboneStorage
