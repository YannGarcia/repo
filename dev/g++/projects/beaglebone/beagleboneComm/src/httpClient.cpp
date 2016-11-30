#include <iostream>
#include <iomanip>
#include <cstring> // Used std::strerror, memcpy

#include "httpText.h"

namespace beagleboneComm {

  namespace network {

    httpClient::httpClient() : _curlHandle(NULL) {
    } // End of Constructor

    httpClient::~httpClient() {
      std::clog << ">>> httpClient::~httpClient" << std::endl;
    } // End of Destructor

    httpClient::HttpResultCodes httpClient::get(const std::string & p_url, std::string & p_data, const int p_timeout) {
      // TODO Sanity checks
      ::curl_global_init(0L);
      if ((_curlHandle = ::curl_easy_init()) == NULL) {
	std::cerr << "httpClient::get: Failed to initialize libcurl - " << std::strerror(errno) << std::endl;
	return httpClient::HttpUnknown;
      }

      struct url_data data;
      data.size = 0;
      data.data = static_cast<char *>(std::malloc(4096)); /* Reasonable size initial buffer */
      if(NULL == data.data) {
	std::cerr << "httpClient::get: Failed to allocate memory - " << std::strerror(errno) << std::endl;
	return httpClient::HttpUnknown;
      }
      data.data[0] = '\0';
  
      ::curl_easy_setopt(_curlHandle, CURLOPT_URL, p_url.c_str());
      ::curl_easy_setopt(_curlHandle, CURLOPT_WRITEFUNCTION, httpClient::writeData);
      ::curl_easy_setopt(_curlHandle, CURLOPT_WRITEDATA, &data);
      CURLcode result = ::curl_easy_perform(_curlHandle);
      if (result != CURLE_OK) {
	std::cerr <<  "httpClient::get: " << ::curl_easy_strerror(result) << std::endl;
      }
      // Store response
      p_data.assign(static_cast<const char *>(data.data));

      // Free resources
      std::free(data.data);
      ::curl_easy_cleanup(_curlHandle);

      return httpClient::HttpOk;
    }
    
    httpClient::HttpResultCodes httpClient::put(const std::string & p_url, const abstractHttpDataOut & p_dataOut, const int p_timeout) {
      return httpClient::HttpUnknown;
    }

    std::size_t httpClient::writeData(const void * p_address, const std::size_t p_size, const std::size_t p_nmemb, struct url_data *p_data) {
      std::size_t index = p_data->size;
      std::size_t n = (p_size * p_nmemb);
      char * tmp;
      
      p_data->size += (p_size * p_nmemb);      
      /*      std::clog << 
	std::hex << std::setw(8) << std::setfill('0') <<
	"data at " << p_address << 
	std::dec <<
	" size=" << static_cast<unsigned int>(p_size) << 
	" nmemb=" << static_cast<unsigned int>(p_nmemb) << std::endl;
      */
      tmp = static_cast<char *>(realloc(p_data->data, p_data->size + 1)); /* +1 for '\0' */
      
      if (tmp) {
	p_data->data = tmp;
      } else {
	if(p_data->data) {
	  free(p_data->data);
	}
	std::cerr << "Failed to allocate memory" << std::endl;
	return 0;
      }
      
      std::memcpy((p_data->data + index), p_address, n);
      p_data->data[p_data->size] = '\0';
      
      return p_size * p_nmemb;
    }
    
  } // End of namespace network
  
} // End of namespace beagleboneComm
  
