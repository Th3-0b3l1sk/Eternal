#include "Msg/MsgItemInfo.h"

namespace Eternal
{
    namespace Msg
    {
        MsgItemInfo::MsgItemInfo(std::shared_ptr<uint8_t[]>&& msg, size_t len)
            : NetMsg(std::move(msg), len)
        {
            _info = (Info*)_buffer.get();
            _info->header.type = MsgType::MSG_ITEM_INFO;
            _info->header.length = sizeof(Info);
        }
        MsgItemInfo::MsgItemInfo(const Database::PlayerOwnItem& item)
            : NetMsg(sizeof(Info))
        {
            _info = (Info*)_buffer.get();
            _info->header.type   = MsgType::MSG_ITEM_INFO;
            _info->header.length = sizeof(Info);

            _info->unique_id      = item.id;
            _info->static_id      = item.type;
            _info->durability     = item.dura;
            _info->max_durability = item.dura_limit;
            _info->action_type    = 1; // TODO: implement action type enum
            _info->location       = item.position;
            _info->unknown1       = 0;
            _info->unknown2       = 0;
            _info->gem1           = item.gem1;
            _info->gem2           = item.gem2;
            _info->reborn_effect  = item.magic1;
            _info->unknown3       = 0;
            _info->plus           = item.magic2;
            _info->bless          = item.bless;
            _info->enchant        = item.enchant;
            _info->unknown4       = 0;
            _info->restrain_type  = item.restrain;
            _info->unknown5       = 0;
            _info->color          = item.color;

        }
        MsgItemInfo::~MsgItemInfo()
        {}

        void MsgItemInfo::process(Server& server, uint32_t con_id)
        {

        }

    }
}

