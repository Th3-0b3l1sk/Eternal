#include <iostream> // TODO: proper logger
#include <future>
#include <thread>
#include <vector>
#include "Map/MapManager.h"
#include "Util/IniFile.h"
#include "Network/Server.h"
#include "Database/Database.h"
#include <unordered_map>

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

                auto data = std::make_shared<MapData>(id); // the map id
                data->load_data(work_pair.second.c_str());
                data->pack();
                
                std::unique_lock game_map_lock(_game_maps.first);
                auto& game_maps = _game_maps.second;
                if (game_maps.find(id) == game_maps.end())
                    continue;

                game_maps[id]->set_data(data);
            }
        }

        void MapManager::load_maps(const char* gamemap_ini)
        {
            try {
                Util::IniFile ini(gamemap_ini);
                std::vector<std::pair<uint32_t, std::string>> maps;
                for (auto iter = ini.cbegin(); iter != ini.cend(); iter++) {
                    const auto& map_id = std::stoul(iter->first);
                    std::string dmap_path = ini.get(iter->first, "File");
                    maps.push_back({ map_id, dmap_path });
                }

                std::vector<std::future<void>> work_queue;
                const uint32_t hardware_concurrency = std::thread::hardware_concurrency();
                for (auto i{ 0 }; i < hardware_concurrency; i++) 
                    work_queue.push_back(std::async(std::launch::async, 
                        std::bind(&MapManager::load_map_and_pack, this, std::ref(maps))));

                for (auto& future : work_queue)
                    future.wait();
            }
            catch (std::exception& e)
            {
                std::cout << "Something went horribly wrong! Error code: " << e.what();
            }
        }

        void MapManager::load_game_maps(Server& server)
        {
            auto& db = server.get_database();
            auto game_maps = db->get_game_maps();
            if (!game_maps)
                throw std::exception{ "Failed to load game data from the database\n" };

            std::unique_lock lock(_game_maps.first);
            for (auto&& map : game_maps.value()) {
                auto game_map = std::make_unique<GameMap>(std::move(map));
                auto& _maps = _game_maps.second;
                auto _map_id = game_map->get_map_id();
                _maps[_map_id] = std::move(game_map);
            }
        }

        std::unique_ptr<GameMap>& MapManager::get_map(uint32_t map_id)
        {
            std::shared_lock lock(_game_maps.first);
            auto& _maps = _game_maps.second;
            if (_maps.find(map_id) == _maps.end()) {
                std::string error_msg = "Couldn't find the map with the id " + std::to_string(map_id);
                throw std::exception{ error_msg.c_str() };
            }

            return _maps[map_id];
        }
    }
}
