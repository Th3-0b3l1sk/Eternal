#pragma once
#include "./Register.h"

namespace Eternal
{
    namespace Database
    {
        class GetAccountInfo 
        {
        public:
            GetAccountInfo(SQLHANDLE hCon);
            ~GetAccountInfo() {
                SQLFreeHandle(SQL_HANDLE_STMT, _hStmt);
            }
            SQLRETURN  bind() ;

        public:
            SQLRETURN execute();
            
        public:
            GetAccountInfo& set_account_id(std::string name) { _name = name; return *this; }

        public:
            std::string&& let_name() { return std::move(_name); }
            std::string&& let_password() { return std::move(_password); }
            std::string&& let_ip() { return std::move(_ip); }
            bool is_online() const { return _is_online; }
            uint8_t account_type() const { return _type; }
            uint32_t get_id() const { return _id; }

        private:
            uint32_t _id;
            std::string _name;
            std::string _password;
            std::string _ip;
            uint8_t _is_online;
            uint8_t _type;

            SQLHANDLE _hStmt;
        };
    }
}
