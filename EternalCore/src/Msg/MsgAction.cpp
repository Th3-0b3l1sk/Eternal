#include "Msg/MsgAction.h"
#include "Msg/MsgItemInfo.h"
#include "Network/Server.h"
#include "Database/Statements/GetUserItems.h"

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
                // TODO: proper logic
                auto msg_action = std::make_shared<Msg::MsgAction>(ActionType::ACTION_SET_LOCATION,
                    0,
                    1000001,
                    1036,
                    198,
                    186,
                    1  );
                server.send(con_id, msg_action);
                break;
            }
            case ActionType::ACTION_SEND_ITEMS:
            {
                auto con = server.get_connetion(con_id);
                auto stmt = std::make_unique<Database::GetUserItems>(con->player_id);
                auto result = server.execute_statement(std::move(stmt));
                for (auto& i : result) {
                    auto msg_item = std::make_shared<Msg::MsgItemInfo>(i.get());
                    server.send(con_id, msg_item);
                }
                break;
            }
            }
        }
    }
}
