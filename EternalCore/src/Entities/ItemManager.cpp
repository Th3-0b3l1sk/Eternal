#include "Entities/ItemManager.h"
#include "Database/Database.h"

namespace Eternal
{
    namespace Entities
    {
        ItemManager::ItemManager(Database::Database& db)
            : _db(db)
        {

        }

        void ItemManager::load_game_items()
        {
            auto items = _db.get_game_items();
            if (!items) {
                // todo: handle in-ability to load game items
                return;
            }

            for (auto&& item : items.value()) {
                _all_items[item.id] = std::make_unique<Database::ItemInfo>(item);
            }
        }

        opt_ref<std::unique_ptr<Database::ItemInfo>> ItemManager::get_item_by_id(uint32_t id)
        {
            auto iter = _all_items.find(id);
            if (iter == _all_items.end())
                return std::nullopt;

            return std::reference_wrapper(iter->second);
        }
    }
}
