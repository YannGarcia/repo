/*!
 * \File      get_opt.h
 * \brief     Header file for command line options support.
 * \author    garciay.yann@gmail.com
 * \copyright Copyright (c) 2015-2017 ygarcia. All rights reserved
 * \license   This project is released under the MIT License
 * \version   0.1
 */
#pragma once

#include <string>
#include <vector>   // candidate to be removed
#include <map>
#include <sstream>
#include <list>

#ifndef GETOPT_INLINE
#define GETOPT_INLINE
#endif

/*! \namespace helpers
 *  \brief helpers namespace
 */
namespace helpers {

  /*! \namespace helpers::get_opt
   *  \brief Command line options processing specific helpers namespace
   */
  namespace get_opt {
    
    /*!
     * \struct token
     * \brief Description of a command line tokens
     */
    struct token {
      enum type_t {
        short_option,
        long_option,
        global_argument,
        global_argument_used, /*!< Already read, skip in the next read */
        option_argument,
        possible_negative_argument,
        unknown_yet  /*!< Can be a global option, or an option of the previous one */
      };
      /*!
       * \typedef 
       */
      typedef type_t type;
      /*!< */
      type _type;
      /*!< The token value */
      std::string _value;
      /*!< Pointer to the next token */
      token* _next;

      /*!
       * Constructor
       */
      token(const std::string& p_value, type p_type = unknown_yet) : _type(p_type), _value(p_value), _next(NULL) { };
      
      /*!
       * \fn bool is_last() const;
       * \brief Indicates if the token is the last item of the list
       * \return true if the token is the last item of the list, false otherwise
       */
      bool is_last() const {
        return _next == NULL;
      };
      
      /*!
       * \fn bool link_to() const;
       * \brief Indicates if the token has a neighbor in the list
       * \return true if the token is not the last item of the list, false otherwise
       */
      void link_to(token * p_new_next) {
        _next = p_new_next;
      };
      
      token* get_next_option_argument() const {
        if (is_last()) {
          return NULL;
        } else {
          if (_next->_type == unknown_yet || _next->_type == option_argument || _next->_type == possible_negative_argument) {
            return _next;
          }	  
          return NULL;
        }
	
      }; // End of method get_next_option_argument
      
    }; // End of struct token
 
    struct option_data {
      enum flags_t {
        cmd_line_not_extracted,
        cmd_line_extracted,
        environment
      };
      typedef flags_t flags;
      
      flags _flags;
      token* _token;
      option_data() : _flags(cmd_line_not_extracted) { };
    }; // End of struct option_data
    
    typedef std::map<std::string, option_data> long_options;
    typedef std::map<char, option_data> short_options;
      
    struct _option {
      enum result_t {
        ok,
        parsing_error,
        option_not_found,
        bad_type,
        no_args,
        too_many_args,
        option_not_found_no_ex
      };
      
      virtual ~_option() { };
      
      virtual result_t operator()(short_options & p_short_ops, long_options & p_long_ops, token* first, std::ios::fmtflags p_flags) const = 0;
      
      static const char NO_SHORT_OPT = 0;
    protected:
      static void set_token_as_used(token* p_token, short_options & p_short_ops, const token::type p_used_as) {
        if (p_token->_type == token::possible_negative_argument) {
          p_short_ops.erase(p_token->_value[1]);
        }
        p_token->_type = p_used_as;
      }; // End of method set_token_as_used
      
    }; // End of struct _option
    
    template <class T> inline _option::result_t convert(const std::string & p_string, T & p_result, const std::ios::fmtflags p_flags) {
      std::stringstream ss;
      ss.clear();
      ss.flags(p_flags);
      ss << p_string;
      ss >> p_result;
      if (ss.fail() || (!ss.eof() && ((p_flags & std::ios::boolalpha) == 0))) {
        return _option::bad_type;
      }
      return _option::ok;
    }; // End of method convert
    
    template<> inline _option::result_t convert<std::string>(const std::string & p_string, std::string & p_result, std::ios::fmtflags /*flags*/) {
      p_result = p_string;
      return _option::ok;
    }; // End of method convert
    
