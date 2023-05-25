#pragma once
#include <unordered_map>
#include <memory>
#include "Util/comms.h"
#include "Database/Statements/GetItemtype.h"

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
            typedef  Database::GetItemtype::Info ItemInfo;
        public:
            ItemManager(Database::Database& db);
            ~ItemManager() = default;

        public:
            size_t get_items_count() const { return _all_items.size(); }
            opt_ref<std::unique_ptr<ItemInfo>>  get_item_by_id(uint32_t id);


        private:
            void _init();

        private:
            Database::Database& _db;
            std::unordered_map<uint32_t, std::unique_ptr<ItemInfo>> _all_items;
        };
    }
}
