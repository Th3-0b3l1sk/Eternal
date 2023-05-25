#pragma once
#include <cstdint>
#include <string>
#include "Database/Statements/GetUserItems.h"
#include "Util/co_defs.h"

namespace Eternal
{
    namespace Entities
    {
        class Item
        {

        public:
            Item();
            Item(Database::GetUserItems::Info* data);
            Item(const Item& other) = default;
            Item(Item&& other) = default;

        public:
            uint8_t get_position() const { return _position; }
            uint32_t get_type() const { return _type; }
            uint8_t get_color() const { return _color; }

        private:

            uint32_t _id;
            uint32_t _type;
            uint32_t _owner_id;
            uint32_t _player_id;
            uint16_t _durability;
            uint16_t _durability_limit;
            uint8_t  _ident;
            uint8_t  _position;
            uint8_t  _gem1;
            uint8_t  _gem2;
            uint8_t  _magic1;
            uint8_t  _magic2;
            uint8_t  _magic3;
            uint8_t  _restrian;
            uint8_t  _bless;
            uint8_t  _enchant;
            uint8_t  _locked;
            uint8_t  _color;
        };
    }
}
