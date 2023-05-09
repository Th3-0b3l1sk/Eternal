#include <chrono>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Msg/MsgAccount.h"
#include "Msg/MsgConnectEx.h"
#include "Network/Encryption/Rc5.h"
#include "Database/Statements/Authenticate.h"
#include "Database/Statements/Register.h"



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

            // Register the user if the username begins with #
            if (_info->account_name[0] == 'r') {
                std::string_view new_account(_info->account_name + 1, strlen(_info->account_name) - 1);
                std::string_view password(_info->account_pass, strlen(_info->account_pass));
                std::string ip = server.get_connection(con_id)->get_ip_address();
                auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto statement = std::make_unique<Eternal::Database::Register>(new_account, password, ip, time);
                auto result = server.execute_statement(std::move(statement));
                // TODO: check on the result;
                std::cout << "responding to the client ...\n";
                auto msg_account_ex = std::make_shared<MsgConnectEx>(MsgConnectEx::RejectionCode::TRY_AGN_LATER);
                server.send_n_kill(con_id, std::move(msg_account_ex), false);
                return;
            }

            auto statement = std::make_unique<Eternal::Database::Authenticate>();
            statement->set_name(_info->account_name);
            auto result = server.execute_statement(std::move(statement));
            
            // TODO: add sanitization checks
            // The account doesn't exist
            if (result.empty()) {
                reject_user(server, con_id, MsgConnectEx::RejectionCode::INVALID_ID_PWD);
                return;
                
            }
            else {
                auto user_info = reinterpret_cast<Eternal::Database::Authenticate::Info*>(result[0].get());
                if (strncmp((char*)user_info->password, _info->account_pass, MAX_PASSWORD_LEN) != 0) {
                    reject_user(server, con_id, MsgConnectEx::RejectionCode::INVALID_ID_PWD);
                    return;
                }

                std::cout << "responding to the client ...\n";
                auto msg_account_ex = std::make_shared<MsgConnectEx>();
                msg_account_ex->set_client_identity(user_info->id + IDENTITY_START)
                               .set_authentication_code(UINT8_C(2))
                               .set_game_server_ip("192.168.7.127")    
                               .set_game_server_port(5816);            // TODO: de-couple
                server.send(con_id, std::move(msg_account_ex));
            }
        }
    }
}
