#pragma once
#include "Database/Statements/IStatement.h"


namespace Eternal
{
    namespace Database
    {
        class GetUser :
            public IStatement
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                Info()
                {
                    ZeroMemory(name, MAX_NAME_LEN);
                    ZeroMemory(mate, MAX_NAME_LEN);
                }

                uint32_t id;
                uint32_t identity;
                uint8_t name[MAX_NAME_LEN];
                uint8_t mate[MAX_NAME_LEN];
                uint32_t lookface;
                uint16_t hair;
                uint32_t money;
                uint32_t money_saved;
                uint32_t cps;
                uint8_t level;
                uint64_t exp;
                uint16_t force;
                uint16_t dexterity;
                uint16_t health;
                uint16_t soul;
                uint16_t add_points;
                uint16_t life;
                uint16_t mana;
                uint8_t profession;
                uint16_t pk;
                uint32_t virtue;
                uint8_t nobility;
                uint8_t rebirth;
                uint16_t syndicate_id;
                uint32_t record_map;
                uint16_t record_x;
                uint16_t record_y;
                uint64_t last_login;
            };
#pragma pack(pop)

        public:
            GetUser(uint32_t id);
            virtual ~GetUser() = default;
            virtual SQLRETURN bind() override;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() override;

        public:
            void set_identity(uint32_t identity) {
                auto ptr = (Info*)_info.get();
                ptr->identity = identity;
            }
        };
    }
}

