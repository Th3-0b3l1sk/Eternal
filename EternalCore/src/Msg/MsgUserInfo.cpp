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

        void MsgUserInfo::process(Server& server, uint32_t con_id)
        {
            // TODO: add macro for not implemented functions
        }

        void MsgUserInfo::init_from_stmt(Database::GetUser::Info* data)
        {
            if (data == nullptr)
                throw std::exception{ "a null pointer was used for the data param!" };

            _info->identity       = data->identity;
            _info->mesh           = data->lookface;
            _info->hairstyle      = data->hair;
            _info->silver         = data->money;
            _info->cps            = data->cps;
            _info->experience     = data->exp;
            _info->strength       = data->force;
            _info->agility        = data->dexterity;
            _info->vitality       = data->life;
            _info->spirit         = data->soul;
            _info->attributes     = data->add_points;
            _info->health         = data->health;
            _info->mana           = data->mana;
            _info->pk_points      = data->pk;
            _info->level          = data->level;
            _info->profession     = data->profession;
            _info->rebirths       = data->rebirth;
            _info->previous_class = 0;  // TODO: why?
            _info->show_name      = true;
        }
    }
}
