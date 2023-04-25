#pragma once
#include "NetMsg.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgConnectEx : public NetMsg
        {
        public:
            enum class RejectionCode: uint32_t
            {
                INVALID_ID_PWD = 1,
            };
        private:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                int32_t client_identity;
                int32_t authentication_code;
                char game_server_ip[16];
                int32_t game_server_port;

            };
#pragma pack(pop)
        public:
            MsgConnectEx(std::shared_ptr<uint8_t[]>&& data, size_t len);
            MsgConnectEx();

            // TODO: should be made private?
        public:
            MsgConnectEx(RejectionCode code);  // for rejection

        public:
            MsgConnectEx& set_client_identity(int32_t identity);
            MsgConnectEx& set_authentication_code(int32_t auth_code);
            MsgConnectEx& set_game_server_ip(const char* ip);
            MsgConnectEx& set_game_server_port(int32_t port);

        private:
            Info* _info;
        };
    }
}
