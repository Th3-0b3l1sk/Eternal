#pragma once
#include <Windows.h>
#include <sqlext.h>
#include <sql.h>
#include <string_view>
#include <unordered_map>
namespace Eternal
{
    namespace Database
    {
        class Database
        {
        public:
            Database();
            Database(std::string_view dsn_name, std::string_view user_name, std::string_view password);
            ~Database() = default;

        public:
            void connect(std::string_view n, std::string_view w);
            void register_statement(std::string_view key, std::string_view statement);
            void load_statements(std::string_view file);


        private:
            std::string get_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE return_code);


        private:
            SQLHANDLE _hEnv;
            SQLHANDLE _hCon;
            std::unordered_map<std::string, std::string> _statements;
        };
    }
}

