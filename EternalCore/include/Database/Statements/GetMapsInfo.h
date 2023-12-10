#pragma once
#include "Database/db_helper.h"
#include <optional>
#include <vector>

namespace Eternal
{
    namespace Database
    {
        struct MapInfo
        {
            uint16_t uid;
            std::string name;
            uint16_t doc_id;
            uint32_t type;
            uint32_t owner_id;
            uint32_t weather;
            uint16_t portal_x;
            uint16_t portal_y;
            uint32_t reborn_map;
            int32_t  reborn_portal;
            uint32_t light;
        };


        class GetMapsInfo 
        {

        public:
            GetMapsInfo(SQLHANDLE hCon);
            virtual ~GetMapsInfo() = default;
            SQLRETURN bind() ;
            std::optional<std::vector<MapInfo>> execute();

        private:
            SQLHANDLE _hStmt;
            MapInfo _map_info;
        }; 
    }
}
