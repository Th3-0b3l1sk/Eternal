#include "Map/MapData.h"
#include "Util/Packer.h"
#include "Util/Logger.h"
#include <Windows.h>
#include <string>

#include <fstream>  // todo remove


// Global logger
extern std::unique_ptr<Eternal::Util::Logger> GServerLogger;

namespace Eternal
{
    namespace Map
    {
        MapData::MapData(uint32_t map_id)
            : _map_id{ map_id }, _grid{ nullptr }
        {

        }

        MapData::MapData(MapData&& other)
            : _map_id{ other._map_id }

        {
            _grid = std::move(other._grid);
            _portals = std::move(other._portals);
        }

        Util::BinaryRW::unique_deleter MapData::_load_file(const char* map_file)
        {           
            std::filesystem::path map_path(map_file);
            auto current_path = std::filesystem::current_path();
            map_path = "../Data" / map_path;
            Util::BinaryRW reader(map_path);
            return reader.let_ptr();
        }

        bool MapData::load_data(const char* map_file)
        {
            // TODO: remove me
            static std::fstream log1002(R"(C:\Dev\1002.txt)", std::ios::out);
            /*
            * TODO: TWEAAAKS
            */
            bool result = false;
            try {

                static std::filesystem::path current_path = std::filesystem::current_path();
                auto dmap_path = "../Data" / std::filesystem::path(map_file);
                //auto dmap_path = std::filesystem::path(R"(C:\Dev\Eternal\Data\map\map\newplain.dmap)");
                auto dmap = DMap();
                if (!dmap.open(dmap_path))
                    return result;

                // get map cells
                auto width  = dmap.get_width();
                auto height = dmap.get_height();
                _grid = std::make_unique<Map::Grid>();
                auto cells = dmap.get_cells();
                if (cells.empty() || (cells.size() != (width * height))) {
                    std::string err = "MapData::load_data() => map<" + std::string(map_file) + "> ExpectedSize(" + std::to_string(width * height) + ")"
                        " Found(" + std::to_string(cells.size()) + ")\n";
                    Error(GServerLogger, err.c_str());
                    return false;
                }

#define xDBG_TC_BRIDGE 
#ifdef DBG_TC_BRIDGE
                if (_map_id == 1002) {
                    int w{}, h{};
                    for (auto& cell : cells) {
                        log1002 << "Cell(" << w << ", " << h << ") -> M " << cell.accessible << " H: " << cell.elevation << " S: " << cell.surface << '\n';
                        w++;
                        if (w % width == 0) {
                            w = 0;
                            h++;
                        }

                    }
                }
#endif

                _grid->reset_grid(std::move(cells), width, height);


                // get map portals
                auto portals = dmap.get_passages();
                for (auto& portal : portals)
                {
                    _portals.insert({ portal.id, portal });
                }

                return true;
            }
            catch (std::exception& what)
            {
                std::string msg = std::string("An exception has occured.\n\tError: ") + what.what();
                Error(GServerLogger, msg.c_str());

                log1002.flush(); // TODO: R
                return result;
            }

            log1002.flush(); // TODO: R
            return !result;
        }

        const PortalInfo& MapData::get_portal(uint32_t portal_id) const
        {
            if (_portals.find(portal_id) != _portals.end())
                return _portals.at(portal_id);
            else
                throw std::exception{ "The portal doesn't exist\n" };
        }

        bool MapData::pack()
        {
            bool err = false;
            if (_grid->is_packed())
                return err;

            // failed to load the map file
            if (nullptr == _grid)
                return err;

            using Util::Packer;
            auto grid_size = _grid->get_byte_count();
            auto grid_data = _grid->get_cell(0, 0);
            auto packed_size = Packer::get_packed_size(grid_size);      // upper limit
            auto packed_grid = std::vector<uint8_t>(packed_size);
            
            auto result = Packer::pack((char*)grid_data, (char*)packed_grid.data(), grid_size, packed_size);
            if (result > 0) {
                packed_grid.resize(result);
                packed_grid.shrink_to_fit();
            }
            else {
                std::string msg = std::string("Failed to pack map data. Error: ") + std::to_string(result);
                Error(GServerLogger, msg.c_str());
                return err;
            }

            _grid->set_packed_info(result, std::move(packed_grid));

            return !err;
        }


        bool MapData::unpack()
        {
            bool err = false;
            if (!_grid->is_packed())
                return err;
            
            using Util::Packer;
            auto unpacked_size = _grid->get_cell_count();
            auto unpacked_grid = std::vector<Map::Cell>(unpacked_size);
            auto result = Packer::unpack((char*)_grid->get_packed_data(), (char*)unpacked_grid.data(), _grid->get_packed_size(), unpacked_size * sizeof(Map::Cell));
            if (result > 0) {
                unpacked_grid.resize(result);
                unpacked_grid.shrink_to_fit();
            }
            else {
                std::string msg = std::string("Failed to unpack map data. Error: ") + std::to_string(result);
                Error(GServerLogger, msg.c_str());
                return err;
            }

            _grid->set_unpack_info(std::move(unpacked_grid));

            return !err;
        }
    }
}
