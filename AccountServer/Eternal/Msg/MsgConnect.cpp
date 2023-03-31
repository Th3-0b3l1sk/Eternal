#include "MsgConnect.h"
#include "../Network/Server.h"
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
            // TODO: disconnect this user from the account server          
        }
    }
}