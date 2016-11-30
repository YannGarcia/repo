#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include "beagleboneCommEx.h"

#include "sharedMemory.h"

namespace beagleboneComm {

  namespace ipc {

    sharedMemory::sharedMemory(const int p_key, const int p_size, const int p_offset, const AccessModes p_accessModes) {
      _sm = -1;
      _address = NULL;
      memset((void *)&_ds, 0xff, sizeof(_ds));
      _isCreator = false;

      // Set access flag
      int orights = 0600;
      switch (p_accessModes) {
      case sharedMemory::ro:
	orights = 0400;
	break;
      case sharedMemory::rw:
	break;
      } // End of 'switch' statement

      // Open the shared memory
      if ((_sm = ::shmget(p_key, p_size, IPC_EXCL | orights)) == -1) {
	std::ostringstream os;
        os << "sharedMemory::sharedMemory: Failed to open shared memory: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // Check creation is OK
      if (::shmctl(_sm, IPC_STAT, &_ds) == -1) {
 	std::ostringstream os;
        os << "sharedMemory::sharedMemory: Failed to stat shared memory: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // Attach it
      if ((_address = (unsigned char *)::shmat(_sm, NULL, 0)) == (unsigned char *)-1) { // Failed to attach the shared memory
 	std::ostringstream os;
        os << "sharedMemory::sharedMemory: Failed to attach the shared memory: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }
      _address += p_offset;

      // This instance is not the shared memory creator, do not remove the IPC
    } // End of constructor

    sharedMemory::sharedMemory(const int p_key, const int p_size) {
      _sm = -1;
      _address = NULL;
      memset((void *)&_ds, 0xff, sizeof(_ds));
      _isCreator = false;

      // Create the shared memory
      if ((_sm = ::shmget(p_key, p_size, IPC_CREAT  | IPC_EXCL | 0600)) == -1) { // Failed to create shared memory
 	std::ostringstream os;
        os << "sharedMemory::sharedMemory: Failed to create shared memory: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // Check creation is OK
      if (::shmctl(_sm, IPC_STAT, &_ds) == -1) {
 	std::ostringstream os;
        os << "sharedMemory::sharedMemory: Failed to stat shared memory: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // Attach it
      if ((_address = (unsigned char *)::shmat(_sm, NULL, 0)) == (unsigned char *)-1) { // Failed to attach the shared memory
 	std::ostringstream os;
        os << "sharedMemory::sharedMemory: Failed to attach the shared memory: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // This instance is the shared memory creator
      _isCreator = true;
    } // End of constructor

    sharedMemory::~sharedMemory() {
      close();
    } // End of destructor

    int sharedMemory::close() {
      std::clog << ">>> sharedMemory::close" << std::endl;

      if (_address != NULL) {
	::shmdt(_address);
	_address = NULL;
      }
      if (_isCreator) {
	if (_sm != -1) {
	  ::shmctl(_sm, IPC_RMID, NULL);
	  _sm = -1;
	}
      } else {
	_sm = -1;
      }

      return 0;
    }

    int sharedMemory::write(const std::vector<unsigned char> & p_buffer, const int p_length) {
      memcpy((void *)(_address), (const void *)p_buffer.data(), p_length);

      return 0;
    }

    int sharedMemory::read(std::vector<unsigned char> & p_buffer, const int p_length) {
      unsigned char *data = new unsigned char[p_length];
      memcpy((void *)data, (const void *)(_address), p_length);
      p_buffer.assign(data, data + p_length);
      delete [] data;

      return 0;
    }

     std::string sharedMemory::toString() const {
      std::ostringstream os;
      os << 
	"Id: " << std::dec << _sm << std::endl << 
	"Address: " << std::hex << std::setfill('0') << (int)_address << std::dec << std::endl <<  
	"Owner uid/gid: " << _ds.shm_perm.uid << "/" << _ds.shm_perm.gid << std::endl <<
	"Creator uid/gid: " << _ds.shm_perm.cuid << "/" << _ds.shm_perm.cgid << std::endl <<
	std::endl;
      return os.str();
    }

  } // End of namespace ipc

} // End of namespace beagleboneComm
