#include "Entities/ItemManager.h"
#include "Database/Database.h"

namespace Eternal
{
    namespace Entities
    {
        ItemManager::ItemManager(Database::Database& db)
            : _db(db)
        {
            _init();
        }

        opt_ref<std::unique_ptr<ItemManager::ItemInfo>> ItemManager::get_item_by_id(uint32_t id)
        {
            auto iter = _all_items.find(id);
            if (iter == _all_items.end())
                return std::nullopt;

            return std::reference_wrapper(iter->second);
        }

        void ItemManager::_init()
        {
            auto item_type_stmt = std::make_unique<Database::GetItemtype>();
            auto item_result = _db.execute(std::move(item_type_stmt));
            for (auto&& item : item_result) {
                auto item_type_info = (ItemInfo*)item.get();
                _all_items[item_type_info->id] = std::unique_ptr<ItemInfo>(item_type_info);
                item.release(); // item manager took owner ship of the pointer |
            }
        }
    }
}
