#include <sstream>
#include <iomanip>
#include <assert.h>
#include "./Msg/NetMsg.h"
#include "./Msg/MsgAccount.h"
#include "./Msg/MsgConnect.h"

namespace Eternal
{
    class Server;
    namespace Msg
    {
        NetMsg::NetMsg(std::shared_ptr<uint8_t[]>&& buffer, size_t size)
            : _buffer(std::move(buffer)), _size(size)
        {}

        NetMsg::NetMsg()
        {
            _buffer.reset(new uint8_t[UINT16_C(1024)]{});
            _size = UINT16_C(1024);
        }

        std::string NetMsg::stringfy()
        {
            assert(_buffer != nullptr);
            // format 
            // [type][len]
            // hex dump .... ascii dump 16 per line 
            std::ostringstream buf;
            std::ostringstream ascii;

            buf << '[' << msg_type_to_string(((Header*)_buffer.get())->type) << "][" << _size << "]\n";
            for (size_t i{}; i < _size; i++) {
                if (i != 0 && i % 15 == 0) {
                    buf << "    " << ascii.str() << '\n';
                    ascii.str("");
                }

                buf << std::setw(2) << std::setfill('0') << std::hex << ((int)_buffer[i] & 0xff )<< ' ';
                if (std::isprint(_buffer[i])) 
                    ascii << _buffer[i];
                else 
                    ascii << ".";

            }

            // for residue bytes < 0x16 limit
            if (auto len = ascii.str().size(); len != 0) {
                size_t len_spaces = 45 - len * 3;   // hacky but it works! 14 * 3(XY ) + 1 
                buf << std::string(len_spaces, ' ') << "    " << ascii.str() << '\n';
            }

            return buf.str();

        }

        void NetMsg::process(Server& server)
        {
            // TODO: implementation
        }
        // static function
        std::shared_ptr<NetMsg> NetMsg::create(std::shared_ptr<uint8_t[]>&& packet, size_t len)
        {
            auto type = ((Header*)packet.get())->type;
            switch (type)
            {
            case MsgType::MSG_ACCOUNT:
                return std::make_shared<MsgAccount>(std::move(packet), len);
            case MsgType::MSG_CONNECT:
                return std::make_shared<MsgConnect>(std::move(packet), len);

            default:
                return std::make_shared<NetMsg>(std::move(packet), len);
            }
        }
    }
}
