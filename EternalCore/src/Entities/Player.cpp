#include "Entities/Player.h"
#include "Util/co_defs.h"
#include "Msg/MsgPlayer.h"
#include "Network/Connection.h"
#include "Network/Server.h"
#include "Entities/Item.h"


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

        void Player::add_item(Item&& item)
        {
            auto my_item = std::make_unique<Item>(item);
            std::shared_lock sinv_lk(_inventory.first);
            auto current_index = _inventory.second.size();

            if (item.get_position() == POS_INVENTORY) {
                if (current_index >= MAX_INVENTORY)
                    return; // TODO: tell the player inventory is full
                
                sinv_lk.unlock();
                std::unique_lock uinv_lk(_inventory.first);
                auto& inventory = _inventory.second;
                inventory[current_index] = std::move(my_item);
            }
            else {
                std::unique_lock ueq_lk(_equipment.first);
                auto& equipments = _equipment.second;
                auto item_pos = (uint8_t)item.get_position();
                if (!equipments[item_pos])
                    equipments[item_pos] = std::move(my_item);
                else
                    ;// TODO: remove the item from the equipments and add it to the inventory
            }
        }

        void Player::inform(std::shared_ptr<Entity> entity)
        {
            auto player = (Player*)entity.get();
            auto msg = std::make_shared<Msg::MsgPlayer>(*player);
            _connection.send_over_server(msg); 
        }

        void Player::send_me(std::shared_ptr<Msg::NetMsg> msg)
        {
            _connection.send_over_server(msg);
        }

        void Player::remove_from_bc_set(std::shared_ptr<Entity> entity)
        {
            std::unique_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            auto it = view_set.find(entity->get_id());
            if (it != view_set.end())
                view_set.erase(it);

            // TODO: check if the entity is a player then send ACTION_LEAVE_MAP (him leaving)
        }

        void Player::clear_bc_set()
        {
            std::unique_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            for (auto entity = view_set.begin(); entity != view_set.end(); ) {
                entity->second->remove_from_bc_set(shared_from_this());
                auto tmp = entity++;
                view_set.erase(tmp);

                // TODO: check if the entity is a player then send ACTION_LEAVE_MAP (me leaving)
            }
        }

        void Player::add_to_bc_set(std::shared_ptr<Entity> entity)
        {
            std::unique_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            if (view_set.find(entity->get_id()) != view_set.end())
                return;

            view_set[entity->get_id()] = entity;

            // TODO: check if player
            entity->inform(shared_from_this());
        }

        void Player::update_bc_set(std::shared_ptr<Msg::NetMsg> msg)
        {
            std::shared_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            // TODO: check if entity is player
            for (const auto& entity : view_set)
                entity.second->send_me(msg);
        }

        void Player::update_bc_set()
        {
            std::shared_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            for (const auto& entity : view_set)
                entity.second->inform(shared_from_this());
        }

        uint32_t Player::get_con_id() const
        {
            return _connection.get_con_uid();
        }

       const Item* Player::get_equipment_by_pos(uint8_t pos)  const
        {
           if (pos >= MAX_EQUIPMENT)
               return nullptr;

           std::shared_lock esh_lk(_equipment.first);
           auto& equipment = _equipment.second;
           if (!equipment[pos])
               return nullptr;

           return equipment[pos].get();
        }     
    }
}
