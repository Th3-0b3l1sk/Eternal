#pragma once
#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include "Util/comms.h"



#define TD(x) ;

namespace Eternal
{
  
    namespace Map
    {
        class MapManager;
    }

    namespace Entities
    {
        class ItemManager;
        class NpcManager;
        class Player;
        class Npc;
    }

    class Server;
    class World
    {
    private:
        template <typename Data>
        using Ptr = std::shared_ptr<Data>;

    public:
        World(Eternal::Server& game_server) noexcept;
        ~World();
    public:
        void join_player(std::shared_ptr<Entities::Player> player);
        void kick_player(uint32_t player_id);

    public:
        std::unique_ptr<Map::MapManager>& get_map_manager();
        std::unique_ptr<Entities::ItemManager>& get_item_manager();
        std::unique_ptr<Entities::NpcManager>& get_npc_manager();

    private:
        void _init();

    private:
        Eternal::Server& _server;
        guarded_pair<std::shared_mutex, std::unordered_map<uint32_t, Ptr<Entities::Player>>> _world_players;
        std::unique_ptr<Entities::ItemManager> _item_manager;
        std::unique_ptr<Entities::NpcManager> _npc_manager;
        std::unique_ptr<Map::MapManager> _map_manager;
        bool _is_stopped;
        std::string _game_map_ini;
    };
}
