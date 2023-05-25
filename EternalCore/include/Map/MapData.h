#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Map/Grid.h"
#include "Util/BinaryRW.h"

namespace Eternal
{
    namespace Map
    {
#pragma pack(push, 1)
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
            MapData(MapData&& other);
            MapData() = default;
            ~MapData() = default;

        public:
            /* requires absolute path */
            void load_data(const char* map_file);
            const PortalInfo& get_portal(uint32_t portal_id) const;

        public:
            void pack();
            void unpack();

            // FOR USE IN THE UNIT TESTS 
            const uint8_t* _get_raw_grid() const { return _grid->_get_raw(); }
            const uint32_t _get_grid_size() const { return _grid->get_grid_size(); }

        private:
            static Util::BinaryRW::unique_deleter _load_file(const char* map_file);

        private:
            bool _is_packed;
            uint32_t _packed_size;
            std::uint32_t _map_id;
            std::unique_ptr<Map::Grid> _grid;
            std::unordered_map<uint32_t, PortalInfo> _portals;          
        };
    }
}
