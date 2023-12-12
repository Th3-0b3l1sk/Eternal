#pragma once
#include "Database/db_helper.h"
#include "Database/Statements/GetPlayerInfo.h"

namespace Eternal
{
    namespace Database
    {
        class SetPlayerInfo
        {
        public:
            SetPlayerInfo(SQLHANDLE hCon);
            ~SetPlayerInfo() { SQLFreeHandle(SQL_HANDLE_STMT, _hStmt); }

        public:
            SQLRETURN bind();
            bool execute();

            // setters
            SetPlayerInfo& set_id(uint32_t id);
            SetPlayerInfo& set_name(const std::string& name);
            SetPlayerInfo& set_mate(const std::string& mate);
            SetPlayerInfo& set_lookface(uint32_t lookface);
            SetPlayerInfo& set_hair(uint16_t hair);
            SetPlayerInfo& set_money(uint32_t money);
            SetPlayerInfo& set_money_saved(uint32_t money_saved);
            SetPlayerInfo& set_cps(uint32_t cps);
            SetPlayerInfo& set_level(uint8_t level);
            SetPlayerInfo& set_exp(uint64_t exp);
            SetPlayerInfo& set_force(uint16_t force);
            SetPlayerInfo& set_dexterity(uint16_t dex);
            SetPlayerInfo& set_health(uint16_t health);
            SetPlayerInfo& set_soul(uint16_t soul);
            SetPlayerInfo& set_add_points(uint16_t add_points);
            SetPlayerInfo& set_life(uint16_t life);
            SetPlayerInfo& set_mana(uint16_t mana);
            SetPlayerInfo& set_profession(uint8_t prof);
            SetPlayerInfo& set_pk_points(uint16_t pk_points);
            SetPlayerInfo& set_virtue(uint32_t virtue);
            SetPlayerInfo& set_nobility(uint8_t nobility);
            SetPlayerInfo& set_rebirth(uint8_t rebirth);
            SetPlayerInfo& set_syndicate_id(uint16_t syn_id);
            SetPlayerInfo& set_record_map(uint32_t record_map);
            SetPlayerInfo& set_record_x(uint16_t record_x);
            SetPlayerInfo& set_record_y(uint16_t record_y);
            SetPlayerInfo& set_last_login(uint64_t last_login);


        private:
            SQLHANDLE _hStmt;
            PlayerInfo _info;
        };
    }
}
