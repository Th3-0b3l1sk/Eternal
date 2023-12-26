#pragma once
#include "Database/db_helper.h"
#include <string>
#include <vector>
#include <optional>

namespace Eternal
{
    namespace Database
    {
        struct NpcInfo
        {
            uint32_t id;
            std::string name;
            uint8_t type;
            uint32_t lookface;
            uint32_t map_id;
            uint16_t cell_x;
            uint16_t cell_y;
            uint16_t life;
            uint16_t maxlife;
            uint8_t base;
            uint8_t sort;
        };


        class GetNpcsInfo
        {
        public:
            GetNpcsInfo(SQLHANDLE hStmt);
            ~GetNpcsInfo() {
                SQLFreeHandle(SQL_HANDLE_STMT, _hStmt);
            }

            SQLRETURN bind() ;
            std::optional<std::vector<NpcInfo>> execute();

        private:
            SQLHANDLE _hStmt;
            NpcInfo _npc_info;
        };
    }
}
