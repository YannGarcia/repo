/*!
 * \file      http_request.h
 * \brief     Header file for the lightweight http server library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2020 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <iostream>     // std::cout, std::ostream, std::ios
#include <string>
#include <cstdint>
#include <vector>
#include <map>

#include <microhttpd.h>

#include "logger.hh"

/*! \namespace http_server
 *  \brief http server namespace
 */
namespace http_server {

  class http_request {
    const std::string _empty;
    std::string _url;
    std::string _method;
    std::string _version;
    std::string _body;
    std::map<const std::string, const std::string> _headers;
    std::map<const std::string, const std::string> _uri_args;
    logger::logger& _logger;

  public:
    http_request(
                 struct MHD_Connection* p_connection,
                 const std::string& p_url,
                 const std::string& p_method,
                 const std::string& p_version,
                 logger::logger& p_logger
                 );
    ~http_request();

    void set_body(const char* p_body);
    const std::string& get_authorization_header();
    const std::string& get_redirect_uri_header();
    const std::string& get_response_type_header();
    const std::string& get_client_id_header();
    const std::string& get_scope_header();
    const std::string& get_uri();

  private:

    static int headers_iterator(
                                void *p_cls,
                                enum MHD_ValueKind p_kind,
                                const char *p_key,
                                const char *p_value);
    static int uri_args_iterator(
                                 void *p_cls,
                                 enum MHD_ValueKind p_kind,
                                 const char *p_key,
                                 const char *p_value);

  }; // End of class http_request

} // End of namespave http_server
