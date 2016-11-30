#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include "smMgr.h"
#include "ipcCommon.h"

namespace wifimapping {

  namespace ipc {

    smMgr::smMgr() {
      _sm = NULL;
      _address = NULL;
      _segmentId = -1;
    } // End of Constructor
     
    smMgr::~smMgr() {
      if (_sm != NULL) {
	delete _sm;
	_sm = NULL;
      }
    } // End of Destructor

    int smMgr::create() {
      // Sanity check
      if (_sm != NULL) {
	std::cerr << "smMgr::create: Shared memory already created" << std::endl;
	return -1;
      }

      _segmentId = 0; // Creator segment part
      _sm = new sharedMemory(SHAREDMEMORY_KEY, SHAREDMEMORY_SEGMENT_SIZE * SHAREDMEMORY_SEGMENT_NUMBER);
      _address = _sm->getBaseAddress(); // Physical address of the shared memory
      // Reset the shared memory
      memset((void *)_address, 0x00, SHAREDMEMORY_SEGMENT_SIZE * SHAREDMEMORY_SEGMENT_NUMBER);

      return 0;
    }
     
    int smMgr::open(const int p_segmentId) {
      // Sanity checks
      if (_sm != NULL) {
	std::cerr << "smMgr::open: Shared memory already created" << std::endl;
	return -1;
      }
      if (!((p_segmentId < SHAREDMEMORY_SEGMENT_NUMBER) || (p_segmentId == 0))) {
	std::cerr << "smMgr::open: Wrong parameter: " << p_segmentId << std::endl;
	return -2;
      }

      _segmentId = p_segmentId;
      _sm = new sharedMemory(SHAREDMEMORY_KEY, SHAREDMEMORY_SEGMENT_SIZE, SHAREDMEMORY_SEGMENT_SIZE * _segmentId/*offset*/, sharedMemory::rw);
      _address = _sm->getBaseAddress(); // Physical address of the segment identifed by p_segmentId
      printf("smMgr::open: Offset is %d - Physical address: 0x%08x\n", (int)(SHAREDMEMORY_SEGMENT_SIZE * _segmentId), (unsigned int)_address);

      return 0;
    }
     
    int smMgr::close() {
      // Sanity check
      if (_sm == NULL) {
	return -1;
      }
      return _sm->close();
    }
    
    int smMgr::write(const std::vector<unsigned char> & p_buffer, const int p_length) {
      // Sanity check
      if (_sm == NULL) {
	std::cerr << "smMgr::write: Shared memory not created" << std::endl;
	return -1;
      }

      /*std::cout << std::endl;
      for (auto it = p_buffer.cbegin(); it != p_buffer.cend(); ++it) {
	std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)*it << " ";
      } // End of 'for' statement
      std::cout << std::endl;*/

      return _sm->write(p_buffer, p_length);
    }
    
    int smMgr::read(std::vector<unsigned char> & p_buffer, const int p_length) {
      // Sanity check
      if (_sm == NULL) {
	std::cerr << "smMgr::read: Shared memory not created" << std::endl;
	return -1;
      }

      /*int result = _sm->read(p_buffer, p_length);

      std::cout << std::endl;
      for (auto it = p_buffer.cbegin(); it != p_buffer.cend(); ++it) {
	std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)*it << " ";
      } // End of 'for' statement
      std::cout << std::endl;

      return result;*/
      return _sm->read(p_buffer, p_length);
    }
 
    std::string smMgr::toString() const {
      std::ostringstream os;
      os << 
	"Segment id: " << std::dec << _segmentId << std::endl << 
	"Address: " << std::hex << std::setfill('0') << (int)_address << std::dec << std::endl;
      if (_sm != NULL) {
	os << _sm->toString() << std::endl;
      }

      return os.str();
    }

  } // End of namespace ipc

} // End of namespace wifimapping