    template <class T> class _option_t_base : public _option {
      const char _short_opt;
      const std::string _long_opt;
    protected:
      T & _target;
      
      virtual result_t _assign(token* p_token, const std::ios::fmtflags p_flags, short_options & p_short_ops) const = 0;
      
    public:
      _option_t_base(const _option_t_base<T>& p_other) : _option(), _short_opt(p_other._short_opt), _long_opt(p_other._long_opt), _target(p_other._target) { };
      _option_t_base(const char p_short_opt, const std::string & p_long_opt, T & p_target) : _short_opt(p_short_opt), _long_opt(p_long_opt), _target(p_target) { };
      
      virtual result_t operator()(short_options & p_short_ops, long_options & p_long_ops, token* /*first*/, const std::ios::fmtflags p_flags) const {
        result_t ret = option_not_found;
        short_options::iterator it;
        if (_short_opt == _option::NO_SHORT_OPT) {
          it = p_short_ops.end();
        } else {
          it = p_short_ops.find(_short_opt);
        }
        if (it != p_short_ops.end()) {
          it->second._flags = option_data::cmd_line_extracted;
          ret = _assign(it->second._token, p_flags, p_short_ops);
        } else if (!_long_opt.empty()) {
          long_options::iterator it = p_long_ops.find(_long_opt);
          if (it != p_long_ops.end()) {
            it->second._flags = option_data::cmd_line_extracted;
            ret = _assign(it->second._token, p_flags, p_short_ops);
          }
        }
        
        return ret;
      } // End of cast operator
      
    }; // End of class _option_t_base
    
    template <class T> class _option_t : public _option_t_base<T> {
    protected:
      virtual _option::result_t _assign(token * p_token, const std::ios::fmtflags p_flags, short_options & p_short_ops) const {
        token* const option_token = p_token->get_next_option_argument();
        if (option_token == NULL) {
          /* return _Option::NpoArgs; */
          this->set_token_as_used(p_token, p_short_ops, token::option_argument);
          return convert<T>("true", this->_target, p_flags | std::ios::boolalpha);
        }
	
        this->set_token_as_used(option_token, p_short_ops, token::option_argument);
        return convert<T>(option_token->_value, this->_target, p_flags);
      }; // End of method _assign
      
    public:
      _option_t(const _option_t<T> & p_other) : _option_t_base<T>(p_other) { };
      _option_t(char p_short_opt, const std::string & p_long_opt, T & p_target) : _option_t_base<T>(p_short_opt, p_long_opt, p_target) { };
      
    }; // End of class _option_t
      
    template <class T> class _option_t<std::vector<T> > : public _option_t_base<std::vector<T> > {
    protected:
      virtual _option::result_t _assign(token * p_token, const std::ios::fmtflags p_flags, short_options & p_short_ops) const {
        token * option_token = p_token->get_next_option_argument();
        if (option_token != NULL) {
          _option::result_t result;
          //option_args::const_iterator it = args.begin();
          T temp;
          
          do {
            this->set_token_as_used(option_token, p_short_ops, token::option_argument);
            result = convert<T>(option_token->_value, temp, p_flags);
            if (result == _option::ok) {
              this->_target.push_back(temp);
            }
            option_token = option_token->get_next_option_argument();
          } while (option_token != NULL && result == _option::ok);
          return result;
        }
        return _option::no_args;
      }; // End of method _assign
      
    public:
      _option_t(const _option_t<std::vector<T> > & p_other) : _option_t_base<std::vector<T> >(p_other) { };
      _option_t(const char p_short_opt, const std::string & p_long_opt, std::vector<T> & p_target) : _option_t_base<std::vector<T> >(p_short_opt, p_long_opt, p_target) { };
    }; // End of class _option_t
  
    template <class T, class base_option> class _default_value_option : public base_option {
      const T _default_value;
    public:
      _default_value_option(const _default_value_option<T, base_option> & p_other) : base_option(p_other), _default_value(p_other._default_value) { };
      _default_value_option(const char p_short_opt, const std::string & p_long_opt, T & p_target, const T & p_default_value) : base_option(p_short_opt, p_long_opt, p_target), _default_value(p_default_value) { };

      virtual _option::result_t operator()(short_options & p_short_ops, long_options & p_long_ops, token * p_first, const std::ios::fmtflags p_flags) const {
        _option::result_t ret = base_option::operator()(p_short_ops, p_long_ops, p_first, p_flags);
        if (ret == _option::option_not_found) {
          this->_target = _default_value;
          ret = _option::ok;
        }
        return ret;
      }; // End of cast operator
      
    }; // End of class _default_value_option
  
