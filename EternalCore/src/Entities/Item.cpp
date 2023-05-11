#include "Entities/item.h"

namespace Eternal
{
    namespace Entities
    {
        Item::Item()
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
            
        }
        Item::Item(Database::GetItemtype::Info* data)
            : Item()
        {
            auto item = (Info*)_info.get();
            item->id = data->id;
            item->name = std::string((char*)data->name, MAX_NAME_LEN);
            item->req_prof = data->req_profession;
            item->req_weaponskill = data->req_weaponskill;
            item->req_level = data->req_level;
            item->req_sex = data->req_sex;
            item->req_force = data->req_force;
            item->req_speed = data->req_speed;
            item->req_health = data->req_health;
            item->req_soul = data->req_soul;
            item->price = data->price;
            item->atk_max = data->atk_max;
            item->atk_min = data->atk_min;
            item->defense = data->defense;
            item->dexterity = data->dexterity;
            item->dodge = data->dodge;
            item->health = data->life;
            item->mana = data->mana;
            item->durability = data->dura;
            item->durability_limit = data->dura_limit;
            item->gem1 = data->gem1;
            item->gem2 = data->gem2;
            item->magic1 = data->magic1;
            item->magic2 = data->magic2;
            item->magic3 = data->magic3;
            item->magic_atk = data->magic_atk;
            item->magic_def = data->magic_def;
            item->atk_range = data->atk_range;
            item->atk_speed = data->atk_speed;           
        }

    }
}
