#pragma once
#include <array>
#include <unordered_map>
#include <memory>
#include "Util/co_defs.h"
#include "Util/comms.h"
#include "Database/Statements/GetPlayerInfo.h"
#include "Entities/AdvancedEntity.h"
#include "Msg/NetMsg.h"

namespace Eternal
{
    class Connection;

    namespace Entities
    {
        class Item;
        class Player : public AdvancedEntity
        {
        private:

            template <typename Data>
            using Ptr = std::unique_ptr<Data>;

        public:
            Player();
            Player(Connection& connection, const Database::PlayerInfo& player_info);
            Player(Connection& connection, Database::PlayerInfo&& player_info);
            virtual ~Player() = default;

        public:
            void add_item(Item&& item);
            virtual void inform(std::shared_ptr<Entity> entity);
            virtual void send_me(std::shared_ptr<Msg::NetMsg> msg);
            virtual void remove_from_bc_set(std::shared_ptr<Entity> entity) override;
            virtual void clear_bc_set() override;
            virtual void add_to_bc_set(std::shared_ptr<Entity> entity) override;
            virtual void update_bc_set() override;
            virtual void update_bc_set(std::shared_ptr<Msg::NetMsg> msg);



        public:
            uint32_t get_con_id() const;
            uint16_t get_hair() const { return _info.hair; }
            uint16_t get_rebirth() const { return _info.rebirth; }
            const Item* get_equipment_by_pos(uint8_t pos) const;


        private:
            Database::PlayerInfo _info;
            Connection& _connection;
            mutable guarded_pair<std::shared_mutex, std::array<Ptr<Item>, MAX_EQUIPMENT>> _equipment;
            mutable guarded_pair<std::shared_mutex, std::unordered_map<uint8_t, Ptr<Item>>> _inventory;
        };
    }
}
