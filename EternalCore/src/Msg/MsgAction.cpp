#include "Msg/MsgAction.h"
#include "Network/Server.h"

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
            }
        }
    }
}
