/*!
 * \File      http_server.cc
 * \brief     Source file for the lightweight http server library.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2020 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */

#include "http_server.hh"

//#include "logger_factory.hh"
#include "helper.hh"
#include "converter.hh"

namespace http_server {

  http_server::http_server(const std::string& p_realm, const std::string& p_user, const std::string& p_password, const std::string& p_key_pem_path, const std::string& p_cert_pem_path, const std::string& p_favicon_path, logger::logger& p_logger, const int32_t p_port, const uint32_t p_timeout) : _deamon_handle(NULL), _port(p_port), _timeout(p_timeout), _key_pem_path(p_key_pem_path), _key_pem{}, _cert_pem_path(p_cert_pem_path), _cert_pem{}, _favicon_path(p_favicon_path), _favicon{}, _realm(p_realm), _user(p_user), _password(p_password), _bad_request(""), _not_found(""), _state(0), _request(nullptr), _callbacks{}, _buffer{}, _logger{p_logger} {
    _logger.info(">>> http_server: %d, %d, %s, %s.", p_port, p_timeout, p_key_pem_path.c_str(), p_cert_pem_path.c_str());
  }

  http_server::~http_server() {
    _logger.info(">>> ~http_server: %p.", _deamon_handle);
    stop();
    _key_pem.clear();
    _cert_pem.clear();
    _favicon.clear();
  }

  int32_t http_server::start() {
    _logger.info(">>> http_server::start.");

    // Sanity checks
    if (_deamon_handle != NULL) {
      _logger.error("http_server::start: Daemon already started.");
      return -1;

    }

    std::vector<unsigned char> buffer;
    int result = helper::get_instance().file_load(_key_pem_path, buffer);
    if (result == -1) {
      _logger.error("http_server::start: Failed to load %s.", _key_pem_path);
      return -1;
    }
    _key_pem.assign(buffer.begin(), buffer.end());
    buffer.clear();
    result = helper::get_instance().file_load(_cert_pem_path, buffer);
    if (result == -1) {
      _logger.error("http_server::start: Failed to load %s.", _cert_pem_path);
      return -1;
    }
    _cert_pem.assign(buffer.begin(), buffer.end());
    result = helper::get_instance().file_load(_favicon_path, _favicon);
    if (result == -1) {
      _logger.error("http_server::start: Failed to load %s.", _favicon_path);
      return -1;
    }
    _logger.info("http_server::start: The key/certificate files were loaded.");

    _deamon_handle = MHD_start_daemon(
                                      MHD_USE_ERROR_LOG | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_TLS,
                                      /* MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, */
                                      /*MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,*/
                                      /* MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | MHD_USE_POLL, */
                                      /* MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | MHD_USE_POLL, */
                                      /* MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, */
                                      _port,
                                      &policy_callback,
                                      this,
                                      &http_server::http_request_answer,
                                      this,
                                      MHD_OPTION_CONNECTION_TIMEOUT,
                                      (unsigned int)_timeout,
                                      MHD_OPTION_NOTIFY_COMPLETED,
                                      http_server::request_http_completed,
                                      this,
                                      MHD_OPTION_HTTPS_MEM_KEY,
                                      _key_pem.c_str(),
                                      MHD_OPTION_HTTPS_MEM_CERT,
                                      _cert_pem.c_str(),
                                      MHD_OPTION_END
                                      );
    if (_deamon_handle == NULL) {
      _logger.info("http_server::start: Failed to start daemon");
      return -1;
    }

    return 0;
  }

  int32_t http_server::stop() {
    _logger.info(">>> http_server::stop: %p.", _deamon_handle);

    // Sanity check
    if (_deamon_handle == NULL) {
      return -1;
    }

    MHD_stop_daemon(_deamon_handle);
    _deamon_handle = NULL;

    _request = nullptr;

    return 0;
  }

  void http_server::register_response_callback(const std::string& p_callback_id, http_response_callback p_callback, void* p_param) {
    _logger.info(">>> http_server::register_response_callback: %s, %p.", p_callback_id.c_str(), p_param);

    std::map<const std::string, std::tuple<void*/*http_response_callback*/, void*> >::const_iterator it = _callbacks.find(p_callback_id);
    if (it != _callbacks.cend()) {
      _callbacks.erase(it);
    }
    _callbacks.insert(std::make_pair<const std::string&, std::tuple<void*/*http_response_callback*/, void*> >(p_callback_id, std::make_tuple((void*)p_callback, p_param)));
  }

  void http_server::prepare_response(const std::string& p_callback_id, std::string& p_response) {
    _logger.info(">>> http_server::prepare_response: %s.", p_callback_id.c_str());

    std::map<const std::string, std::tuple<void*/*http_response_callback*/, void*> >::const_iterator it = _callbacks.find(p_callback_id);
    if (it == _callbacks.cend()) {
      p_response.assign(_bad_request);
    } else {
      ((http_response_callback)(std::get<0>(it->second)))(_request, p_response, std::get<1>(it->second));
    }
    _logger.info("<<< http_server::prepare_response: %d.", p_response.length());
  }

  int32_t http_server::policy_callback(void* p_cls, const struct sockaddr* p_addr, socklen_t p_addrlen) {
    static_cast<http_server*>(p_cls)->_logger.info(">>> http_server::policy_callback: %d.", p_addrlen);

    return MHD_YES;
  }

