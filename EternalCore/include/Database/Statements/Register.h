#pragma once
#include "./Database/db_helper.h"

namespace Eternal
{
    namespace Database
    {
        
        class Register
        {
        public:
            Register(SQLHANDLE hCon);
            ~Register() {
                SQLFreeHandle(SQL_HANDLE_STMT, _hStmt);
            }

        public:
            Register& set_name(std::string_view name) { _name = name; return *this; }
            Register& set_password(std::string_view password) { _password = password; return *this; }
            Register& set_ip(std::string_view ip) { _ip = ip; return *this; }
            Register& set_type(uint8_t type) { type = _type; return *this; }

            
        public:
            SQLRETURN bind();
            SQLRETURN execute();

        private:
            std::string _name;
            std::string _password;
            std::string _ip;
            uint8_t _type;
            SQLHANDLE _hStmt;
        };
    }
}
