#include <string>
#include <future>
#include <thread>
#include <vector>
#include "Map/MapManager.h"
#include "Network/Server.h"
#include "Database/Database.h"
#include "Util/IniFile.h"
#include "Util/Logger.h"
#include <unordered_map>

// Global logger
extern std::unique_ptr<Eternal::Util::Logger> GServerLogger;

namespace Eternal
{
    namespace Map
    {
        void MapManager::load_map_and_pack(std::vector < std::pair<uint32_t, std::string>>& maps)
        {
            while (maps.size() != 0) {
                _work_mtx.lock();
                if (maps.size() == 0) {
                    _work_mtx.unlock();
                    return;
                }

                auto work_pair = maps.back();
                auto id = work_pair.first;
                maps.pop_back();
                _work_mtx.unlock();

                auto data = std::make_shared<MapData>(id); 
                /*std::string dbg_msg = "MapManager::load_map_and_pack() => Loading map data for map id [" + std::to_string(id) + "]\n";
                Debug(GServerLogger, dbg_msg.c_str());*/
                if (!data->load_data(work_pair.second.c_str()))
                    continue;
                
                data->pack();
                
                _game_maps.first.lock();
                auto& game_maps = _game_maps.second;
                if (game_maps.find(id) == game_maps.end()) {
                    _game_maps.first.unlock();
                    continue;
                }
                game_maps[id]->set_data(data);
                _game_maps.first.unlock();
            }
        }

        // loads maps info from the DMaps
        bool MapManager::load_maps(const char* gamemap_ini)
        {
            bool err = false;
            std::vector<std::pair<uint32_t, std::string>> maps;

            try {
                Util::IniFile ini(gamemap_ini);
                for (auto iter = ini.cbegin(); iter != ini.cend(); iter++) {
                    const auto& map_id = std::stoul(iter->first);
                    std::string dmap_path = ini.get(iter->first, "File");
                    maps.push_back({ map_id, dmap_path });
                }
            }
            catch (std::exception& e)
            {
                std::string msg = std::string("MapManager::load_maps() => An error has occured.\n\tError: ") + e.what();
                msg += "\nTerminating the server.\n";
                Fatal(GServerLogger, msg.c_str());
                ::terminate();
            }

            std::vector<std::future<void>> work_queue;
            const uint32_t hardware_concurrency = std::thread::hardware_concurrency();
            //const uint32_t hardware_concurrency = 1;
            for (auto i{ 0 }; i < hardware_concurrency; i++)
                work_queue.push_back(std::async(std::launch::async,
                    std::bind(&MapManager::load_map_and_pack, this, std::ref(maps))));

            for (auto& future : work_queue)
                future.wait();

            return !err;
        }

        bool MapManager::load_game_maps(Server& server, const char* gameMap_ini)
        {
            try {
                auto& db = server.get_database();
                auto game_maps = db->get_game_maps();
                if (!game_maps) {
                    Fatal(GServerLogger, "Failed to load game data from the database.");
                    ::terminate();
                }
                
                {
                    std::lock_guard lock(_game_maps.first);
                    for (auto&& map : game_maps.value()) {
                        auto game_map = std::make_unique<GameMap>(std::move(map));
                        auto& _maps = _game_maps.second;
                        auto _map_id = game_map->get_map_id();
                        _maps[_map_id] = std::move(game_map);
                    }
                }

                return this->load_maps(gameMap_ini);
            }
            catch (std::exception& e)
            {
                Error(GServerLogger, "An error has occured.\n\tError: " + std::string(e.what()));
            }
        }

        std::unique_ptr<GameMap>& MapManager::get_map(uint32_t map_id)
        {
            // shouldn't use a lock since game maps won't be written to
            // after initialization
            std::unique_lock lock(_game_maps.first);
            auto& _maps = _game_maps.second;
            if (_maps.find(map_id) == _maps.end()) {
                std::string error_msg = "Can't find the map with ID[ " + std::to_string(map_id) + " ]";
                throw std::exception{ error_msg.c_str() };
            }

            return _maps[map_id];
        }
    }
}
