#include <chrono>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Msg/MsgAccount.h"
#include "Msg/MsgConnectEx.h"
#include "Network/Encryption/Rc5.h"
#include "Database/Database.h"

namespace Eternal
{
    namespace Msg
    {
        static void reject_user(Eternal::Server& server, uint32_t con_id, Eternal::Msg::MsgConnectEx::RejectionCode code)
        {
            std::cout << "responding to the client ...\n";
            auto msg_account_ex = std::make_shared<MsgConnectEx>(MsgConnectEx::RejectionCode::INVALID_ID_PWD);
            server.send_n_kill(con_id, std::move(msg_account_ex), false);
        }


        MsgAccount::MsgAccount(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len),
            _info((Info*)this->_buffer.get())
        { }

        void MsgAccount::process(Server& server, uint32_t con_id)
        {
            Encryption::Rc5 rc5;
            rc5.decrypt((uint8_t*)_info->account_pass, get_password_len());
            // TODO: proper logging
            std::cout << stringfy() << '\n';

            // Register the user if the username begins with r
            if (_info->account_name[0] == 'r') {
                std::string_view new_account(_info->account_name + 1, strlen(_info->account_name) - 1);
                std::string_view password(_info->account_pass, strlen(_info->account_pass));
                std::string_view ip = server.get_connection(con_id)->get_ip_address();
                
                auto& db = server.get_database();
                if (!db->register_user(new_account.data(), password.data(), ip.data())) {
                    // TODO: failed to register the new account
                    return;
                }
                std::cout << "[+] Account " + std::string(_info->account_name + 1) + " was registerd successfully!\n"
                          << "    Disconnecting the client" << std::endl;
                auto msg_account_ex = std::make_shared<MsgConnectEx>(MsgConnectEx::RejectionCode::TRY_AGN_LATER);
                server.send_n_kill(con_id, std::move(msg_account_ex), false);
                return;
                
            }

            auto& db = server.get_database();
            auto user_info = db->get_account_info(_info->account_name);
            // TODO: add sanitization checks
            // The account doesn't exist
            if (!user_info.has_value()) {
                reject_user(server, con_id, MsgConnectEx::RejectionCode::INVALID_ID_PWD);
                return;

            }
            else {
                if (user_info.value()._password.compare({ _info->account_pass , MAX_PASSWORD_LEN}) != 0) {
                    reject_user(server, con_id, MsgConnectEx::RejectionCode::INVALID_ID_PWD);
                    return;
                }

                std::cout << "responding to the client ...\n";
                auto msg_account_ex = std::make_shared<MsgConnectEx>();
                msg_account_ex->set_client_identity(user_info.value()._id + IDENTITY_START)
                    .set_authentication_code(UINT8_C(2))
                    .set_game_server_ip("192.168.7.127")
                    .set_game_server_port(5816);            // TODO: de-couple
                server.send(con_id, std::move(msg_account_ex));
            }       
        }
    }
}
