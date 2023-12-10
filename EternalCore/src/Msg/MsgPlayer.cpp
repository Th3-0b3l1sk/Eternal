#include "Msg/MsgPlayer.h"
#include "Util/StringPacker.h"
#include "Util/co_defs.h"
#include "Entities/Item.h"

namespace Eternal
{
    namespace Msg
    {
        MsgPlayer::MsgPlayer(std::shared_ptr<uint8_t[]>&& data, size_t len)
            : NetMsg(std::move(data), len),
            _info((Info*)this->_buffer.get())
        {

        }

        MsgPlayer::MsgPlayer(Entities::Player& player)
            : NetMsg(sizeof(Info) + player.get_name().length() + 1)
        {
            _info = (Info*)this->_buffer.get();
            _info->header.length = _size;
            _info->header.type = MsgType::MSG_PLAYER;

            _info->unique_id = player.get_identity();
            _info->look = player.get_look();
            _info->status = 0;
            _info->synid_rank = 0;

            auto equipment = player.get_equipment_by_pos(POS_GARMENT);
            _info->garment_type = equipment? equipment->get_type() : 0;
            
            equipment = player.get_equipment_by_pos(POS_ARMET);
            _info->armet_type  = equipment? equipment->get_type()  : 0;
            _info->armet_color = equipment? equipment->get_color() : 0;

            equipment = player.get_equipment_by_pos(POS_ARMOR);
            _info->armor_type  = equipment? equipment->get_type()  : 0;
            _info->armor_color = equipment? equipment->get_color() : 0;

            equipment = player.get_equipment_by_pos(POS_RWEAPON);
            _info->rweapon_type = equipment? equipment->get_type() : 0;

            equipment = player.get_equipment_by_pos(POS_LWEAPON);
            _info->lweapon_type= equipment? equipment->get_type()  : 0;

            memset(_info->_padding1, 0x0, sizeof(_info->_padding1));
            memset(_info->_padding2, 0x0, sizeof(_info->_padding2));
            memset(_info->_padding3, 0x0, sizeof(_info->_padding3));
            memset(_info->_padding4, 0x0, sizeof(_info->_padding4));
            
            _info->life = 0;
            _info->level = 0;
            _info->pos_x = player.get_x();
            _info->pos_y = player.get_y();
            _info->hair = player.get_hair();
            _info->dir = player.get_dir();
            _info->pose = player.get_pose();
            _info->metempsychosis = player.get_rebirth();
            _info->player_level = player.get_level();
            _info->nobility_rank = 0;
            _info->nobility_uid = player.get_identity();

            Util::StringPacker packer(_info->strings);
            packer.AddString(player.get_name().data());
        }

        void MsgPlayer::process(Server& server, uint32_t con_id)
        {
            DebugBreak();
        }
    }
}
