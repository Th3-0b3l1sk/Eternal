#include "Msg/MsgUserInfo.h"
#include "Util/StringPacker.h"

namespace Eternal
{
    namespace Msg
    {
        MsgUserInfo::MsgUserInfo(std::string_view char_name, std::string_view spouse_name)
            : NetMsg(sizeof(Info) + char_name.size() + spouse_name.size() - UINT8_C(2)) // 2 * char[] spaceholders
        {
            _info = (Info*)_buffer.get();
            _info->header.type = MsgType::MSG_USER_INFO;
            _info->header.length = _size;

            auto buffer = &_info->string_list_count;
            auto string_packer = Eternal::Util::StringPacker(buffer);
            string_packer.AddString(char_name.data());
            string_packer.AddString(spouse_name.data());
        }

        MsgUserInfo::MsgUserInfo(const Database::PlayerInfo& info)
            : MsgUserInfo(info.name, info.mate)
        {
            _info->identity       = info.identity;
            _info->mesh           = info.lookface;
            _info->hairstyle      = info.hair;
            _info->silver         = info.money;
            _info->cps            = info.cps;
            _info->experience     = info.exp;
            _info->strength       = info.force;
            _info->agility        = info.dexterity;
            _info->vitality       = info.life;
            _info->spirit         = info.soul;
            _info->attributes     = info.add_points;
            _info->health         = info.health;
            _info->mana           = info.mana;
            _info->pk_points      = info.pk_points;
            _info->level          = info.level;
            _info->profession     = info.profession;
            _info->rebirths       = info.rebirth;
            _info->previous_class = 0;  // TODO: why?
            _info->show_name      = true;

        }

        void MsgUserInfo::process(Server& server, uint32_t con_id)
        {
            // TODO: add macro for not implemented functions
        }
    }
}
