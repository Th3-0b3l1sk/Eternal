#include "Map/GameMap.h"
#include "Map/MapData.h"

namespace Eternal
{
    namespace Map
    {
        GameMap::GameMap(std::unique_ptr<uint8_t[]>&& data)
            : _entities_count{ 0 }, _data{ nullptr }
        {
            _info.reset((Database::GetMap::Info*)data.release());
        }

        GameMap::GameMap(GameMap&& other) noexcept
        {
            _data = std::move(other._data);
            _info = std::move(other._info);
            _entities.second = std::move(other._entities.second);
            _entities_count = other._entities_count;
        }

        size_t GameMap::get_player_count()
        {
            std::shared_lock lock(_entities.first);
            return _entities.second.size();
        }

        void GameMap::set_data(std::shared_ptr<MapData> data)
        {
            _data = std::move(data);
        }

        void GameMap::add_player(std::shared_ptr<Entities::Entity> entity)
        {
            auto entity_id = entity->get_id();
            
            // TODO: if entity is player ... 
            if (get_player_count() == 0) {
                std::unique_lock<std::shared_mutex> lock(_entities.first);
                _data->unpack();
            }
            
            std::unique_lock<std::shared_mutex> lock(_entities.first);
            auto& entities = _entities.second;
            if(entities.find(entity_id) != entities.end())
                throw std::exception{ "Attempting to add a player to the map failed since the player exists." };

            entities[entity_id] = entity;
            entity->clear_bc_set();
            _update_bc_set(entity);

            //
            // TODO: send map info
            //
        }

        // TODO: find a better place
        static uint32_t _distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
        {
            auto x_diff = std::abs(x2 - x1);
            auto y_diff = std::abs(y2 - y1);

            return std::sqrt(x_diff * x_diff + y_diff * y_diff);
        }


        void GameMap::_update_bc_set(std::shared_ptr<Entities::Entity> entity)
        {
            auto entity_id = entity->get_id();
            auto& entities = _entities.second;
            
            if (entities.find(entity_id) != entities.end()) {
                for (auto& e : entities) {
                    if (e.second->get_id() == entity_id)
                        continue;
                    
                    auto ee = e.second;
                    // TODO: find a better place
                    const uint32_t CELLS_PER_VIEW = 18;
                    if (_distance(entity->get_x(), entity->get_y(), ee->get_x(), ee->get_y()) <= 18) {
                        entity->add_to_bc_set(ee);
                        ee->add_to_bc_set(entity);
                    }
                    else {
                        entity->remove_from_bc_set(ee);
                        ee->remove_from_bc_set(entity);
                    }
                }
            }
        }
    }
}
