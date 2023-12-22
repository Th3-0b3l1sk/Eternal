#pragma once
#include <unordered_map>
#include "Util/comms.h"
#include "Map/MapData.h"
#include "Map/GameMap.h"
#include <mutex>

namespace Eternal
{
    class Server;
    namespace Map
    {
        class MapManager
        {
        public:
            MapManager() {};
            ~MapManager() = default;

        public:
            bool load_game_maps(Server& server, const char* gamemap_ini);
            std::unique_ptr<GameMap>& get_map(uint32_t map_id);

        private:
            bool load_maps(const char* gamemap_ini);
            void load_map_and_pack(std::vector < std::pair<uint32_t, std::string>>& maps);

        private:
            guarded_pair < std::mutex,
                std::unordered_map<uint32_t, std::shared_ptr<MapData>>> _map_data;
            guarded_pair < std::mutex,
                std::unordered_map<uint32_t, std::unique_ptr<GameMap>>> _game_maps;
            std::mutex _work_mtx;
        };
    }
}
