#if !defined(__HTTPTEXT_H__)
#define __HTTPTEXT_H__

#include <cmath>

#include "httpClient.h"

namespace beagleboneComm {

  namespace network {

    class httpText : public abstractHttpDataIn, public abstractHttpDataOut {
      std::string _str;
      std::size_t _pos;

    public:
      /** 
       * @desc Create an HTTPText instance for output
       * @param buffer String to be transmitted
       */
      httpText(const char * p_buffer, const std::size_t p_length = 0) : _str(p_buffer), _pos(0) { 
	if ((p_length != 0) && (_str.length() > p_length)) {
	  // Resize the string
	  _str.resize(p_length);
	}
      };

      ~httpText() { };

    protected:
      /**
       * @desc Get a specific header
       */
      inline bool getHeader(char* header, size_t maxHeaderLen) { return false; };

      /** 
       * @desc Reset stream to its beginning. Called by the httpClient on each new request
       */
      inline void readReset() { _pos = 0; };
      
      /** 
       * @desc Read a piece of data to be transmitted
       * @param p_buffer Pointer to the buffer on which to copy the data
       * @param p_length Length of the buffer
       * @param p_readLength Pointer to the variable on which the actual copied data length will be stored
       * @return 0 on success, -1 otherwise
       */
      inline int read(std::string & p_buffer, const std::size_t p_length, std::size_t * p_readLength) {
	*p_readLength = fmin(p_length, _str.length());
	p_buffer.assign(_str.substr(_pos, *p_readLength));
	_pos += *p_readLength;

	return 0;
      };
      
      /** 
       * @desc Get MIME type
       * @param p_type Internet media type from Content-Type header
       * @return 0 on success, -1 otherwise
       */
      inline int getDataType(std::string & p_type, const std::size_t p_maxTypeLength) {
	p_type.assign("text/plain");
	if (p_maxTypeLength < p_type.length()) {
	  p_type.resize(p_maxTypeLength);
	}

	return 0;
      };
  
      /** 
       * @desc Determine whether the HTTP client should chunk the data. Used for Transfer-Encoding header
       */
      inline bool getIsChunked() { return false; };
  
      /** 
       * @desc If the data is not chunked, get its size. Used for Content-Length header
       */
      inline std::size_t getDataLen() { return static_cast<std::size_t>(_str.length() - _pos); };

      /** 
       * @desc Reset stream to its beginning 
       * Called by the HTTPClient on each new request
       */
      inline void writeReset() { _str.clear(); _pos = 0; };

      /** 
       * @desc Write a piece of data transmitted by the server
       * @param p_buffer Pointer to the buffer from which to copy the data
       * @param p_length Length of the buffer
       * @return 0 on success, -1 otherwise
       */
      inline int write(const std::string & p_buffer, const std::size_t p_length) {
	_str.append(p_buffer);
	_pos += p_length;

	return 0;
      };

      /** 
       * @desc Set MIME type
       * @param p_type Internet media type from Content-Type header
       */
      inline void setDataType(const std::string & p_type) { };

      /** 
       * @desc Determine whether the data is chunked. Recovered from Transfer-Encoding header
       */
      inline void setIsChunked(const bool p_chunked) { };
  
      /** 
       * @desc If the data is not chunked, set its size. From Content-Length header
       */
      inline void setDataLen(const std::size_t p_lengh) { };

    }; // End of class httpText

  } // End of namespace network

} // End of namespace beagleboneComm

using namespace beagleboneComm;

#endif // __HTTPTEXT_H__
