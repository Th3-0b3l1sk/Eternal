#include "./Msg/MsgConnect.h"
#include "./Network/Server.h"
#include <iostream> // TODO: remove

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
                std::cout << "Connecting to the game ... \n";
                break;
            }
            
            // TODO: disconnect this user from the account server          
        }
    }
}