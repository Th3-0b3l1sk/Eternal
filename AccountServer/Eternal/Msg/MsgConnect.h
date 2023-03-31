#pragma once
#include "./NetMsg.h"

namespace Eternal
{
    class Server;
    namespace Msg
    {
        class MsgConnect : public NetMsg
        {
        private:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint32_t client_identity;
                uint32_t file_contents;
                char file_name[16];
            };
#pragma pack(pop)

        public:
            MsgConnect(std::shared_ptr<uint8_t[]>&& data, size_t len);
            virtual void process(Server&) override;


        };
    }
}


