#pragma once
#include "Database/Statements/IStatement.h"

namespace Eternal
{
    namespace Database
    {
        class GetNpc :
            public IStatement
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                uint32_t id;
                uint8_t name[MAX_NAME_LEN];
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
#pragma pack(pop)
        public:
            GetNpc(uint32_t npc_id = 0);
            virtual ~GetNpc() = default;
            virtual SQLRETURN bind() override;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() override;
            virtual void hook_stmt(std::string& stmt) override;
        };
    }
}
