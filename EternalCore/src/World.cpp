#include "World.h"
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Util/IniFile.h"
#include "Entities/Player.h"
#include "Entities/ItemManager.h"
#include "Entities/NpcManager.h"
#include "Entities/Npc.h"
#include "Map/MapManager.h"

namespace Eternal
{
    World::World(Eternal::Server& game_server) noexcept
        : _is_stopped{ false }, _server(game_server)
    {
        _map_manager = std::make_unique<Map::MapManager>();
        auto& config = _server.get_config();
        _game_map_ini = config->get("world", "maps");

        _init();
    }

    void World::join_player(std::shared_ptr<Entities::Player> player)
    {
        std::unique_lock guard(_world_players.first);
        auto& players = _world_players.second;
        if (players.find(player->get_id()) == players.end())
            players.insert({ player->get_id(), player });
        else {
            guard.unlock();
            kick_player(player->get_id());
            players.insert({ player->get_id(), player });
        }
    }
    void World::kick_player(uint32_t player_id)
    {
        std::unique_lock guard(_world_players.first);
        auto connection_id = _world_players.second.at(player_id)->get_con_id();
        _world_players.second.erase(player_id);
        _server.disconnect(connection_id); 
    }

    std::unique_ptr<Map::MapManager>& World::get_map_manager()
    {
        return _map_manager;
    }

    std::unique_ptr<Entities::ItemManager>& World::get_item_manager()
    {
        return _item_manager;
    }

    std::unique_ptr<Entities::NpcManager>& World::get_npc_manager()
    {
        return _npc_manager;
    }

    void World::_init()
    {
        auto& db = _server._database;
        _item_manager = std::make_unique<Entities::ItemManager>(*db);
        _npc_manager = std::make_unique<Entities::NpcManager>(*db);

        _map_manager->load_db_maps(_server);
        _map_manager->load_maps(_game_map_ini.c_str());
        
        auto& world_npcs = _npc_manager->expose_container();
        for (auto& iter : world_npcs) {
            auto entity = iter.second;
            auto map_id = entity->get_map();

            try {
                _map_manager->get_map(map_id)->add_npc(entity);
            }
            catch (...) {
                continue;
            }
        }
    }
}
