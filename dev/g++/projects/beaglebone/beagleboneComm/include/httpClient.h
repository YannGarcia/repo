#if !defined(__HTTPCLIENT_H__)
#define __HTTPCLIENT_H__

#include <string>
#include <memory> // Used for std::shared_ptr

#include <curl/curl.h> // libcurl

#include "abstractHttpData.h"

namespace beagleboneComm {

  namespace network {

    class httpClient {
      struct url_data {
	std::size_t size;
	char * data;
      };
      
      CURL *_curlHandle;

    public:
      enum HttpResultCodes : int {
	HttpOk = 0,
        HttpUnknown = -1000
      };

    public:
      httpClient();
      virtual ~httpClient();

      HttpResultCodes get(const std::string & p_url, std::string & p_data, const int p_timeout = 500);

      HttpResultCodes put(const std::string & p_url, const abstractHttpDataOut & p_dataOut, const int p_timeout = 500);

    private:
      static std::size_t writeData(const void * p_address, const std::size_t p_size, const std::size_t p_nmemb, struct url_data *p_data);

    }; // End of class httpClient

  } // End of namespace network

} // End of namespace beagleboneComm

using namespace beagleboneComm;

#endif // __HTTPCLIENT_H__
