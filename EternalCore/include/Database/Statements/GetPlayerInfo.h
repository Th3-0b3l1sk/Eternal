#pragma once
#include "Database/db_helper.h"
#include <stdint.h>
#include <string>
#include <optional>

namespace Eternal
{
    namespace Database
    {
        class GetPlayerInfo;
        struct PlayerInfo
        {
            friend class GetPlayerInfo;
            // id is internal i.e. db
        private:
            uint32_t id;        
        public:
            // identity is used by the server and the client
            uint32_t identity;
            std::string name;
            std::string mate;
            uint32_t lookface;
            uint16_t hair;
            uint32_t money;
            uint32_t money_saved;
            uint32_t cps;
            uint8_t level;
            uint64_t exp;
            uint16_t force;
            uint16_t dexterity;
            uint16_t health;
            uint16_t soul;
            uint16_t add_points;
            uint16_t life;
            uint16_t mana;
            uint8_t profession; // maybe an enum
            uint16_t pk_points;
            uint32_t virtue;
            uint8_t nobility;
            uint8_t rebirth;
            uint16_t syndicate_id;
            uint32_t record_map;
            uint16_t record_x;
            uint16_t record_y;
            uint64_t last_login;
        };
             

        class GetPlayerInfo 
        {
        public:

        public:
            GetPlayerInfo(SQLHANDLE hCon);
            ~GetPlayerInfo() { SQLFreeHandle(SQL_HANDLE_STMT, _hStmt); }

        public:
            SQLRETURN bind();
            std::optional<PlayerInfo> execute();

        public:
            GetPlayerInfo& set_player_id(uint32_t player_id) {
                _player_info.identity = player_id; 
                _player_info.id       = player_id - IDENTITY_START;  
                return *this; }

        private:
            SQLHANDLE _hStmt;
            PlayerInfo _player_info;
        };
    }
}

