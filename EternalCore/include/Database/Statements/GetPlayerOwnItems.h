#pragma once
#include "./Database/db_helper.h"
#include <vector>
#include <optional>


namespace Eternal
{
    namespace Database
    {
        struct PlayerOwnItem {
            uint32_t id;
            uint32_t type;
            uint32_t owner_id;
            uint32_t player_id;
            int16_t dura;
            int16_t dura_limit;
            int8_t ident;
            int8_t position;    // maybe an enum is better;
            int8_t gem1;
            int8_t gem2;
            int8_t magic1;
            int8_t magic2;
            int8_t magic3;
            int32_t restrain;
            int8_t bless;       // maybe an enum
            int8_t enchant;
            int8_t is_sussy;
            int8_t is_locked;
            int8_t color;       // maybe an enum
        };

        class GetPlayerOwnItems
            
        {
        public:
            GetPlayerOwnItems(SQLHANDLE hCon);
            ~GetPlayerOwnItems() { SQLFreeHandle(SQL_HANDLE_STMT, _hStmt); }
            
        public:
            SQLRETURN bind();
            std::optional<std::vector<PlayerOwnItem>>  execute();

        public:
            GetPlayerOwnItems& set_player_id(uint32_t player_id) { _player_id = player_id; return *this; }
            
        private:
            uint32_t _player_id;
            SQLHANDLE _hStmt;
            PlayerOwnItem _item;    // a placeholder for fetching results

        };
    }
}
