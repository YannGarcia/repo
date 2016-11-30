#if !defined(__TXTSTORAGE_H__)
#define __TXTSTORAGE_H__

#include <iostream>
#include <fstream>
#include <memory> // Used for unique_ptr
#include <vector>

#include "abstractStorage.h"

namespace beagleboneStorage {

  class txtStorage : public abstractStorage {
    std::ofstream _file;
    std::string _storageDevice;

  public:
    txtStorage();
    ~txtStorage();

    virtual int initialize(const std::string & p_storageDevice);
    virtual int uninitialize();
    virtual int open();
    virtual void close();
    virtual void store(const std::vector<unsigned char> & p_data);
    virtual void store(const datum & p_datum);

  }; // End of class txtStorage

} // End of namespace beagleboneStorage

using namespace beagleboneStorage;

#endif // __TXTSTORAGE_H__
