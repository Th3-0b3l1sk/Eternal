#pragma once
#include "Msg/NetMsg.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgNpcInfo :
            public NetMsg
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint32_t unique_id;
                uint16_t x;
                uint16_t y;
                uint16_t look_face;
                uint16_t npc_type;
                uint16_t role_type;
                uint16_t pad;
                uint8_t name[1];
            };
#pragma pack(pop)
        public:
            MsgNpcInfo(uint32_t unique_id, uint16_t x, uint16_t y,
                uint16_t look, uint16_t type, uint16_t role,
                std::string_view npc_name);
            MsgNpcInfo(std::shared_ptr<uint8_t[]>&& data, size_t len);

        public:
            virtual void process(Server& server, uint32_t con_id) override;


        private:
            Info* _info;
        };
    }
}
