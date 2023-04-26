#include <assert.h>
#include "./Msg/MsgConnectEx.h"


namespace Eternal
{
    // TODO: spread it
    namespace Msg
    {
        MsgConnectEx::MsgConnectEx(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len),
            _info((Info*)this->_buffer.get())
        {
            _info->header.type = MsgType::MSG_CONNECT_EX;
            _info->header.length = sizeof(Info);
        }

        // TODO: fix and make more generic
        MsgConnectEx::MsgConnectEx(RejectionCode code)
            : NetMsg(sizeof(RInfo))
        {
            auto info = (RInfo*)this->_buffer.get();
            this->_size = sizeof(RInfo);
            info->header.type = MsgType::MSG_CONNECT_EX;
            info->header.length = sizeof(RInfo);
            info->rejection_code = code;
            info->rejection = UINT32_C(0);
            info->random_bullshit = UINT32_C(0x050);    // Totally random bullshit
        }

        MsgConnectEx::MsgConnectEx()
            : NetMsg(sizeof(Info))
        {
            _info = (Info*)this->_buffer.get();
            this->_size = sizeof(Info);
            _info->header.type = MsgType::MSG_CONNECT_EX;
            _info->header.length = sizeof(Info);
        }

        MsgConnectEx& MsgConnectEx::set_client_identity(int32_t identity)
        {
            _info->client_identity = identity;
            return *this;
        }

        MsgConnectEx& MsgConnectEx::set_authentication_code(int32_t auth_code)
        {
            _info->authentication_code = auth_code;
            return *this;
        }

        MsgConnectEx& MsgConnectEx::set_game_server_ip(const char* ip)
        {
            //assert(strlen(ip) == UINT8_C(16), "Invalid ip len");
            memcpy_s(_info->game_server_ip, UINT8_C(16), ip, UINT8_C(16));
            return *this;
        }

        MsgConnectEx& MsgConnectEx::set_game_server_port(int32_t port)
        {
            assert(port < UINT16_MAX);
            _info->game_server_port = port;
            return *this;
        }

    }
}
