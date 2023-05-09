#include "Msg/MsgItemInfo.h"
#include "Database/Statements/GetUserItems.h"

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
        MsgItemInfo::MsgItemInfo(uint8_t* item)
            : NetMsg(sizeof(Info))
        {
            _info = (Info*)_buffer.get();
            _info->header.type   = MsgType::MSG_ITEM_INFO;
            _info->header.length = sizeof(Info);

            auto temp = (Database::GetUserItems::Info*)item;
            _info->unique_id = temp->id;
            _info->static_id = temp->type;
            _info->durability = temp->dura;
            _info->max_durability = temp->dura_limit;
            _info->action_type = 1; // TODO: implement action type enum
            _info->location = temp->position;
            _info->unknown1 = 0;
            _info->unknown2 = 0;
            _info->gem1 = temp->gem1;
            _info->gem2 = temp->gem2;
            _info->reborn_effect = temp->magic1;
            _info->unknown3 = 0;;
            _info->plus = temp->magic2;
            _info->bless = temp->bless;
            _info->enchant = temp->enchant;
            _info->unknown4 = 0;
            _info->restrain_type = temp->restrain;
            _info->unknown5 = 0;
            _info->color = temp->color;

        }
        MsgItemInfo::~MsgItemInfo()
        {}

        void MsgItemInfo::process(Server& server, uint32_t con_id)
        {

        }

    }
}

