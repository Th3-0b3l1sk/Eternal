#include "Msg/MsgAction.h"
#include "Msg/MsgItemInfo.h"
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Database/Database.h"
//#include "Database/Statements/GetUser.h"
#include "Entities/Player.h"
#include "Entities/Item.h"
#include "Entities/ItemManager.h"
#include "Map/MapManager.h"
#include "World.h"
#include <chrono>

namespace  Eternal
{
    namespace Msg
    {
        MsgAction::MsgAction(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len)
        {
            _info = (Info*)_buffer.get();
        }

        MsgAction::MsgAction(ActionType action, uint32_t time_stamp, uint32_t identity,
            uint32_t data, uint16_t x, uint16_t y, uint32_t direction)
            : NetMsg(sizeof(Info))
        {
            _info = (Info*)_buffer.get();
            _info->header.type      = MsgType::MSG_ACTION;
            _info->header.length    = sizeof(Info);
            _info->client_timestamp = time_stamp;
            _info->identity         = identity;
            _info->data             = data;
            _info->x                = x;
            _info->y                = y;
            _info->direction        = direction;
            _info->action_type      = action;
        }

        void MsgAction::process(Server& server, uint32_t con_id)
        {
            switch (_info->action_type)
            {
            case ActionType::ACTION_SET_LOCATION:
            {
                auto& con = server.get_connection(con_id);
                auto& player = con->get_player();

                auto now = std::chrono::high_resolution_clock::now();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                auto msg_action = std::make_shared<Msg::MsgAction>(ActionType::ACTION_SET_LOCATION,
                    milliseconds,
                    player->get_identity(),
                    player->get_map(),
                    player->get_x(),
                    player->get_y(),
                    player->get_dir()); 

                server.send(con_id, msg_action);
                
                // add the player to the map/room
                auto& map_mgr = server.get_world()->get_map_manager();
                auto& map = map_mgr->get_map(player->get_map());
                map->add_player(player);
                
                break;

            }
            case ActionType::ACTION_SEND_ITEMS:
            {
                auto& player = server.get_connection(con_id)->get_player();
                auto& db = server.get_database();
                auto player_items = db->get_player_own_items(player->get_identity());
                // some error
                if (!player_items)
                    return;
                // no items
                if (player_items && player_items->size() == 0)
                    return;

                auto& item_mgr = server.get_world()->get_item_manager();
                for (auto& item : *player_items) {
                    auto msg_item = std::make_shared<Msg::MsgItemInfo>(item);
                    player->add_item(std::move(item));
                    server.send(con_id, msg_item);
                }

                // TODO: should i move it inside the loop?
                player->update_bc_set();
                break;
            }
            case ActionType::ACTION_JUMP:
            {
                auto& player = server.get_connection(con_id)->get_player();
                auto map_id = player->get_map();
                auto& map = server.get_world()->get_map_manager()->get_map(map_id);

                auto old_x = player->get_x();
                auto old_y = player->get_y();
                auto new_x = _info->new_x;
                auto new_y = _info->new_y;
                auto dir = _info->direction;
                auto now = std::chrono::high_resolution_clock::now();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                auto flyback = std::make_shared<Eternal::Msg::MsgAction>(
                    ActionType::ACTION_FLYMAP,
                    milliseconds,
                    player->get_identity(),
                    map_id,
                    old_x,
                    old_y,
                    dir
                );

                if (map->attempt_jump(player, old_x, old_y, new_x, new_y, dir)) {
                    auto jump = flyback;
                    jump->_info->action_type = ActionType::ACTION_JUMP;
                    jump->_info->new_x = new_x;
                    jump->_info->new_y = new_y;
                    map->update_bc_set(player);
                    
                    server.send(con_id, jump);
                    player->update_bc_set(jump);
                    
                    auto& db = server.get_database();
                    if (db) {
                        // TODO: DB-FIX
                        //db->update_player_jump(player->get_id(), new_x, new_y);    // TODO: think of a better impl maybe.
                    }
                }
                else
                    server.send(con_id, flyback);

                break;
            }
            }
        }
    }
}
