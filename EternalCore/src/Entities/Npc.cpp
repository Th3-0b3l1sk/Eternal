#include "Entities/Npc.h"
#include "Msg/MsgNpcInfo.h"

namespace Eternal
{
    namespace Entities
    {
        Npc::Npc(uint32_t id, std::string name, uint8_t type, uint16_t look, uint32_t map_id, uint16_t pos_x, uint16_t pos_y, uint8_t base, uint8_t sort)
            : Entity(id),
            _sort{ sort }, _base{ base }, _type{ type }
        {
            _name = name;
            _look = look;
            _map = map_id;
            _x = pos_x;
            _y = pos_y;
        }

        Npc::Npc(const Database::NpcInfo& info)
            : Npc(info.id, info.name, info.type, info.lookface, info.map_id, info.cell_x, info.cell_y, info.base, info.sort)
        {

        }

        Npc::Npc(Database::NpcInfo&& info)
            : Entity(info.id),
            _sort{ info.sort }, _base{ info.base }, _type{ info.type }
        {
            _name = std::move(info.name);
            _look = info.lookface;
            _map  = info.map_id;
            _x    = info.cell_x;
            _y    = info.cell_y;
        }

        void Npc::add_to_bc_set(std::shared_ptr<Entity> entity)
        {
            auto npc_info = std::make_shared<Msg::MsgNpcInfo>(
                _identity,
                _x,
                _y,
                _look,
                _type,
                _sort,
                _name
            );

            entity->send_me(npc_info);
        }
    }
}
