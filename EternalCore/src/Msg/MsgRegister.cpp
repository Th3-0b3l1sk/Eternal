#include "Msg/MsgRegister.h"
#include "Msg/MsgTalk.h"
#include "Database/Database.h"
#include "Network/Server.h"

namespace Eternal
{
    namespace Msg
    {
        MsgRegister::MsgRegister(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len)
        {
            _info = (Info*)_buffer.get();
        }

        void MsgRegister::process(Server& server, uint32_t con_id)
        {
            // TODO: handle exceptions
            auto& db = server.get_database();

            // GetAccountInfo to test if the account is already registerd, if not send an error
            auto account_info = db->get_account_info((char*)_info->account);
            if (!account_info) {
                // There is no register account associated with this request. I think should be flagged as a hack
                auto msg_talk = std::make_shared<MsgTalk>(SYSTEM, ALLUSERS, "", INVALID_ACC, 2100, 0x00FFFFFF);
                server.send(con_id, msg_talk);
                return;
            }

            // Make sure there is no player info associated with that account, if not send an error ( using the returned ID from the previous step )
            auto player_info = db->get_player_info(account_info.value().get_player_id() + IDENTITY_START);
            if (player_info.value().identity != PLAYER_STATS_NON_EXISTING) {
                // There is already info associated with that account. I think should be flagged as a hack
                auto msg_talk = std::make_shared<MsgTalk>(SYSTEM, ALLUSERS, "", INVALID_PLR, 2100, 0x00FFFFFF);
                server.send(con_id, msg_talk);
                return;
            }

            // Make sure the user name is available
            auto player_id = db->get_player_identity((char*)_info->name);
            if (-1 != player_id) {
                // the player name already exists
                auto msg_talk = std::make_shared<MsgTalk>(SYSTEM, ALLUSERS, "", INVALID_PLR, 2100, 0x00FFFFFF);
                server.send(con_id, msg_talk);
                return;
            }

            // Register the player
            Database::PlayerInfo info{};
            info.name = (char*)_info->name;
            info.mate = std::string("None");    // TODO: move defaults to a file/config
            auto face = _info->model / 1000 == 1 ? 2 : 1;
            info.lookface = (face * 10000) + _info->model;
            info.hair = 0;
            info.money = 1'000'000;
            info.money_saved = 0;
            info.cps = 100'000;
            info.level = 110;
            info.exp = 0;
            info.force = 10;
            info.dexterity = 10;
            info.health = 400;
            info.soul = 10;
            info.add_points = 10;
            info.life = 400;
            info.mana = 0;
            info.profession = _info->profession;
            info.pk_points = 0;
            info.virtue = 100;
            info.nobility = 0;
            info.rebirth = 0;
            info.syndicate_id = 0;
            info.record_map = 1036;
            info.record_x = 195;
            info.record_y = 186;
            info.last_login = 0;
            if (!db->set_player_info(account_info.value().get_player_id(), info)) {
                // failed handle error
                auto msg_talk = std::make_shared<MsgTalk>(SYSTEM, ALLUSERS, "", "An error has occured!", 2100, 0x00FFFFFF);
                server.send(con_id, msg_talk);
                return;
            }

            auto msg_talk = std::make_shared<MsgTalk>(SYSTEM, ALLUSERS, "", ANSWER_OK, 2100, 0x00FFFFFF);
            server.send(con_id, msg_talk);
            return;
        }   
    }
}


