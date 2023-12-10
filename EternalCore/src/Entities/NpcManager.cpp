#include "Entities/NpcManager.h"
#include "Entities/Npc.h"
#include "Database/Database.h"


namespace Eternal
{
    namespace Entities
    {
        NpcManager::NpcManager(Database::Database& db)
            : _db(db)
        {
        }

        void NpcManager::load_game_npcs()
        {
            auto game_npcs = _db.get_game_npcs();
            for (auto&& npc : game_npcs.value()) {
                _all_npcs[npc.id] = std::make_shared<Npc>(std::move(npc));
            }
        }

        std::shared_ptr<Npc> NpcManager::get_npc_by_id(uint32_t id)
        {
            auto iter = _all_npcs.find(id);
            if (iter == _all_npcs.end())
                return nullptr;

            return iter->second;
        }        
    }
}
