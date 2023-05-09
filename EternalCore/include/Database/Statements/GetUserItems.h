#pragma once
#include "IStatement.h"

namespace Eternal
{
    namespace Database
    {
        class GetUserItems :
            public IStatement
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                uint32_t id;
                uint32_t type;
                uint32_t owner_id;
                uint32_t player_id;
                uint16_t dura;
                uint16_t dura_limit;
                uint8_t ident;
                uint8_t position;
                uint8_t gem1;
                uint8_t gem2;
                uint8_t magic1;
                uint8_t magic2;
                uint8_t magic3;
                uint8_t restrain;
                uint8_t bless;
                uint8_t enchant;
                uint8_t sus;
                uint8_t locked;
                uint8_t color;
            };
#pragma pack(pop)
        public:

            GetUserItems(uint32_t user_id);
            virtual ~GetUserItems() = default;
            virtual SQLRETURN bind() override;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() override;

        private:

        };
    }
}
