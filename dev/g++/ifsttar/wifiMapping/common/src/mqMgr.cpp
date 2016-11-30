#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include "mqMgr.h"
#include "ipcCommon.h"

namespace wifimapping {

  namespace ipc {

    mqMgr::mqMgr() {
      _mq = NULL;
    } // End of Constructor
     
    mqMgr::~mqMgr() {
      if (_mq != NULL) {
	delete _mq;
	_mq = NULL;
      }
    } // End of Destructor

    int mqMgr::create(const mqMgr::MqTypes p_type) {
      // Sanity check
      if (_mq != NULL) {
	std::cerr << "mqMgr::create: Message queue already created" << std::endl;
	return -1;
      }

      switch (p_type) {
      case mqMgr::guiMq:
	_mq = new messageQueue(MESSAGEQUEUE_KEY, static_cast<long>(mqMgr::guiMq));
	break;
      case mqMgr::adcMq:
	_mq = new messageQueue(MESSAGEQUEUE_KEY, static_cast<long>(mqMgr::adcMq));
	break;
      case mqMgr::dsMq:
	_mq = new messageQueue(MESSAGEQUEUE_KEY, static_cast<long>(mqMgr::dsMq));
	break;
      default:
	std::cerr << "mqMgr::create: Message queue type " << static_cast<unsigned int>(p_type) << " unknown" << std::endl;
	return -1;
      } // End of 'switch' statement

      return 0;
    }
     
    int mqMgr::open(const mqMgr::MqTypes p_type) {
      // Sanity checks
      if (_mq != NULL) {
	std::cerr << "mqMgr::open: Message queue already created" << std::endl;
	return -1;
      }

      switch (p_type) {
      case mqMgr::guiMq:
	_mq = new messageQueue(MESSAGEQUEUE_KEY, static_cast<long>(mqMgr::guiMq), messageQueue::rw);
	break;
      case mqMgr::adcMq:
	_mq = new messageQueue(MESSAGEQUEUE_KEY, static_cast<long>(mqMgr::adcMq), messageQueue::rw);
	break;
      case mqMgr::dsMq:
	_mq = new messageQueue(MESSAGEQUEUE_KEY, static_cast<long>(mqMgr::dsMq), messageQueue::rw);
	break;
      default:
	std::cerr << "mqMgr::open: Message queue type " << static_cast<unsigned int>(p_type) << " unknown" << std::endl;
	return -1;
      } // End of 'switch' statement

      return 0;
    }
     
    int mqMgr::close() {
      if (_mq != NULL) {
	return _mq->close();
	delete _mq;
      }

      return -1;
    }
    
    int mqMgr::write(const std::vector<unsigned char> & p_buffer, const int p_length) {
      // Sanity check
      if (_mq == NULL) {
	std::cerr << "mqMgr::write: Message queue not created" << std::endl;
	return -1;
      }

      /*std::cout << std::endl;
      for (auto it = p_buffer.cbegin(); it != p_buffer.cend(); ++it) {
	std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)*it << " ";
      } // End of 'for' statement
      std::cout << std::endl;*/

      return _mq->send(p_buffer, p_length);
    }
    
    int mqMgr::read(std::vector<unsigned char> & p_buffer, const int p_length) {
      // Sanity check
      if (_mq == NULL) {
	std::cerr << "mqMgr::read: Message queue not created" << std::endl;
	return -1;
      }

      /*int result = _mq->read(p_buffer, p_length);

      std::cout << std::endl;
      for (auto it = p_buffer.cbegin(); it != p_buffer.cend(); ++it) {
	std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)*it << " ";
      } // End of 'for' statement
      std::cout << std::endl;

      return result;*/
      return _mq->recv(p_buffer, p_length);
    }
 
    std::string mqMgr::toString() const {
      std::ostringstream os;
      os << "mqMgr: " << std::endl;
      if (_mq != NULL) {
	os << _mq->toString() << std::endl;
      }

      return os.str();
    }

  } // End of namespace ipc

} // End of namespace wifimapping
