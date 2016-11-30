#if !defined(__ABSTRACTHTTPDATA_H__)
#define __ABSTRACTHTTPDATA_H__

namespace beagleboneComm {

  namespace network {

    class abstractHttpData {
    protected:
      /**
       * @desc Get a specific header
       */
      virtual bool getHeader(char* header, size_t maxHeaderLen) { return false; };

    }; // End of class abstractHttpData

    class abstractHttpDataOut : public abstractHttpData {
    protected:
      friend class httpClient;

      /** 
       * @desc Reset stream to its beginning. Called by the httpClient on each new request
       */
      virtual void readReset() = 0;

      /** 
       * @desc Read a piece of data to be transmitted
       * @param p_buffer Pointer to the buffer on which to copy the data
       * @param p_length Length of the buffer
       * @param p_readLength Pointer to the variable on which the actual copied data length will be stored
       * @return 0 on success, -1 otherwise
       */
      virtual int read(std::string & p_buffer, const std::size_t p_length, std::size_t * p_readLength) = 0;
  
      /** 
       * @desc Get MIME type
       * @param p_type Internet media type from Content-Type header
       * @return 0 on success, -1 otherwise
       */
      virtual int getDataType(std::string & p_type, const std::size_t p_maxTypeLength) = 0;
  
      /** 
       * @desc * @desc Determine whether the HTTP client should chunk the data. Used for Transfer-Encoding header
       */
      virtual bool getIsChunked() = 0;
  
      /** 
       * @desc If the data is not chunked, get its size. Used for Content-Length header
       */
      virtual std::size_t getDataLen() = 0;

    }; // End of class abstractHttpDataOut

    class abstractHttpDataIn : public abstractHttpData {
    protected:
      friend class httpClient;

      /** 
       * @desc Reset stream to its beginning 
       * Called by the HTTPClient on each new request
       */
      virtual void writeReset() = 0;

      /** 
       * @desc Write a piece of data transmitted by the server
       * @param p_buffer Pointer to the buffer from which to copy the data
       * @param p_length Length of the buffer
       * @return 0 on success, -1 otherwise
       */
      virtual int write(const std::string & p_buffer, const std::size_t p_length) = 0;

      /** 
       * @desc Set MIME type
       * @param p_type Internet media type from Content-Type header
       */
      virtual void setDataType(const std::string & p_type) = 0;

      /** 
       * @desc Determine whether the data is chunked. Recovered from Transfer-Encoding header
       */
      virtual void setIsChunked(const bool p_chunked) = 0;
  
      /** 
       * @desc If the data is not chunked, set its size. From Content-Length header
       */
      virtual void setDataLen(const std::size_t p_lengh) = 0;

    }; // End of class abstractHttpDataIn

  } // End of namespace network

} // End of namespace beagleboneComm

using namespace beagleboneComm;

#endif // __ABSTRACTHTTPDATA_H__
