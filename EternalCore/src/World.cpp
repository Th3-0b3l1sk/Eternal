#include "World.h"
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Util/IniFile.h"
#include "Entities/Player.h"
#include "Entities/ItemManager.h"

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

    std::unique_ptr<Entities::ItemManager>& World::get_item_manager()
    {
        return _item_manager;
    }

    void World::_init()
    {
        // TODO: config
        _map_manager->load_db_maps(_server);
        _map_manager->load_maps(_game_map_ini.c_str());
        auto& db = _server._database;
        _item_manager = std::make_unique<Entities::ItemManager>(*db);
    }
}
