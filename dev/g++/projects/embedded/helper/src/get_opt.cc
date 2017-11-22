/*!
 * \file      get_opt.cpp
 * \brief     Implementation file for command line options support.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#include <fstream>

#include <unistd.h>

#include "get_opt.hh"

extern char **environ;

namespace helpers {
  
  namespace get_opt {

    GETOPT_INLINE token* get_opt::_add_token(const std::string & p_value, token::type p_type) {
      token* const ret = new token(p_value, p_type);
      if (_first_token == NULL) {
	_first_token = ret;
      } else {
	_last_token->link_to(ret);
      }
      _last_token = ret;
      return ret;
    }

    GETOPT_INLINE void get_opt::_init_flags() {
      std::stringstream ss;
      _flags = ss.flags();
    }

    GETOPT_INLINE void get_opt::_parse_sub_file(const std::string & p_file) {
      std::ifstream ifile(p_file.c_str());
      if (!ifile) {
	throw options_file_not_found_ex(p_file);
      }
    
      std::vector<std::string> args;
      std::string arg;

      while (ifile >> arg) {
	args.push_back(arg);
      }
    
      _parse(args);
    }

    GETOPT_INLINE void get_opt::_parse(const std::vector<std::string> & p_args) {
      bool any_option_processed = false;
      const size_t argc = p_args.size();

      size_t start = 0;
      if ( _app_name.empty() ) {
	_app_name = p_args[0];
	start = 1;
      }

      // parse arguments by their '-' or '--':
      //   (this will be a state machine soon)
      for (size_t i = start; i < argc; i++) {
	const std::string & currentArg = p_args[i];
      
	if (currentArg[0] == '-' && currentArg.size() > 1) {
	  // see what's next, differentiate whether it's short or long:
	  if (currentArg[1] == '-') {
	    if ( currentArg.size() > 2 ) {
	      // long option
	      _long_ops[currentArg.substr(2)]._token = _add_token(currentArg.substr(2), token::long_option);
	    } else {
	      // it's the -- option alone
	      _long_ops[currentArg]._token = _add_token(currentArg, token::global_argument);
	    }

	    any_option_processed = true;
	  } else {
	    // check if it is a negative number: rules
	    //  * floating point negative numbers are straight classified as 'arguments'
	    //  * integer negative numbers of more than 1 digit length are also 'arguments'
	    //  * integer negatives of 1 digit length can be either arguments or short options.
	    //  * anything else: short options.
	    int anInt;
	    float aFloat;
	    std::stringstream dummy;
	    if ( convert(currentArg, anInt, dummy.flags()) == _option::ok) {
	      if ( currentArg.size() > 2 ) {// if it's larger than -d (d=digit), then assume it's a negative number:
		_add_token(currentArg, any_option_processed ? token::unknown_yet : token::global_argument);
	      } else { // size == 2: it's a 1 digit negative number
		_short_ops[currentArg[1]]._token = _add_token(currentArg, token::possible_negative_argument);
	      }
	    } else if ( convert(currentArg, aFloat, dummy.flags()) == _option::ok) {
	      _add_token(currentArg, any_option_processed ? token::unknown_yet : token::global_argument);
	    } else {
	      // short option
	      // iterate over all of them, keeping the last one in currentData
	      // (so the intermediates will generate 'existent' arguments, as of '-abc')
	      for( size_t j = 1; j < currentArg.size(); j++ ) {
		_short_ops[currentArg[j]]._token = _add_token(std::string(currentArg, j, 1), token::short_option);
	      } // End of 'for' statement
	    }

	    any_option_processed = true;
	  }
	} else if ( currentArg[0] == '@' && currentArg.size() > 1 ) {
	  // suboptions file
	  _parse_sub_file(currentArg.substr(1));
	} else {
	  _add_token(currentArg, any_option_processed ? token::unknown_yet : token::global_argument);
	}
      } // End of 'for' statement

      _last = _option::ok;    // TODO: To be enhance
    }

    GETOPT_INLINE void get_opt::_parse_env() {
      // this will be optimized in version 3
      std::string var_name;
      std::string var_value;
      size_t var = 0;
      std::string::size_type pos;
      option_data* data;

      while (environ[var] != NULL) {
	var_name = environ[var];
	pos = var_name.find('=');
	
	if (pos != std::string::npos) {
	  var_value = var_name.substr(pos + 1);
	  var_name = var_name.substr(0, pos);
	  
	  if (_long_ops.find(var_name) == _long_ops.end()) {
	    data = &_long_ops[var_name];
	    data->_token = _add_token(var_name, token::long_option);
	    data->_flags = option_data::environment;
	    _add_token(var_value, token::option_argument);
	  }
	} else {
	    (data = &_long_ops[var_name])->_flags = option_data::environment;
	}
	var++;
      }
    }
    
    GETOPT_INLINE void get_opt::_argc_argv_to_vector(int argc, const char* const* const argv, std::vector<std::string> & p_args) {
      for (int i = 0; i < argc; i++)
	p_args.push_back(argv[i]);
    }

    GETOPT_INLINE get_opt::tokens_deleter::~tokens_deleter() {
      token* next;
      token* current(_first);
      while (current != NULL) {
	next = current->_next;
	delete current;
	current = next;
      } // End of 'while' statement
    }

    GETOPT_INLINE get_opt::get_opt(const int p_argc, const char* const* const p_argv) : _exc(std::ios_base::goodbit), _first_token(NULL), _last_token(NULL), _tokens_deleter(_first_token) {
      _init_flags();
      std::vector<std::string> args;
      _argc_argv_to_vector(p_argc, p_argv, args);
      _parse(args);
    }

    GETOPT_INLINE get_opt::get_opt(int p_argc, const char* const* const p_argv, const _env_tag_t)
      : _first_token(NULL), _last_token(NULL), _tokens_deleter(_first_token) {
      _init_flags();
      std::vector<std::string> args;
      _argc_argv_to_vector(p_argc, p_argv, args);
      _parse(args);
      _parse_env();
    }

    GETOPT_INLINE get_opt & get_opt::operator >> (const _option & p_opt) throw(get_opt_ex) {
      if (_last != _option::parsing_error) {
        _last = p_opt(_short_ops, _long_ops, _first_token, _flags);

        switch (_last) {
	case _option::ok:
	  break;

	case _option::option_not_found:
	  if (_exc & std::ios_base::eofbit)
	    throw option_not_found_ex();
	  break;

	case _option::bad_type:
	  if (_exc & std::ios_base::failbit)
	    throw invalid_format_ex();
	  break;

	case _option::no_args:
	  if (_exc & std::ios_base::eofbit)
	    throw argument_not_found_ex();
	  break;

	case _option::too_many_args:
	  if (_exc & std::ios_base::failbit)
	    throw too_many_arguments_ex();
	  break;

	case _option::option_not_found_no_ex:
	  break;  // Ok, it will be read by casting to bool

	case _option::parsing_error:
	  break;  // just to disable warning
	}
      }
      else if (_exc & std::ios_base::failbit)
	throw parsing_error_ex();

      return *this;
    }

    GETOPT_INLINE get_opt & get_opt::operator >> (std::ios_base & (* p_iomanip)(std::ios_base &)) {
      std::stringstream ss;
      ss.flags(_flags);
      _flags = (ss << p_iomanip).flags();
      return *this;
    }

    GETOPT_INLINE bool get_opt::options_remain() const {
      bool remain = false;
      short_options::const_iterator it = _short_ops.begin();
      while (it != _short_ops.end() && !remain) {
	remain = (it->second._flags == option_data::cmd_line_not_extracted);
	++it;
      } // End of 'while' statement

      if (!remain){
	long_options::const_iterator it = _long_ops.begin();
	while (it != _long_ops.end() && !remain) {
	  remain = (it->second._flags == option_data::cmd_line_not_extracted);
	  ++it;
	} // End of 'while' statement
      }

      if (!remain) {
	// check for the global arguments:
	token* token = _first_token;
	while (!remain && token != NULL) {
	  remain = (token->_type == token::global_argument || token->_type == token::unknown_yet);
	  token = token->_next;
	} // End of 'while' statement
      }

      return remain;
    }

  } // End of namespace get_opt

} // End of namespace helpers