    template <class T> class _global_option : public _option {
      T & _target;
    
      virtual result_t operator()(short_options & p_short_ops, long_options & p_long_ops, token * p_first, const std::ios::fmtflags p_flags) const {
        // find first token GlobalArgument or UnknownYet (candidate) or PossibleNegativeArgument (candidate too)
        token* token(p_first);
        bool found(false);
        while (token != NULL && !found) {
          found = (token->_type == token::global_argument || token->_type == token::unknown_yet || token->_type == token::possible_negative_argument);
          if (!found) {
            token = token->_next;
          }
        } // End of 'while' statement
        if (found) {
          this->set_token_as_used(token, p_short_ops, token::global_argument_used);
          return convert<T>(token->_value, _target, p_flags);
        } else {
          return option_not_found;
        }
      }; // End of cast operator
    public:
      _global_option(const _global_option<T> & p_other) : _target(p_other._target) { };
      _global_option(const T & p_target) : _target(p_target) { };
    }; // End of class _global_option

    template <class T> class _global_option<std::vector<T> > : public _option {
      std::vector<T>& _target;
      
      virtual result_t operator()(short_options & p_short_ops, long_options & /*long_ops*/, token * p_first, const std::ios::fmtflags p_flags) const {
        // find first token GlobalArgument or UnknownYet (candidate) or PossibleNegativeArgument (candidate too)
        token* token(p_first);
        bool found_any(false);
        T tmp;
        result_t res(ok);
	
        while (token != NULL && res == ok) {
          if (token->_type == token::global_argument || token->_type == token::unknown_yet || token->_type == token::possible_negative_argument) {
            res = convert<T>(token->_value, tmp, p_flags);
            if (res == ok) {
              this->set_token_as_used(token, p_short_ops, token::global_argument_used);
              found_any = true;
              _target.push_back(tmp);
            }
          }
          token = token->_next;
        } // End of 'while' statement
        if (res == ok) {
          if (found_any) {
            return res;
          } else {
            return option_not_found;
          }
        } else {
          return res;
        }
      }; // End of cast operator
    
    public:
      _global_option(const _global_option<std::vector<T> > & p_other) : _target(p_other._target) { };
      _global_option(const std::vector<T> & p_target) : _target(p_target) { };
    }; // End of class _global_option
  
    template <class T> inline _option_t<T> option(const char p_short_opt, const std::string & p_long_opt, T & p_target) {
      return _option_t<T>(p_short_opt, p_long_opt, p_target);
    };
    
    template <class T> inline _option_t<T> option(const char p_short_opt, T & p_target) {
      return _option_t<T>(p_short_opt, std::string(), p_target);
    };
    
    // LongOpt only
    template <class T> inline _option_t<T> option(const std::string & p_long_opt, T & p_target) {
      return _option_t<T>(_option::NO_SHORT_OPT, p_long_opt, p_target);
    };
    
    // Defaulted version
    template <class T> inline _default_value_option<T, _option_t<T> > option(const char p_short_opt, const std::string & p_long_opt, T & p_target, const T & p_def) {
      return _default_value_option<T, _option_t<T> >(p_short_opt, p_long_opt, p_target, p_def);
    };
    
    template <class T> inline _default_value_option<T, _option_t<T> > option(const char p_short_opt, T & p_target, const T & p_def) {
      return _default_value_option<T, _option_t<T> >(p_short_opt, std::string(), p_target, p_def);
    };
    
    // No short opt.
    template <class T> inline _default_value_option<T, _option_t<T> > option(const std::string & p_long_opt, T & p_target, const T & p_def) {
      return _default_value_option<T, _option_t<T> >(_option::NO_SHORT_OPT, p_long_opt, p_target, p_def);
    };
    
    // Defaults for strings:
    inline _default_value_option<std::string, _option_t<std::string> > option(const char p_short_opt, const std::string & p_long_opt, std::string & p_target, const char * p_def) {
      return _default_value_option<std::string, _option_t<std::string> >(p_short_opt, p_long_opt, p_target, p_def);
    };
    
    inline _option_t<std::string> option(const char p_short_opt, std::string & p_target, const char * p_def) {
      return _default_value_option<std::string, _option_t<std::string> >(p_short_opt, std::string(), p_target, p_def);
    };
    
    // No short opt.
    inline _default_value_option<std::string, _option_t<std::string> > option(const std::string & p_long_opt, std::string & p_target, const char * p_def) {
      return _default_value_option<std::string, _option_t<std::string> >(_option::NO_SHORT_OPT, p_long_opt, p_target, p_def);
    };
    
