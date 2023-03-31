#include "./MsgAccount.h"
#include "../Network/Encryption/Rc5.h"
#include "../Network/Server.h"
#include "./MsgConnectEx.h"
#include <iostream>

namespace Eternal
{
    namespace Msg
    {
        MsgAccount::MsgAccount(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len),
            _info((Info*)this->_buffer.get())
        { }

        void MsgAccount::process(Server& server)
        {
            // TODO: proper msg account processing
            Encryption::Rc5 rc5;
            rc5.decrypt((uint8_t*)_info->account_pass, get_password_len());
            // TODO: proper logging
            std::cout << stringfy() << '\n';

            // MSG_CONNECT_EX
            std::cout << "responding to the client ...\n";
            auto msg_account_ex = std::make_shared<MsgConnectEx>();
            msg_account_ex->set_client_identity(3).set_authentication_code(281557324).set_game_server_ip("192.168.7.127").set_game_server_port(5816);
            server.queue_msg(std::move(msg_account_ex));
        }
    }
}

