#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include "Database/Statements/GetItemtype.h"

namespace Eternal
{
    namespace Entities
    {
        class Item
        {
        public:
            enum class Position : uint8_t
            {
                INVENTORY = 0,
                ARMET,
                NECKLACE,
                ARMOR,
                RWEAPON,
                LWEAPON,
                RING,
                GOURD,
                SHOES,
                GARMENT,
            };

            static constexpr size_t MAX_EQUIPMENT = 9;
#pragma pack(push, 1)
            struct Info
            {
                uint32_t id;
                std::string name;
                uint8_t req_prof;
                uint8_t req_weaponskill;
                uint8_t req_level;
                uint8_t req_sex;
                uint16_t req_force;
                uint16_t req_speed;
                uint16_t req_health;
                uint16_t req_soul;
                uint32_t price;
                uint16_t atk_max;
                uint16_t atk_min;
                uint16_t defense;
                uint16_t dexterity;
                uint16_t dodge;
                uint16_t health;
                uint16_t mana;
                uint16_t durability;
                uint16_t durability_limit;
                uint8_t gem1;
                uint8_t gem2;
                uint8_t magic1;
                uint8_t magic2;
                uint8_t magic3;
                uint16_t magic_atk;
                uint16_t magic_def;
                uint16_t atk_range;
                uint16_t atk_speed;

                Info()
                    : name{ "Default" }
                { }
            };
#pragma pack(pop)

        public:
            Item();
            Item(Database::GetItemtype::Info* data);
        private:
            std::unique_ptr<uint8_t[]> _info;
        };
    }
}
