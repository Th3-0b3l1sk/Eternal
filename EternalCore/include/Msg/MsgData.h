#pragma once
#include "Msg/NetMsg.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgData :
            public NetMsg
        {
        private:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint32_t update_type;
                uint32_t year;      // starting from 1900
                uint32_t month;
                uint32_t day_of_year;
                uint32_t day;
                uint32_t hour;
                uint32_t minute;
                uint32_t second;
            };
#pragma pack(pop)

        public:
            MsgData(uint32_t year, uint32_t month, uint32_t day_of_year, uint32_t day,
                uint32_t hour, uint32_t minute, uint32_t second);
            virtual ~MsgData() = default;

        public:
            virtual void process(Server& server, uint32_t con_id);

        private:
            Info* _info;
        };
    }
}