  int32_t http_server::http_request_answer(
                                           void* p_cls,
                                           struct MHD_Connection* p_connection,
                                           const char* p_url,
                                           const char* p_method,
                                           const char* p_version,
                                           const char* p_upload_data,
                                           size_t* p_upload_data_size,
                                           void** p_con_cls
                                           ) {
    static_cast<http_server*>(p_cls)->_logger.info(">>> http_server::http_request_answer.");

    return static_cast<http_server*>(p_cls)->process_http_request_answer(p_connection, p_url, p_method, p_version, p_upload_data, p_upload_data_size, p_con_cls);
  }

  int32_t http_server::process_http_request_answer(
                                                   struct MHD_Connection* p_connection,
                                                   const char* p_url,
                                                   const char* p_method,
                                                   const char* p_version,
                                                   const char* p_upload_data,
                                                   size_t* p_upload_data_size,
                                                   void** p_con_cls
                                                   ) {
    _logger.info(">>> http_server::process_http_request_answer: %d.", _state);

    if (_state == 0) {
      // Build request object
      _request.reset(new http_request(p_connection, p_url, p_method, p_version, _logger));

      _state = 1;
      _buffer.clear();

      return MHD_YES;
    }

    // Check for authentication
    if (is_authenticated (p_connection, _user, _password) == -1) {
      _logger.info("http_server::process_http_request_answer: Call ask_for_authentication.");
      return ask_for_authentication (p_connection, _realm);
    }
    _logger.info("http_server::process_http_request_answer: Authorization done successfully.");

    if (*p_upload_data_size != 0) {
      _logger.info("http_server::process_http_request_answer: Store HTTP Body message: %s.", *p_upload_data);
      _request->set_body(p_upload_data);
      *p_upload_data_size = 0;
      return MHD_YES;
    }

    /* Prepare the response */
    const std::string& callback_id = _request->get_uri();
    prepare_response(callback_id, _buffer);
    struct MHD_Response* response = MHD_create_response_from_buffer(_buffer.length(), (void*)_buffer.c_str(), MHD_RESPMEM_PERSISTENT);
    int32_t result = MHD_queue_response(p_connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return result;
  }

  void http_server::request_http_completed(
                                           void* p_cls,
                                           struct MHD_Connection* p_connection,
                                           void** p_con_cls,
                                           enum MHD_RequestTerminationCode toe
                                           ) {
    static_cast<http_server*>(p_cls)->_logger.info(">>> http_server::request_http_completed.");

    static_cast<http_server*>(p_cls)->process_request_http_completed(p_connection, p_con_cls, toe);
  }

  void http_server::process_request_http_completed(
                                                   struct MHD_Connection* p_connection,
                                                   void** p_con_cls,
                                                   enum MHD_RequestTerminationCode p_toe
                                                   ) {
    _logger.info(">>> http_server::process_request_http_completed: %d.", p_toe);
    _state = 0;
    _request = nullptr;
  }

  int32_t http_server::ask_for_authentication(struct MHD_Connection* p_connection, const std::string& p_realm) {
    _logger.info(">>> http_server::ask_for_authentication: %s.", p_realm.c_str());

    static std::string base("Basic realm=");
    int32_t result = MHD_NO;

    struct MHD_Response* response = MHD_create_response_from_buffer (0, NULL, MHD_RESPMEM_PERSISTENT);
    if (response == NULL) {
      _logger.error("http_server::ask_for_authentication: Failed to generate response.");
      return result;
    }

    std::string header = base + p_realm;
    _logger.info("http_server::ask_for_authentication: WWW-Authenticate=%s.", header.c_str());
    result = MHD_add_response_header(response, "WWW-Authenticate", header.c_str());
    if (result == 0) {
      _logger.error("http_server::ask_for_authentication: Failed to generate response.");
      MHD_destroy_response(response);
      return MHD_NO;
    }

    result = MHD_queue_response(p_connection, MHD_HTTP_UNAUTHORIZED, response);
    MHD_destroy_response(response);

    _logger.error("<<< http_server::ask_for_authentication: %d\n", result);
    return result;
  }

  int32_t http_server::is_authenticated(struct MHD_Connection* p_connection, const std::string& p_username, const std::string& p_password) {
    _logger.info(">>> http_server::is_authenticated: %s/%s.", p_username.c_str(), p_password.c_str());

    static std::string base("Basic ");

    const std::string& header = _request->get_authorization_header();
    _logger.info("http_server::is_authenticated: Authorization: %s.", header.c_str());
    if (header.empty()) {
      _logger.info("http_server::is_authenticated: No Authorization header.");
      return -1;
    }
    std::string s = header.substr(0, 6);
    _logger.info("http_server::is_authenticated: s:%s.", s.c_str());
    if (base.compare(s) != 0) {
      _logger.warning("http_server::is_authenticated: Invalid Authorization header.");
      return -1;
    }

    std::string str = converter::get_instance().string_to_base64(p_username + ":" + p_password);
    if (str.empty()) {
      _logger.error("http_server::is_authenticated: Invalid base64 encoding.");
      return -1;
    }

    s = header.substr(6);
    _logger.info("http_server::is_authenticated: s:%s/%s.", s.c_str(), str.c_str());
    if (s.compare(str) != 0) {
      return -1;
    }

    _logger.info("http_server::is_authenticated: 0.");
    return 0;
  }

} // End of namespace http_server
