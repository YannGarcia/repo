/*!
 * \File      http_request.cc
 * \brief     Source file for the lightweight http server library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2020 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */

#include "http_request.hh"

//#include "logger_factory.hh"
#include "helper.hh"
#include "converter.hh"

namespace http_server {

  http_request::http_request(
                             struct MHD_Connection* p_connection,
                             const std::string& p_url,
                             const std::string& p_method,
                             const std::string& p_version,
                             logger::logger& p_logger
                             ) : _empty(""), _url(p_url), _method(p_method), _version(p_version), _body{}, _headers{}, _uri_args{}, _logger{p_logger} {
    _logger.info(">>> http_request::http_request: %s, %s, %s.", _url.c_str(), _method.c_str(), _version.c_str());

    MHD_get_connection_values(p_connection, MHD_HEADER_KIND, &this->headers_iterator, this);
    MHD_get_connection_values(p_connection, MHD_GET_ARGUMENT_KIND, &this->uri_args_iterator, this);
  }

  http_request::~http_request() {
    _logger.info(">>> http_request::~http_request.");
    _headers.clear();
    _uri_args.clear();
  }

  void http_request::set_body(const char* p_body) {
    _logger.info(">>> http_request::set_body.");

    if (p_body != nullptr) {
      if (_body.empty()) {
        _body.assign(p_body);
      } else {
        _body += p_body;
      }
    }
  }

  const std::string& http_request::get_authorization_header() {
    _logger.info(">>> http_request::get_authorization_header.");

    std::map<const std::string, const std::string>::const_iterator it = _headers.find("Authorization");
    if (it != _headers.cend()) {
      return it->second;
    }

    return _empty;
  }

  const std::string& http_request::get_redirect_uri_header() {
    _logger.info(">>> http_request::get_redirect_uri_header.");

    std::map<const std::string, const std::string>::const_iterator it = _headers.find("redirect_uri");
    if (it != _headers.cend()) {
      return it->second;
    }

    return _empty;
  }

  const std::string& http_request::get_response_type_header() {
    _logger.info(">>> http_request::get_response_type_header.");

    std::map<const std::string, const std::string>::const_iterator it = _headers.find("response_type");
    if (it != _headers.cend()) {
      return it->second;
    }

    return _empty;
  }

  const std::string& http_request::get_client_id_header() {
    _logger.info(">>> http_request::get_client_id_header.");

    std::map<const std::string, const std::string>::const_iterator it = _headers.find("client_id");
    if (it != _headers.cend()) {
      return it->second;
    }

    return _empty;
  }

  const std::string& http_request::get_scope_header() {
    _logger.info(">>> http_request::get_scope_header.");

    std::map<const std::string, const std::string>::const_iterator it = _headers.find("scope");
    if (it != _headers.cend()) {
      return it->second;
    }

    return _empty;
  }

  const std::string& http_request::get_uri() {
    _logger.info(">>> http_request::get_uri.");
    return _url;
  }

  int http_request::headers_iterator(
                                     void *p_cls,
                                     enum MHD_ValueKind p_kind,
                                     const char *p_key,
                                     const char *p_value) {
    static_cast<http_request*>(p_cls)->_logger.info(">>> http_request::headers_iterator: %s/%s.\n", p_key, p_value);
    static_cast<http_request*>(p_cls)->_headers.insert(std::make_pair<const std::string, const std::string>(std::string(p_key), std::string(p_value)));
    return MHD_YES;
  }

  int http_request::uri_args_iterator(
                                      void *p_cls,
                                      enum MHD_ValueKind p_kind,
                                      const char *p_key,
                                      const char *p_value) {
    static_cast<http_request*>(p_cls)->_logger.info(">>> http_request::uri_args_iterator: %s/%s.\n", p_key, p_value);
    static_cast<http_request*>(p_cls)->_uri_args.insert(std::make_pair<const std::string, const std::string>(std::string(p_key), std::string(p_value)));
    return MHD_YES;
  }

} // End of namespace http_server