    // Global option:
    template <class T> inline _global_option<T> global_option(T & p_target) {
      return _global_option<T>(p_target);
    };
    
    class option_present : public _option {
      const char _short_opt;
      const std::string _long_opt;
      bool* const _present;
    public:
      // two combinations: with/without target, and with/without long opt.
      
      // WITH long_opt:
      option_present(const char p_short_opt, const std::string & p_long_opt, bool & p_present) : _short_opt(p_short_opt), _long_opt(p_long_opt), _present(&p_present) { };
      option_present(const char p_short_opt, const std::string & p_long_opt) : _short_opt(p_short_opt), _long_opt(p_long_opt), _present(NULL) { };
      // WITHOUT long_opt:
      option_present(const char p_short_opt, bool & p_present) : _short_opt(p_short_opt), _present(&p_present) { };
      option_present(const char p_short_opt) : _short_opt(p_short_opt), _present(NULL) { };
      // WITHOUT short_opt
      option_present(const std::string & p_long_opt, bool & p_present) : _short_opt(_option::NO_SHORT_OPT), _long_opt(p_long_opt), _present(&p_present) { };
      option_present(const std::string & p_long_opt) : _short_opt(_option::NO_SHORT_OPT), _long_opt(p_long_opt), _present(NULL) { };
    protected:
      virtual result_t operator()(short_options & p_short_ops, long_options & p_long_ops, token* /*first*/, const std::ios::fmtflags /*flags*/) const {
        bool found;
        short_options::iterator it = p_short_ops.find(_short_opt);
        found = (it != p_short_ops.end());
        if (found) {
          it->second._flags = option_data::cmd_line_extracted;
        } else if (!_long_opt.empty()) {
          long_options::iterator it = p_long_ops.find(_long_opt);
          found = (it != p_long_ops.end());
          if (found) {
            it->second._flags = option_data::cmd_line_extracted;
          }
        }
	
        if (_present != NULL) {
          *_present = found;
          return ok;
        } else {
          return found ? ok : option_not_found_no_ex;
        }
      }; // End of cast operator
      
    }; // End of class option_present
    
    class get_opt_ex : public std::exception { };
    struct parsing_error_ex : get_opt_ex { };
    struct invalid_format_ex : get_opt_ex { };
    struct argument_not_found_ex : get_opt_ex { };
    struct too_many_arguments_ex : get_opt_ex { };
    struct option_not_found_ex : get_opt_ex { };
    struct too_many_options_ex : get_opt_ex { };
    struct options_file_not_found_ex : get_opt_ex {
      const std::string _targetFile;
      
      options_file_not_found_ex(const std::string & p_file) : _targetFile(p_file) { };
      ~options_file_not_found_ex() throw() { };
    }; // End of struct options_file_not_found_ex 
    
    enum _env_tag_t {
      include_environment
    };

    class get_opt {
      short_options _short_ops;
      long_options _long_ops;
      std::ios_base::iostate _exc;
      _option::result_t _last;
      std::ios::fmtflags _flags;
      std::string _app_name;
      token* _first_token;
      token* _last_token;
      
      class tokens_deleter {
        token * & _first;
      public:
        tokens_deleter(token *&  p_first) : _first(p_first) { };
        GETOPT_INLINE ~tokens_deleter();
      }; // End of class tokens_deleter
      
      tokens_deleter _tokens_deleter;
      
      GETOPT_INLINE token * _add_token(const std::string& value, token::type type);
      GETOPT_INLINE void _init_flags();
      GETOPT_INLINE void _parse(const std::vector<std::string>& args);
      GETOPT_INLINE void _parse_env();
      static GETOPT_INLINE void _argc_argv_to_vector(int argc, const char* const* const argv, std::vector<std::string>& args);
      GETOPT_INLINE void _parse_sub_file(const std::string& file);
    public:
      GETOPT_INLINE get_opt(int argc, const char* const* const argv);
      GETOPT_INLINE get_opt(int argc, const char* const* const argv, const _env_tag_t);
      
      std::ios_base::iostate exceptions() const {
        return _exc;
      }
      void exceptions(std::ios_base::iostate except) {
        _exc = except;
      }
      void exceptions_all() {
        _exc = std::ios_base::failbit | std::ios_base::eofbit;
      }
      operator bool() const {
        return _last == _option::ok;
      }
      
      GETOPT_INLINE bool options_remain() const;
      
