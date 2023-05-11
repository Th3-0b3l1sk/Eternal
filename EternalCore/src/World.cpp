#include "World.h"
#include "Network/Server.h"
#include "Network/Connection.h"

namespace Eternal
{
    World::World(Eternal::Server& game_server)
        : _is_stopped{ false }, _server(game_server)
    {

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
}
