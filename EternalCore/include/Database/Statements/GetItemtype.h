#pragma once
#include "IStatement.h"

namespace Eternal
{
    namespace Database
    {
        class GetItemtype :
            public IStatement
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                uint32_t id{};
                uint8_t name [MAX_NAME_LEN]{};
                uint8_t req_profession{};
                uint8_t req_weaponskill{};
                uint8_t req_level{};
                uint8_t req_sex{};
                uint16_t req_force{};
                uint16_t req_speed{};
                uint16_t req_health{};
                uint16_t req_soul{};
                uint8_t monopoly{};
                uint16_t weight{};    // TODO: what is this
                uint32_t price{};
                uint32_t task{};
                uint16_t atk_max{};
                uint16_t atk_min{};
                uint16_t defense{};
                uint16_t dexterity{};
                uint16_t dodge{};
                uint16_t life{};
                uint16_t mana{};
                uint32_t dura{};      // TODO: check if dura or stackin limit
                uint32_t dura_limit{};
                uint8_t status{};
                uint8_t gem1{};
                uint8_t gem2{};
                uint8_t magic1{};
                uint8_t magic2{};
                uint8_t magic3{};
                uint16_t magic_atk{};
                uint16_t magic_def{};
                uint16_t atk_range{};
                uint16_t atk_speed{};
                uint8_t fray_mode{};
                uint8_t repair_mode{};
                uint8_t type_mask{};
                uint32_t cps_price{};
            };
#pragma pack(pop)
        public:
            GetItemtype(uint32_t item_id = 0);
            GetItemtype(std::vector<uint32_t> item_ids);
            virtual ~GetItemtype() = default;
            virtual SQLRETURN bind() override;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() override;
            virtual void hook_stmt(std::string& stmt) override;

        private:
            std::vector<uint32_t> _item_ids;
        };
    }
}
