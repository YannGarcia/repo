#include <iomanip>
#include <sstream> // std::ostringstream
#include <cstring> // Used std::strerror, memcpy

#include "beagleboneCommEx.h"

#include "messageQueue.h"

//#include "dump.h"

namespace beagleboneComm {

  namespace ipc {

    messageQueue::messageQueue(const int p_key, const long p_msgType, const messageQueue::AccessModes p_accessModes) {
      _mq = -1;
      memset((void *)&_ds, 0xff, sizeof(_ds));
      _isCreator = false;
      _msgType = p_msgType;

      // Set access flag
      int orights = 0600;
      switch (p_accessModes) {
      case messageQueue::ro:
	orights = 0400;
	break;
      case messageQueue::rw:
	break;
      } // End of 'switch' statement

      // Open the message queue
      if ((_mq = ::msgget(p_key, IPC_EXCL | orights)) == -1) {
	std::ostringstream os;
        os << "messageQueue::messageQueue: Failed to open message queue: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // Check creation is OK
      if (::msgctl(_mq, IPC_STAT, &_ds) == -1) {
 	std::ostringstream os;
        os << "messageQueue::messageQueue: Failed to stat message queue: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // This instance is not the message queue creator, do not remove the IPC
    } // End of constructor

    messageQueue::messageQueue(const int p_key, const long p_msgType) {
      _mq = -1;
      memset((void *)&_ds, 0xff, sizeof(_ds));
      _isCreator = false;
      _msgType = p_msgType;

      // Create the messqge queue
      if ((_mq = ::msgget(p_key, IPC_CREAT  | IPC_EXCL | 0600)) == -1) { // Failed to create messqge queue
 	std::ostringstream os;
        os << "messageQueue::messageQueue: Failed to create messqge queue: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // Check creation is OK
      if (::msgctl(_mq, IPC_STAT, &_ds) == -1) {
 	std::ostringstream os;
        os << "messageQueue::messageQueue: Failed to stat messqge queue: " << std::strerror(errno) << std::endl;
	throw beagleboneCommEx(os.str());
      }

      // This instance is the messqge queue creator
      _isCreator = true;
    } // End of constructor

    messageQueue::~messageQueue() {
      close();
    } // End of destructor

    int messageQueue::close() {
      std::clog << ">>> messageQueue::close" << std::endl;

      if (_isCreator) {
	if (_mq != -1) {
	  ::msgctl(_mq, IPC_RMID, NULL);
	  _mq = -1;
	}
      } else {
	_mq = -1;
      }

      return 0;
    }

    int messageQueue::send(const std::vector<unsigned char> & p_buffer, const int p_length) {
      //      std::clog << ">>> messageQueue::send: " << std::dec << p_length << std::endl;

      unsigned char *data = (unsigned char *)new unsigned char[sizeof(long) + p_length];
      memcpy((void *)data, (const void *)&_msgType, sizeof(long));
      memcpy((void *)(data + sizeof(long)), (const void *)p_buffer.data(), p_length);

      //      dump::hexaDump(data, sizeof(long) + p_length);

      if (::msgsnd(_mq, (const void *)data, p_length/*Size of the payload*/, IPC_NOWAIT) == -1) {
	std::cerr << "messageQueue::messageQueue: Failed to send message: " << std::strerror(errno) << std::endl;
	delete [] data;
	return -1;
      }
      delete [] data;
      
      //      std::clog << "<<< messageQueue::send (0)" << std::endl;
      return 0;
    }

    int messageQueue::recv(std::vector<unsigned char> & p_buffer, const int p_length) {
      //      std::clog << ">>> messageQueue::recv: " << std::dec << p_length << std::endl;

      unsigned char *data = (unsigned char *)new unsigned char[sizeof(long) + p_length];

      int bytesRead = ::msgrcv(_mq, (void *)data, p_length, _msgType, IPC_NOWAIT);
      if (bytesRead == -1) {
	if (errno == E2BIG) {
	  if (::msgrcv(_mq, (void *)data, p_length, _msgType, IPC_NOWAIT | MSG_NOERROR) == -1) {
	    std::cerr << "messageQueue::messageQueue: Failed to receive message: " << std::strerror(errno) << std::endl;
	    delete [] data;
	    return -2;
	  }
	} else if (errno != ENOMSG) { // 'No message of desired type'
	  std::cerr << "messageQueue::messageQueue: Failed to receive message: " << std::strerror(errno) << std::endl;
	  delete [] data;
	  return -1;
	} else {
	  //std::clog << "<<< messageQueue::recv (0)" << std::endl;
	  delete [] data;
	  return 0;
	}
      }

      p_buffer.assign(data + sizeof(long), data + sizeof(long) + bytesRead);
      delete [] data;
      //      p_buffer.resize(bytesRead);

      //      std::clog << "<<< messageQueue::recv (" << p_buffer.size() << ")" << std::endl;
      return p_buffer.size();
    }

    std::string messageQueue::toString() const {
      std::ostringstream os;
      os << 
	"Id: " << std::dec << _mq << std::endl << 
	"Owner uid/gid: " << _ds.msg_perm.uid << "/" << _ds.msg_perm.gid << std::endl <<
	"Creator uid/gid: " << _ds.msg_perm.cuid << "/" << _ds.msg_perm.cgid << std::endl <<
	std::endl;

      return os.str();
    }

  } // End of namespace ipc

} // End of namespace beagleboneComm
