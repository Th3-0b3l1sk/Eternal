#pragma once
#include "Database/db_helper.h"
#include <optional>
#include <vector>

namespace Eternal
{
    namespace Database
    {
        struct MonsterGenerator
        {
            std::uint32_t id;
            std::uint32_t map_id;
            std::uint16_t bound_x;
            std::uint16_t bound_y;
            std::uint16_t bound_cx;
            std::uint16_t bound_cy;
            std::uint16_t max_npc;
            std::uint16_t reset_secs;
            std::uint16_t max_per_gen;
            std::uint32_t monster_type;
        };

        class GetMonsterGenerators
        {
        public:
            GetMonsterGenerators(SQLHANDLE hCon);
            ~GetMonsterGenerators() {
                SQLFreeHandle(SQL_HANDLE_STMT, _hStmt);
            }

            SQLRETURN bind();
            std::optional<std::vector<MonsterGenerator>> execute();
        private:
            SQLHANDLE _hStmt;
            MonsterGenerator _gen;
        };
    }
}
