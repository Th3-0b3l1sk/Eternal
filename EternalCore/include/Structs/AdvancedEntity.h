#pragma once
#include "./Entity.h"

namespace Eternal
{
    namespace Structs
    {
        class AdvancedEntity : public Entity
        {
        public:
            virtual ~AdvancedEntity() = default;

        protected:
            AdvancedEntity(uint32_t uid)
                : Entity(uid), _level{ 0 },
                _current_hp{ 0 }, _max_hp { 0 }
            { }

        public:
            uint8_t get_level() const { return _level; }
            uint16_t get_current_hp() const { return _current_hp; }
            virtual uint16_t get_max_hp () const { return _max_hp; }

        public:
            bool is_alive() const { return _current_hp > 0; }

        protected:
            uint8_t _level;
            uint16_t _current_hp;
            uint16_t _max_hp;          
        };
    }
}
