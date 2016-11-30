#if !defined(__STORAGEMANAGER_H__)
#define __STORAGEMANAGER_H__

#include <stdexcept> // std::out_of_range
#include <memory> // Used for unique_ptr

#include "abstractStorage.h"

namespace beagleboneStorage {

  class storageManager {

    static std::unique_ptr<storageManager> _instance;

  public:
    inline static storageManager & getInstance() { return *_instance; };

  private:
    storageManager();

  public:
    ~storageManager();

    int createStorageDevice(const abstractStorage::StorageFamily p_storageFamily, std::unique_ptr<abstractStorage> & p_storageInstance);
  }; // End of class storageManager

} // End of namespace beagleboneStorage

using namespace beagleboneStorage;

#endif // __STORAGEMANAGER_H__
