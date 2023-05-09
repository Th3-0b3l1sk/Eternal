#pragma once
#include "NetMsg.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgItemInfo :
            public NetMsg
        {
        private:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                std::uint32_t unique_id;
                std::uint32_t static_id;
                std::uint16_t durability;
                std::uint16_t max_durability;
                std::uint16_t action_type;
                std::uint8_t location;
                std::uint8_t unknown1;
                std::uint32_t unknown2;
                std::uint8_t gem1;
                std::uint8_t gem2;
                std::uint8_t reborn_effect;
                std::uint8_t unknown3;
                std::uint8_t plus;
                std::uint8_t bless;
                std::uint8_t enchant;
                std::uint8_t unknown4;
                std::uint32_t restrain_type;
                std::uint32_t unknown5;
                std::uint32_t color;
            };
#pragma pack(pop)

        public:
            MsgItemInfo(std::shared_ptr<uint8_t[]>&& msg, size_t len);
            MsgItemInfo(uint8_t* item);
            virtual ~MsgItemInfo();

            virtual void process(Server& server, uint32_t con_id) override;

            inline uint8_t get_location() const { return _info->location; }
            inline uint32_t get_type() const { return _info->static_id; }


        private:
            Info* _info;
        };
    }
}
