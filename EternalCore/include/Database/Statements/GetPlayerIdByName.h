#pragma once
#include "Database/Database.h"

namespace Eternal
{
    namespace Database
    {
        class GetPlayerIdByName
        {
        public:
            GetPlayerIdByName(SQLHANDLE hCon);
            ~GetPlayerIdByName() { SQLFreeHandle(SQL_HANDLE_STMT, _hStmt); }

        public:
            SQLRETURN bind();
            std::uint32_t execute();
            
            GetPlayerIdByName& set_player_name(std::string name) { 
                if (name.size() >= MAX_NAME_LEN)
                    return *this;
                _player_name = name; 
                return *this;
            }

        private:
            SQLHANDLE _hStmt;
            std::string _player_name;
            std::uint32_t _player_id;   // id, not identity
        };
    }
}
