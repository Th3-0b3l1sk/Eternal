#pragma once
#include "Database/db_helper.h"
#include <string>
#include <vector>
#include <optional>

namespace Eternal
{
    namespace Database
    {
        struct ItemInfo
        {
            uint32_t id;
            std::string name;
            uint8_t req_profession;
            uint8_t req_weaponskill;
            uint8_t req_level;
            uint8_t req_sex;
            uint16_t req_force;
            uint16_t req_speed;
            uint16_t req_health;
            uint16_t req_soul;
            uint8_t monopoly;
            uint16_t weight;    
            uint32_t price;
            uint32_t task;
            uint16_t atk_max;
            uint16_t atk_min;
            uint16_t defense;
            uint16_t dexterity;
            uint16_t dodge;
            uint16_t life;
            uint16_t mana;
            uint32_t dura;      
            uint32_t dura_limit;
            uint8_t status;
            uint8_t gem1;
            uint8_t gem2;
            uint8_t magic1;
            uint8_t magic2;
            uint8_t magic3;
            uint16_t magic_atk;
            uint16_t magic_def;
            uint16_t atk_range;
            uint16_t atk_speed;
            uint8_t fray_mode;
            uint8_t repair_mode;
            uint8_t type_mask;
            uint32_t cps_price;
        };

        class GetItemsInfo 
        {
        public:
            GetItemsInfo(SQLHANDLE hCon);
            ~GetItemsInfo() {
                SQLFreeHandle(SQL_HANDLE_STMT, _hStmt);
            }
            SQLRETURN bind();
            std::optional<std::vector<ItemInfo>> execute();
        
        private:
            SQLHANDLE _hStmt;
            ItemInfo _item_info;
        };
    }
}
