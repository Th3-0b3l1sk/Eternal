#include <iostream> // TODO: proper logger
#include <future>
#include <thread>
#include <vector>
#include "Map/MapManager.h"
#include "Map/MapData.h"
#include "Util/IniFile.h"
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
                maps.pop_back();
                _work_mtx.unlock();

                auto data = std::make_unique<MapData>(work_pair.first); // the map id
                data->load_data(work_pair.second.c_str());
                data->pack();

                std::unique_lock lock(_map_data.first);
                auto& map_data = _map_data.second;
                map_data[work_pair.first] = std::move(data);
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
                        std::bind(&MapManager::load_map_and_pack, this, maps)));

                for (auto& future : work_queue)
                    future.wait();
            }
            catch (std::exception& e)
            {
                std::cout << "Something went horribly wrong! Error code: " << e.what();
            }
        }
    }
}
