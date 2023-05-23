#pragma once
#include <unordered_map>
#include "Util/comms.h"
#include <shared_mutex>

namespace Eternal
{
    namespace Map
    {
        class MapData;
        class MapManager
        {
        public:
            MapManager() {};
            ~MapManager() = default;

        public:
            void load_maps(const char* gamemap_ini);

        private:
            void load_map_and_pack(std::vector < std::pair<uint32_t, std::string>>& maps);

        private:
            guarded_pair < std::shared_mutex, 
                std::unordered_map<uint32_t, std::unique_ptr<MapData>>> _map_data;
            std::mutex _work_mtx;
        };
    }
}
