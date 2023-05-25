#include "Entities/Player.h"
#include "Util/co_defs.h"
#include "Msg/MsgPlayer.h"
#include "Network/Connection.h"
#include "Network/Server.h"

namespace Eternal
{
    namespace Entities
    {
        Player::Player(Connection& connection, Database::GetUser::Info* data)
            : AdvancedEntity(data->identity), _connection(connection)
        {
            _info.mate       = std::string{ (char*)data->mate, MAX_NAME_LEN };
            _info.hair       = data->hair;
            _info.profession = data->profession;
            _info.rebirth    = data->rebirth;
            _info.exp        = data->exp;
            _info.force      = data->force;
            _info.dexterity  = data->dexterity;
            _info.health     = data->health;
            _info.soul       = data->soul;
            _info.add_points = data->add_points;
            _info.current_mp = data->mana;
            _info.gold       = data->money;
            _info.cps        = data->cps;
            _info.pk_points  = data->pk;
            _info.virtue     = data->virtue;
            _info.energy     = INITIAL_ENERGY;
            _info.xp         = INITIAL_XP;
            _info.prev_map   = data->record_map;
            _info.prev_x     = data->record_x;
            _info.prev_y     = data->record_y;

            // from AdvancedEntity
            _level           = data->level;
            _current_hp      = data->health;
            _max_hp          = 0;    // needs calculations

            // From Entity
            _name            = std::string{ (char*)data->name, MAX_NAME_LEN };
            _id              = data->identity;
            _look            = data->lookface;
            _map             = data->record_map;
            _x               = data->record_x;
            _y               = data->record_y;
            _dir             = 1;   // TODO: replace with an enum
        }
        void Player::add_item(std::unique_ptr<Item>&& item)
        {
        }

        void Player::inform(std::shared_ptr<Entity> entity)
        {
            auto player = (Player*)entity.get();
            auto msg = std::make_shared<Msg::MsgPlayer>(*player);
            _connection.send_over_server(msg);
            
        }

        uint32_t Player::get_con_id() const
        {
            return _connection.get_con_uid();
        }
      
    }
}
