#if !defined(__FILESTORAGE_H__)
#define __FILESTORAGE_H__

#include <iostream>
#include <fstream>
#include <memory> // Used for unique_ptr
#include <vector>

#include "abstractStorage.h"

namespace beagleboneStorage {

  class fileStorage : public abstractStorage {
    std::ofstream _file;
    std::string _storageDevice;

  public:
    fileStorage();
    ~fileStorage();

    virtual int initialize(const std::string & p_storageDevice);
    virtual int uninitialize();
    virtual int open();
    virtual void close();
    virtual void store(const std::vector<unsigned char> & p_data);
    inline virtual void store(const datum & p_datum) { return; };

  }; // End of class fileStorage

} // End of namespace beagleboneStorage

using namespace beagleboneStorage;

#endif // __FILESTORAGE_H__
