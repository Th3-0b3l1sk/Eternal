#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <shared_mutex>

namespace Eternal
{
    namespace Entities
    {
        class Entity
        {
        protected:
            template <typename Mtx, typename Cont>
            using guarded_pair = std::pair<Mtx, Cont>;

            template <typename Data>
            using Ptr = std::unique_ptr<Data>;

        public:
            virtual ~Entity() = default;
            
        protected:
            Entity(uint32_t uid) 
                :  _id{ uid } 
            { }

        public:
            void set_map(uint32_t map_id) { _map = map_id; }
            void set_pos(uint16_t x, uint16_t y) { _x = x; _y = y; }
            void set_dir(uint8_t dir) { _dir = dir; }

        public:
            uint32_t get_id() const { return _id; }
            uint32_t get_look() const { return _look; }
            uint32_t get_map() const { return _map; }
            uint16_t get_x() const { return _x; }
            uint16_t get_y() const { return _y; }
            uint8_t get_dir() const { return _dir; }
            std::string_view get_name() const { return _name; }

        protected:    
            std::string _name;
            uint32_t _id;
            uint32_t _look;
            uint32_t _map;
            uint16_t _x;
            uint16_t _y;
            uint8_t _dir;

            guarded_pair<std::shared_mutex, std::unordered_map<uint32_t, Ptr<Entity>>> _view_set;
        };
    }
}
