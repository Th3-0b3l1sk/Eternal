#pragma once
#include <unordered_map>
#include <memory>
#include "Util/comms.h"
#include "Database/Statements/GetItemsInfo.h"

namespace Eternal
{
    namespace Database
    {
        class Database;
    }

    namespace Entities
    {

        class ItemManager
        {
        public:
            ItemManager(Database::Database& db);
            ~ItemManager() = default;

        public:
            void load_game_items();
            size_t get_items_count() const { return _all_items.size(); }
            opt_ref<std::unique_ptr<Database::ItemInfo>>  get_item_by_id(uint32_t id);


        private:

        private:
            Database::Database& _db;
            std::unordered_map<uint32_t, std::unique_ptr<Database::ItemInfo>> _all_items;
        };
    }
}
