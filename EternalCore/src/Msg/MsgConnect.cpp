#include "Database/Database.h"
#include "Msg/MsgConnect.h"
#include "Msg/MsgTalk.h"
#include "Msg/MsgData.h"
#include "Msg/MsgUserInfo.h"
#include "Entities/Player.h"
#include "Network/Server.h"
#include "Network/Connection.h"
#include "World.h"
#include "Util/co_defs.h"
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
                auto& db = server.get_database();
                auto player_info = db->get_player_info(_info->client_identity);
                if (player_info && player_info->id == PLAYER_STATS_NON_EXISTING) {
                    // add new character
                    std::cout << "Requesting character creation!\n";
                    auto msg_talk = std::make_shared<MsgTalk>(SYSTEM, ALLUSERS, "", NEW_ROLE, 2101, 0x00FFFFFF);
                    server.send(con_id, msg_talk);
                }
                else if (player_info) {
                    auto& connection = server.get_connection(con_id);
                    // TODO: use enums
                    auto msg_talk_reply = std::make_shared<Msg::MsgTalk>(SYSTEM, ALLUSERS, "", ANSWER_OK, 2101, 0x00FFFFFF);
                    server.send(con_id, msg_talk_reply);

                    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::tm* lt = std::localtime(&time);
                    auto msg_data = std::make_shared<Msg::MsgData>(lt->tm_year, lt->tm_mon, lt->tm_yday, lt->tm_mday, lt->tm_hour,  lt->tm_min, lt->tm_sec);
                    server.send(con_id, msg_data);

                    auto player = std::make_shared<Entities::Player>(*connection, player_info.value());
                    connection->set_player(player);
                    auto& game_world = server.get_world();
                    game_world->join_player(player);
                    
                    auto msg_user_info = std::make_shared<Msg::MsgUserInfo>(player_info.value());
                    server.send(con_id, msg_user_info);

                    auto msg_talk_welcome = std::make_shared<Msg::MsgTalk>(SYSTEM, ALLUSERS, "", ETERNAL_WELCOME, 2006, 0x00FFFFFF);
                    server.send(con_id, msg_talk_welcome);

                    auto msg_talk_welcome2 = std::make_shared<Msg::MsgTalk>(SYSTEM, ALLUSERS, "", ETERNAL_ENJOY, 2006, 0x00FFFFFF);
                    server.send(con_id, msg_talk_welcome2);
                }
            }    
            }
        }
    }
}
