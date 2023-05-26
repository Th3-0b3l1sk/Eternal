#pragma once
#include "Util/comms.h"
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include "Database/Statements/GetMap.h"

namespace Eternal
{
    namespace Entities
    {
        class Entity;
        class Item;
        class Player;
        class Npc;
    }

    namespace Map
    {
        class MapData;
        class GameMap
        {
        public:
            GameMap(std::unique_ptr<uint8_t[]>&& data);
            GameMap(GameMap&& other) noexcept;

        public:
            size_t get_map_id() const { return _info->_uid; }
            size_t get_player_count();

        public:
            void set_data(std::shared_ptr<MapData> data);

            void add_player(std::shared_ptr<Entities::Entity> entity);
            void add_npc(std::shared_ptr<Entities::Entity> entity);
            void update_bc_set(std::shared_ptr<Entities::Entity> entity);
            bool attempt_jump(std::shared_ptr<Entities::Entity> entity, uint16_t old_x, uint16_t old_y, uint16_t new_x, uint16_t new_y, uint8_t dir);

        private:
            std::shared_ptr<MapData> _data;
            std::unique_ptr<Database::GetMap::Info> _info;
            guarded_pair<std::shared_mutex, 
                std::unordered_map<uint32_t, std::shared_ptr<Entities::Entity>>> _entities;
            uint32_t _player_count;
            uint32_t _entities_count;
        };
    }
}
