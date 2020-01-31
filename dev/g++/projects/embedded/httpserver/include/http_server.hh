/*!
 * \file      http_server.h
 * \brief     Header file for the lightweight http server library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2020 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <memory>

#include "http_request.hh"
#include "logger.hh"

/*! \namespace http_server
 *  \brief http server namespace
 */
namespace http_server {

  typedef void (*http_response_callback)(const std::unique_ptr<http_request>& p_request, std::map<std::string, std::string>& p_response_headers, std::string& p_response, void*);

  /**
   * \brief 
   * /remark See https://www.gnu.org/software/libmicrohttpd/tutorial.html#Adding-a-layer-of-security
   */
  class http_server {
    struct MHD_Daemon* _deamon_handle;
    int32_t _port;
    uint32_t _timeout;
    std::string _key_pem_path;
    std::string _key_pem;
    std::string _cert_pem_path;
    std::string _cert_pem;
    std::string _favicon_path;
    std::vector<unsigned char> _favicon;
    std::string _realm;
    std::string _user;
    std::string _password;
    std::string _bad_request;
    std::string _not_found;
    uint8_t _state;
    std::unique_ptr<http_request> _request;
    std::map<const std::string, std::tuple<void*/*http_response_callback*/, void*> > _callbacks;
    std::string _buffer;
    logger::logger& _logger;

  public:
    http_server(const std::string& p_realm, const std::string& p_user, const std::string& p_password, const std::string& p_key_pem_path, const std::string& p_cert_pem_path, const std::string& p_favicon_path, logger::logger& p_logger, const int32_t p_port = 8080, const uint32_t p_timeout = 120);

    virtual ~http_server();

    int32_t start();
    int32_t stop();
    void register_response_callback(const std::string& p_callback_id, http_response_callback p_callback, void* p_param);
    void prepare_response(const std::string& p_callback_id, std::map<std::string, std::string>& p_response_headers, std::string& p_response);

    inline void set_bad_request(const std::string& p_bad_request) { _bad_request = p_bad_request; };
    inline void set_not_found(const std::string& p_not_found) { _not_found = p_not_found; };
    inline const std::vector<unsigned char>& get_favicon() { return _favicon; };

  protected:
    static int32_t policy_callback(void* p_cls, const struct sockaddr* p_addr, socklen_t p_addrlen);
    static int32_t http_request_answer(
                                       void* p_cls,
                                       struct MHD_Connection* p_connection,
                                       const char* p_url,
                                       const char* p_method,
                                       const char* p_version,
                                       const char* p_upload_data,
                                       size_t* p_upload_data_size,
                                       void** p_con_cls
                                       );
    int32_t process_http_request_answer(
                                        struct MHD_Connection* p_connection,
                                        const char* p_url,
                                        const char* p_method,
                                        const char* p_version,
                                        const char* p_upload_data,
                                        size_t* p_upload_data_size,
                                        void** p_con_cls
                                        );
    static void request_http_completed(
                                       void* p_cls,
                                       struct MHD_Connection* p_connection,
                                       void** p_con_cls,
                                       enum MHD_RequestTerminationCode toe
                                       );
    void process_request_http_completed(
                                        struct MHD_Connection* p_connection,
                                        void** p_con_cls,
                                        enum MHD_RequestTerminationCode p_toe
                                        );
    int32_t ask_for_authentication(struct MHD_Connection* p_connection, const std::string& p_realm);
    int32_t is_authenticated(struct MHD_Connection* p_connection, const std::string& p_username, const std::string& p_password);
  }; // End of class http_server

} // End of namespace http_server
