#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include "Util/comms.h"

namespace Eternal
{
    namespace Msg
    {
        class NetMsg;
    }

    namespace Entities
    {
        class Entity: public std::enable_shared_from_this<Entity>
        {
        public:
            virtual ~Entity() = default;
            
        protected:
            Entity(uint32_t uid) 
                :  _identity{ uid } 
            { }

            Entity() = default;

        public:
            void set_map(uint32_t map_id) { _map = map_id; }
            void set_pos(uint16_t x, uint16_t y) { _x = x; _y = y; }
            void set_dir(uint8_t dir) { _dir = dir; }

        public:
            uint32_t get_identity() const { return _identity; }
            uint32_t get_look() const { return _look; }
            uint32_t get_map() const { return _map; }
            uint16_t get_x() const { return _x; }
            uint16_t get_y() const { return _y; }
            uint8_t get_dir() const { return _dir; }
            std::string_view get_name() const { return _name; }

            virtual void clear_bc_set() 
            {};
            virtual void remove_from_bc_set(std::shared_ptr<Entity> entity) 
            {};
            virtual void add_to_bc_set(std::shared_ptr<Entity> entity) 
            {};
            virtual void send_me(std::shared_ptr<Msg::NetMsg> msg)
            {};
            virtual void update_bc_set() 
            {};
            virtual void update_bc_set(std::shared_ptr<Msg::NetMsg> msg)
            {};
            virtual void inform(std::shared_ptr<Entity> entity)
            {};

        protected:    
            std::string _name{ "UNKOWN" };
            uint32_t _identity{};
            uint32_t _look{};
            uint32_t _map{};
            uint16_t _x{};
            uint16_t _y{};
            uint8_t _dir{};

            guarded_pair<std::shared_mutex, 
                std::unordered_map<uint32_t, std::shared_ptr<Entity>>> _view_set;
        };
    }
}
