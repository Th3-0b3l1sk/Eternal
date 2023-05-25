#include "Msg/MsgAction.h"
#include "Msg/MsgItemInfo.h"
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Database/Statements/GetUserItems.h"
#include "Database/Statements/GetItemtype.h"
#include "Database/Statements/GetUser.h"
#include "Entities/Player.h"
#include "Entities/Item.h"
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
                auto con = server.get_connection(con_id);
                auto& player = con->get_player();

                auto now = std::chrono::high_resolution_clock::now();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                auto msg_action = std::make_shared<Msg::MsgAction>(ActionType::ACTION_SET_LOCATION,
                    milliseconds,
                    player->get_id(),
                    player->get_map(),
                    player->get_x(),
                    player->get_y(),
                    player->get_dir()); // direction

                server.send(con_id, msg_action);
                
                // add the player to the map
                auto& map_mgr = server.get_world()->get_map_manager();
                auto& map = map_mgr->get_map(player->get_map());
                map->add_player(player);
                
                break;

            }
            case ActionType::ACTION_SEND_ITEMS:
            {
                auto con = server.get_connection(con_id);
                auto stmt = std::make_unique<Database::GetUserItems>(con->get_player_id());
                auto result = server.execute_statement(std::move(stmt));
                std::vector<uint32_t> item_ids;
                for (auto& i : result) {
                    auto msg_item = std::make_shared<Msg::MsgItemInfo>(i.get());
                    item_ids.push_back(msg_item->get_type());
                    server.send(con_id, msg_item);
                }

                // TODO: move to a proper location
                if (item_ids.empty())
                    return;
                auto item_type_stmt = std::make_unique<Database::GetItemtype>(item_ids);
                auto item_result = server.execute_statement(std::move(item_type_stmt));
                auto& player = server.get_connection(con_id)->get_player();
                for (auto& i : item_result) {
                    player->add_item(std::make_unique<Entities::Item>((Database::GetItemtype::Info*)i.get()));
                }
                break;
            }
            }
        }
    }
}
