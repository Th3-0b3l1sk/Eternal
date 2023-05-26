#include "Msg/MsgNpcInfo.h"
#include "Util/StringPacker.h"

namespace Eternal
{
    namespace Msg
    {
        MsgNpcInfo::MsgNpcInfo(uint32_t unique_id, uint16_t x, uint16_t y,
            uint16_t look, uint16_t type, uint16_t role,
            std::string_view npc_name)
            : NetMsg(sizeof(Info) + npc_name.size() + 1)
        {
            _info = (Info*)_buffer.get();
            _info->header.type = MsgType::MSG_NPC_INFO;
            _info->header.length = _size;

            _info->unique_id = unique_id;
            _info->x = x;
            _info->y = y;
            _info->look_face = look;
            _info->npc_type = type;
            _info->role_type = role;

            Util::StringPacker packer(_info->name);
            packer.AddString(npc_name.data());
        }


        MsgNpcInfo::MsgNpcInfo(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len)
        {
            _info = (Info*)_buffer.get();
            _info->header.type = MsgType::MSG_NPC_INFO;
            _info->header.length = _size;
        }

        void MsgNpcInfo::process(Server& server, uint32_t con_id)
        {
        }
    }
}
