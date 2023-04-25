#pragma once
#include "db_helper.h"
#include "./Statements/IStatement.h"
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
            void load_statements(std::string_view file);
            std::vector<std::unique_ptr<uint8_t[]>> execute(std::unique_ptr<IStatement>&& statement);

        private:
            std::string_view id_to_stmt(StatementID id) const;


        private:
            SQLHANDLE _hEnv;
            SQLHANDLE _hCon;
            std::unordered_map<std::string, std::string> _statements;
        };
    }
}

