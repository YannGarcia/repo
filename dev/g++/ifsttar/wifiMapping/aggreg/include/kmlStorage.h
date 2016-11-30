#if !defined(__KMLSTORAGE_H__)
#define __KMLSTORAGE_H__

#include <iostream>
#include <fstream>
#include <memory> // Used for unique_ptr
#include <vector>

#include "abstractStorage.h"

#include "kmlImpl.h"

namespace beagleboneStorage {

  class kmlStorage : public abstractStorage {
    std::ofstream _file;
    std::string _storageDevice;
    kmlImpl & _kml;
    unsigned int _idCounter;
    std::vector<datum> _coordinates;

  public:
    kmlStorage();
    ~kmlStorage();

    virtual int initialize(const std::string & p_storageDevice);
    virtual int uninitialize();
    virtual int open();
    virtual void close();
    virtual void store(const std::vector<unsigned char> & p_data);
    virtual void store(const datum & p_datum);

  }; // End of class kmlStorage

} // End of namespace beagleboneStorage

using namespace beagleboneStorage;

#endif // __KMLSTORAGE_H__
