#pragma once
#include "NetMsg.h"
#include "Database/Statements/GetPlayerInfo.h"    // for the info dt to preserve my sanity

namespace Eternal
{
    namespace Msg
    {
        class MsgUserInfo :
            public NetMsg
        {
        private:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint32_t identity;
                uint32_t mesh;
                uint16_t hairstyle;
                uint32_t silver;
                uint32_t cps;
                uint64_t experience;
                uint8_t  pad[20];
                uint16_t strength;
                uint16_t agility;
                uint16_t vitality;
                uint16_t spirit;
                uint16_t attributes;
                uint16_t health;
                uint16_t mana;
                uint16_t pk_points;
                uint8_t level;
                uint8_t profession;
                uint8_t previous_class;
                uint8_t rebirths;
                uint8_t show_name;
                uint8_t string_list_count;
                uint8_t character_name_length;
                uint8_t character_name;
                uint8_t spouse_name_length;
                uint8_t spouse_name;
            };
#pragma pack(pop)
        
        public:
            MsgUserInfo(std::string_view char_name, std::string_view spouse_name);
            MsgUserInfo(const Database::PlayerInfo& data);
            virtual ~MsgUserInfo() = default;
        public:
            virtual void process(Server& server, uint32_t con_id) override;
            
        private:
            Info* _info;
        };
    }
}
