#pragma once
#include "Database/db_helper.h"
#include <vector>
#include <optional>


namespace Eternal
{
    namespace Database
    {
        struct MonsterType
        {
            std::uint32_t id;
            std::string name;
            std::uint8_t type;
            std::uint8_t ai_type;
            std::uint32_t look;
            std::uint32_t level;
            std::uint32_t life;             // in the db stored as uint64_t
            std::uint32_t escape_life;      // in the db stored as uint64_t
            std::uint8_t attack_user;
            std::uint32_t attack_min;
            std::uint32_t attack_max;
            std::uint32_t defense;
            std::uint16_t dexterity;
            std::uint16_t dodge;
            std::uint32_t magic_type;
            std::uint32_t magic_def;
            std::uint32_t magic_hit_rate;
            std::uint8_t view_range;
            std::uint8_t attack_range;
            std::uint16_t attack_speed;
            std::uint16_t move_speed;
            std::uint16_t run_speed;
            std::uint16_t drop_armet;
            std::uint16_t drop_necklace;
            std::uint16_t drop_armor;
            std::uint16_t drop_ring;
            std::uint16_t drop_weapon;
            std::uint16_t drop_shield;
            std::uint16_t drop_shoes;
            std::uint32_t drop_money;   // in the db stored as uint64_t 
            std::uint32_t drop_hp;
            std::uint32_t drop_mp;
            std::uint32_t extra_battle_level;
            std::uint32_t extra_exp;
            std::uint16_t extra_damage;
        };


        class GetMonsterType
        {
        public:
            GetMonsterType(SQLHANDLE hCon);
            ~GetMonsterType() {
                SQLFreeHandle(SQL_HANDLE_STMT, _hStmt);
            }

            SQLRETURN bind();
            std::optional<std::vector<MonsterType>> execute();

        private:
            SQLHANDLE _hStmt;
            MonsterType _m_type;
        };
    }
}
