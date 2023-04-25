#include "./Msg/MsgConnect.h"
#include "./Network/Server.h"
#include <iostream> // TODO: remove
#include "./Msg/MsgTalk.h"

namespace Eternal
{
    namespace Msg
    {
        MsgConnect::MsgConnect(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len)
        {
        }

        void MsgConnect::process(Server& server)
        {
            std::cout << stringfy() << '\n';
            switch (server._which)
            {
            case Eternal::Server::Which::ACCOUNT:
                server._disconnect_last = true;
                break;
            case Eternal::Server::Which::GAME:
                // process game logic here ... 
            {
                std::cout << "Connecting to the game ... \n";
                std::string sender_name{ "SYSTEM" };
                std::string recipient_name{ "ALLUSERS" };
                std::string message{ "NEW_ROLE" };
                std::string suffix{ "" };
                auto msg_talk = std::make_shared<MsgTalk>(sender_name, recipient_name, suffix, message, 2101, 0x00FFFFFF);
                server.queue_msg(msg_talk);
                break;
            }
                
            }
        }
    }
}
