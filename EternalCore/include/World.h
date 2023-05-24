#pragma once
#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include "Entities/Player.h"
#include "Util/comms.h"

#define TD(x) ;

namespace Eternal
{
    class Server;
    class World
    {
    private:
        template <typename Data>
        using Ptr = std::shared_ptr<Data>;

    public:
        World(Eternal::Server& game_server);

    public:
        void join_player(std::shared_ptr<Entities::Player> player);
        void kick_player(uint32_t player_id);

    private:
        Eternal::Server& _server;
        guarded_pair<std::shared_mutex, std::unordered_map<uint32_t, Ptr<Entities::Player>>> _world_players;
        TD(std::unique_ptr<MapManager> manager);
        bool _is_stopped;
    };
}