      void end_of_options() const throw(get_opt_ex) {
        if (options_remain() && (_exc & std::ios_base::eofbit)) {
          throw too_many_options_ex();
        }
      };
      
      std::ios::fmtflags flags() const {
        return _flags;
      };
      void flags(std::ios::fmtflags p_flags) {
        _flags = p_flags;
      };
      
      const std::string & app_name() const {
        return _app_name;
      };
      
      GETOPT_INLINE get_opt& operator >> (const _option & p_opt) throw(get_opt_ex);
      GETOPT_INLINE get_opt& operator >> (std::ios_base & (* p_iomanip)(std::ios_base &));
      
      // Alternative to manipulators, for those who don't like them: the 'getopt' method :)
      // Combination 1: with long option:
      template <class T> inline T getopt(const char p_short_opt, const std::string & p_long_opt) throw(get_opt_ex) {
        T result;
        operator >> (option(p_short_opt, p_long_opt, result));
        return result;
      };
      
      template <class T> inline T getopt(const char p_short_opt, const std::string & p_long_opt, const T & p_def_value) {
        T result;
        operator >> (option(p_short_opt, p_long_opt, result, p_def_value));
        return result;
      };
    
      // Combination 2: without long option:
      template <class T> inline T getopt(const char p_short_opt) throw(get_opt_ex) {
        T result;
        operator >> (option(p_short_opt, result));
        return result;
      };
    
      template <class T> inline T getopt(const char p_short_opt, const T & p_def_value) {
        T result;
        operator >> (option(p_short_opt, result, p_def_value));
        return result;
      };
      
      struct iterator_ctor_data {
        short_options::const_iterator _short_iter;
        long_options::const_iterator  _long_iter;
        get_opt * _getopt_pp;
      };
    
      template <class Container, class adapter, class option_type> class _iterator {
        typename Container::const_iterator _it;
        get_opt* _getopt_pp;
      public:
        _iterator(const iterator_ctor_data & p_ctor_data) {
          _it = adapter::adapt(p_ctor_data);
          _getopt_pp = p_ctor_data._getopt_pp;
        };
	
        _iterator() : _getopt_pp(NULL) { };
	
        _iterator<Container, adapter, option_type>& operator = (const _iterator<Container, adapter, option_type> & p_other) {
          _it = p_other._it;
          _getopt_pp = p_other._getopt_pp;
          return *this;
        }; // End of operator =
	
        bool operator != (const _iterator<Container, adapter, option_type> & p_other) const {
          return _it != p_other._it;
        };
	
        option_type option() const {
          return _it->first;
        };
        option_type operator*() const {
          return option();
        };
	
        _iterator<Container, adapter, option_type>& operator ++() {
          ++_it;
          return *this;
        };
	
        template <class T> get_opt & operator >> (T & p_token) {
          adapter::extract(p_token, *_getopt_pp, option());
          return *_getopt_pp;
        };
      }; // End of class _iterator
      
      iterator_ctor_data begin() {
        iterator_ctor_data ret;
        ret._short_iter = _short_ops.begin();
        ret._long_iter  = _long_ops.begin();
        ret._getopt_pp  = this;
        return ret;
      };
      
      iterator_ctor_data end() {
        iterator_ctor_data ret;
        ret._short_iter = _short_ops.end();
        ret._long_iter  = _long_ops.end();
        ret._getopt_pp  = this;
        return ret;
      };
      
      struct short_adapter {
        static short_options::const_iterator adapt(const iterator_ctor_data & p_data) {
          return p_data._short_iter;
        }; // End of method adapt
	
        template <class T> static void extract(T & p_token, get_opt & p_getopt_pp, const char p_option) {
          p_getopt_pp >> option(p_option, p_token);
        };
      }; // End of method adapt
      
      struct long_adapter {
        static long_options::const_iterator adapt(const iterator_ctor_data & p_data) {
          return p_data._long_iter;
        };
	
        template <class T> static void extract(T & p_token, get_opt & p_getopt_pp, const std::string & p_option) {
          p_getopt_pp >> option('\0', p_option, p_token);
        };
      };
      
      typedef _iterator<short_options, short_adapter, char> short_iterator;
      typedef _iterator<long_options, long_adapter, const std::string &> long_iterator;
    }; // End of class get_opt
    
    class Environment {
      // Coming soon!
    };
    
  } // End of namespace get_opt
  
} // End of namespace helpers

using namespace helpers::get_opt;
