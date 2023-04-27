#include "Msg/MsgConnect.h"
#include "Network/Server.h"
#include "Msg/MsgTalk.h"
#include "Msg/MsgData.h"
#include "Msg/MsgUserInfo.h"
#include "Database/Statements/GetUser.h"
#include <iostream> // TODO: remove


namespace Eternal
{
    namespace Msg
    {
        MsgConnect::MsgConnect(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len)
        {
            _info = (Info*)_buffer.get();
        }

        void MsgConnect::process(Server& server, uint32_t con_id)
        {
            std::cout << stringfy() << '\n';
            switch (server._which)
            {
            case Eternal::Server::Which::ACCOUNT:
                server.disconnect(con_id);
                break;
            case Eternal::Server::Which::GAME:
                // process game logic here ... 
            {
                auto statement = std::make_unique<Database::GetUser>(_info->client_identity);  
                auto result = server.execute_statement(std::move(statement));
                if (result.empty()) {
                    // add new character
                    std::cout << "Requesting character creation!\n";
                    std::string sender_name{ "SYSTEM" };
                    std::string recipient_name{ "ALLUSERS" };
                    std::string message{ "NEW_ROLE" };
                    std::string suffix{ "" };
                    auto msg_talk = std::make_shared<MsgTalk>(sender_name, recipient_name, suffix, message, 2101, 0x00FFFFFF);
                    server.send(con_id, msg_talk);
                }
                else {
                    // TODO: use enums
                    auto msg_talk_reply = std::make_shared<Msg::MsgTalk>("SYSTEM", "ALLUSERS", "", "ANSWER_OK", 2101, 0x00FFFFFF);
                    server.send(con_id, msg_talk_reply);

                    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::tm* lt = std::localtime(&time);
                    auto msg_data = std::make_shared<Msg::MsgData>(lt->tm_year, lt->tm_mon, lt->tm_yday, lt->tm_mday, lt->tm_hour,  lt->tm_min, lt->tm_sec);
                    server.send(con_id, msg_data);
                    
                    auto user_data = (Database::GetUser::Info*)result[0].get();
                    auto msg_user_info = std::make_shared<Msg::MsgUserInfo>((char*)user_data->name, (char*)user_data->mate);
                    msg_user_info->init_from_stmt(user_data);
                    server.send(con_id, msg_user_info);

                    auto msg_talk_welcome = std::make_shared<Msg::MsgTalk>("SYSTEM", "ALLUSERS", "", "Welcome to Eternal!", 2006, 0x00FFFFFF);
                    server.send(con_id, msg_talk_welcome);

                    auto msg_talk_welcome2 = std::make_shared<Msg::MsgTalk>("SYSTEM", "ALLUSERS", "", "Enjoy your stay!", 2006, 0x00FFFFFF);
                    server.send(con_id, msg_talk_welcome2);
                }
            }    
            }
        }
    }
}
