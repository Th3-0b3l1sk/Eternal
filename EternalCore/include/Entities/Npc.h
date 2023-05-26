#pragma once
#include "Entities/Entity.h"
#include "Database/Statements/GetNpc.h"

namespace Eternal
{
    namespace Entities
    {
        class Npc : public Entity
        {
        public:
            Npc(uint32_t id, std::string name, uint8_t type,
                uint16_t look, uint32_t map_id, uint16_t pos_x, uint16_t pos_y,
                uint8_t base, uint8_t sort);

            Npc(Database::GetNpc::Info* info);
            virtual ~Npc() = default;

        public:
            virtual void add_to_bc_set(std::shared_ptr<Entity> entity);
            

        private:
            uint8_t _type;
            uint8_t _base;
            uint8_t _sort;
        };
    }
}
