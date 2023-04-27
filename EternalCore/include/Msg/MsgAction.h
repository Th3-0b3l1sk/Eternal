#pragma once
#include "Msg/NetMsg.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgAction :
            public NetMsg
        {
        public:
            enum class ActionType: uint16_t
            {
                ACTION_SET_LOCATION = 74,

            };

        private:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint32_t   client_timestamp;
                uint32_t   identity;
                uint32_t   data;
                uint16_t   x;
                uint16_t   y;
                uint16_t   direction;
                ActionType action_type;
            };
#pragma pack(pop)

        public:
            MsgAction(std::shared_ptr<uint8_t[]>&& data, size_t len);
            MsgAction(ActionType action, uint32_t time_stamp, uint32_t identity, 
                uint32_t data1, uint16_t x, uint16_t y, uint32_t direction);
            virtual ~MsgAction() = default;

        public:
            virtual void process(Server& server, uint32_t con_id) override;

        private:
            Info* _info;
        };
    }
}
