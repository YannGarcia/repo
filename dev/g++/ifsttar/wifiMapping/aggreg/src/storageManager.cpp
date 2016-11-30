#include <cstring> // Used for strerror

#include "storageManager.h"

#include "fileStorage.h"
#include "txtStorage.h"
#include "kmlStorage.h"

namespace beagleboneStorage {

  std::unique_ptr<storageManager> storageManager::_instance(new storageManager());

  storageManager::storageManager() {
    // Nothing to do
  } // End of constructor

  storageManager::~storageManager() {
    // Nothing to do
  } // End of destructor

  int storageManager::createStorageDevice(const abstractStorage::StorageFamily p_storageFamily, std::unique_ptr<abstractStorage> & p_storageInstance) {

    switch (p_storageFamily) {
    case abstractStorage::binFile:
      p_storageInstance.reset(new fileStorage);
      break;
    case abstractStorage::txtFile:
      p_storageInstance.reset(new txtStorage);
      break;
    case abstractStorage::kmlFile:
      p_storageInstance.reset(new kmlStorage);
      break;
    } // End of 'switch' statement

    return 0;

  } // End of method createStorageDevice

} // End of namespace beagleboneStorage
