#include "Entities/NpcManager.h"
#include "Entities/Npc.h"
#include "Database/Database.h"
#include "Database/Statements/GetNpc.h"


namespace Eternal
{
    namespace Entities
    {
        NpcManager::NpcManager(Database::Database& db)
            : _db(db)
        {
            _init();
        }

        std::shared_ptr<Npc> NpcManager::get_npc_by_id(uint32_t id)
        {
            auto iter = _all_npcs.find(id);
            if (iter == _all_npcs.end())
                return nullptr;

            return iter->second;
        }

        void NpcManager::_init()
        {
            using Database::GetNpc;

            auto npc_stmt = std::make_unique<Database::GetNpc>();
            auto npc_result = _db.execute(std::move(npc_stmt));
            for (auto&& npc : npc_result) {
                auto npc_info = (GetNpc::Info*)npc.get();
                _all_npcs[npc_info->id] = std::make_shared<Npc>(npc_info);
                npc.release(); // npc manager took owner ship of the pointer |
            }
        }
    }
}
