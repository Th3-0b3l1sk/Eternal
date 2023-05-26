#pragma once
#include <unordered_map>
#include <memory>

namespace Eternal
{
    namespace Database
    {
        class Database;
    }

    
    namespace Entities
    {       
        class Npc;
        class NpcManager
        {
            using container = std::unordered_map<uint32_t, std::shared_ptr<Npc>>;
        public:
            NpcManager(Database::Database& db); 
            ~NpcManager() = default;
            
        public:
            size_t get_npcs_count() const { return _all_npcs.size(); }
            std::shared_ptr<Npc>  get_npc_by_id(uint32_t id);
            container& expose_container() { return _all_npcs; }

        private:
            void _init();

        private:
            Database::Database& _db;
            container _all_npcs;
        };
    }
}
