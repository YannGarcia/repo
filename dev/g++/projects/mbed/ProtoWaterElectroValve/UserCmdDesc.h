#pragma once

#include <string>
#include <vector>

#include "Debug.h"

class UserCmdDesc {
    std::string _cmd;
    std::vector<string> _parms;
    
    public:
        UserCmdDesc() : _cmd(), _parms() { };
        virtual ~UserCmdDesc() { };

        inline void setCommand(const std::string & p_cmd) { _cmd.assign(p_cmd); };
        inline void addParam(const std::string & p_param) { _parms.push_back(p_param); };
        
        inline std::string & getCommand() { return _cmd; };
        inline unsigned int getParmsNumber() { return _parms.size(); };
        inline std::string & getParmNo(const unsigned char p_parmNo) { 
            static std::string result;
            if (p_parmNo <  _parms.size()) {
                return _parms.at(p_parmNo);
            }
            return result;
        };
        
}; // End of class UserCmdDesc
