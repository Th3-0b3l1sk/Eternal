#pragma once
#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include "Structs/Player.h"

namespace Eternal
{
    class Server;
    class World
    {
    private:
        template <typename Mtx, typename Cont>
        using guarded_pair = std::pair<Mtx, Cont>;

        template <typename Data>
        using Ptr = std::shared_ptr<Data>;

    public:
        World(Eternal::Server& game_server);

    public:
        void join_player(std::shared_ptr<Structs::Player> player);
        void kick_player(uint32_t player_id);

    private:
        Eternal::Server& _server;
        guarded_pair<std::shared_mutex, std::unordered_map<uint32_t, Ptr<Structs::Player>>> _world_players;
        bool _is_stopped;
    };
}
