#include "Entities/item.h"

namespace Eternal
{
    namespace Entities
    {
        Item::Item()
            : _item_info{ 0 }
        {
            
        }

        Item::Item(const Database::PlayerOwnItem& item)
            : Item()
        {
            _item_info.id          = item.id;
            _item_info.type        = item.type;
            _item_info.owner_id    = item.owner_id;
            _item_info.player_id   = item.player_id;
            _item_info.dura        = item.dura;
            _item_info.dura_limit  = item.dura_limit;
            _item_info.ident       = item.ident;
            _item_info.position    = item.position;
            _item_info.gem1        = item.gem1;
            _item_info.gem2        = item.gem2;
            _item_info.magic1      = item.magic1;
            _item_info.magic2      = item.magic2;
            _item_info.magic3      = item.magic3;
            _item_info.restrain    = item.restrain;
            _item_info.bless       = item.bless;
            _item_info.enchant     = item.enchant;
            _item_info.is_locked   = item.is_locked;
            _item_info.color       = item.color;
        }
        Item::Item(Database::PlayerOwnItem&& item)
            : Item()
        {
            _item_info = std::move(item);
        }
    }
}
