#pragma once
#include <cstdint>
#include <string>
#include "Database/Statements/GetPlayerOwnItems.h"
#include "Util/co_defs.h"

namespace Eternal
{
    namespace Entities
    {
        class Item
        {

        public:
            Item();
            Item(const Database::PlayerOwnItem& data);
            Item(Database::PlayerOwnItem&& data);
            Item(const Item& other) = default;
            Item(Item&& other) = default;

        public:
            uint8_t get_position() const { return _item_info.position; }
            uint32_t get_type() const { return _item_info.type; }
            uint8_t get_color() const { return _item_info.color; }

        private:
            Database::PlayerOwnItem _item_info;
        };
    }
}
