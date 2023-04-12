#pragma once
#include "Msg/NetMsg.h"
#include "Network/Server.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgTalk :
            public NetMsg
        {
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint32_t argb;
                int16_t tone;
                int16_t style;
                int32_t identity;
                uint32_t sender_mesh;
                uint32_t recipient_mesh;
                uint8_t str_count;
                uint8_t sender_len;
                char sender_name[1];
                uint8_t recipient_len;
                char recipient_name[1];
                uint8_t suffix_len;
                char suffix[1];
                uint8_t message_len;
                char message[1];
            };
#pragma pack(pop)
        public:
            MsgTalk(std::shared_ptr<uint8_t[]>&& msg, size_t len);
            MsgTalk(std::string sender_name, std::string recipient_name, std::string suffix, std::string message, uint32_t tone, uint32_t color);
            virtual ~MsgTalk() = default;

        private:
            void helper(uint32_t tone, uint32_t color);

        public:
            virtual void process(Eternal::Server& server) override;

        private:

        };
    }
}
