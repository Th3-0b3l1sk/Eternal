#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Util/BinaryRW.h"

namespace Eternal
{
    namespace Map
    {
#pragma pack(push, 1)
        struct Cell
        {
            Cell()
                : accessible{}, surface{}, elevation{}
            {

            }

            Cell(const Cell& other)
            {
                accessible = other.accessible;
                surface    = other.surface;
                elevation  = other.elevation;
            }

            uint16_t accessible;
            uint16_t surface;
            int16_t  elevation;
        };

        struct DMapHeader
        {
            uint64_t file_version;
            uint8_t  puzzle[260];
            uint32_t map_width;
            uint32_t map_height;
        };

        struct PortalInfo
        {
            uint32_t x;
            uint32_t y;
            uint32_t id;
        };

        enum MapObject : uint32_t
        {
            TERRAIN       = 1,
            SCENE         = 3, 
            COVER         = 4, 
            EFFECT_3D     = 10,
            SOUND         = 15,
            EFFECT_3DNEW  = 19,
        };
#pragma pack(pop)


        class MapData
        {

        public:
            MapData(uint32_t map_id);
            
        public:
            Util::BinaryRW::unique_deleter load_data(const char* map_file);
            const PortalInfo& get_portal(uint32_t portal_id) const;

        private:
            static Util::BinaryRW::unique_deleter _load_file(const char* map_file);

        private:
            // collection of cells / grid
            // is_packed
            std::unordered_map<uint32_t, PortalInfo> _portals;
            std::uint32_t _map_id;
        };
    }
}
