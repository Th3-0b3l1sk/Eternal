#include "Map/DMap.h"
#include "Util/BinaryRW.h"
#include "Util/Logger.h"
#include <cassert>

#define MAX_FILE_PATH   260

// Global logger
extern std::unique_ptr<Eternal::Util::Logger> GServerLogger;


namespace Eternal
{
    namespace Map
    {
        DMap::DMap() noexcept
            : _version{}, _data{}, _puzzle_file{ "" }, _width{}, _height{},
            _first_cell{ nullptr }, _first_passage{ nullptr }, _first_layer{nullptr},
            _n_passages{}, _n_layers{}
        {

        }
        
        DMap::DMap(const std::filesystem::path& dmap)
            : DMap()
        {
            if (!_dmap.open(dmap)) {
                std::string err = "DMap::DMap() => Failed to open the DMap file<" + dmap.string() + ">\n";
                throw std::exception{ err.c_str() };
            }

            _do_read_header();
        }

        bool DMap::open(const std::filesystem::path& dmap)
        {
            bool res = false;
            res = _dmap.open(dmap);
            if(res)
                _do_read_header();

            return res;
        }

        std::vector<Cell> DMap::get_cells()
        {
            if (!_first_cell || _width == 0 || _height == 0) {
                std::string err = "DMap::get_cells() => Invalid ptr or empty cell data\n";
                Error(GServerLogger, err.c_str());
                return {};
            }
            
            DMapCells Dcells(_width, _height);
            std::vector<Cell>& cells = Dcells._cells;
            
            Util::BinaryRW idx((uint8_t*)_first_cell);
            for (int r{}; r < _height; r++) {
                uint32_t row_checksum{};
                for (int c{}; c < _width; c++) {
                    auto cell = idx.cast<Cell>();
                    idx.seek_r(sizeof(Cell));

                    cells.push_back(*cell);
                    cells.back().accessible = cell->accessible != 1;     // The mask in DMap is (1) -> inaccessible, (0) -> accessible

                    row_checksum += (cell->accessible * (cell->surface + r + 1)) + ((cell->elevation + 2) * (c + 1 + cell->surface));
                }

                uint32_t tq_checksum = idx.read<std::uint32_t>();
                if (row_checksum != tq_checksum)
                {
                    // i should be using stringstream
                    std::string err = "DMap::get_cells() => Invalid row checksum for row(" + std::to_string(r) + ") Found(" + std::to_string(row_checksum)
                        + ") Expected(" + std::to_string(tq_checksum) + ")\n";
                    Error(GServerLogger, err.c_str());
                    return {};
                }
            }

            // Process DMap Terrain cells
            if (!_first_layer || _n_layers == 0) {
                std::string err = "DMap::get_cells() => Terrain got no layers>";
                Verbose(GServerLogger, err.c_str());
                return cells;
            }
            Util::BinaryRW terr(_first_layer);
            for (int n{}; n < _n_layers; n++) {
                std::uint32_t type = terr.read<std::uint32_t>();
                switch (type)
                {
                case 1: // layer terrain
                {
                    const char* cpath = terr.cast<char>();
                    terr.seek_r(MAX_FILE_PATH);
                    auto path = "../Data/" / std::filesystem::path(std::string(cpath, MAX_FILE_PATH));
                    auto pos_x = terr.read<std::int32_t>();
                    auto pos_y = terr.read<std::int32_t>();
                    auto res = _do_parse_terrain(path.string(), Dcells, pos_x, pos_y);
                    if (!res) 
                        return {};

                }
                break;
                case 4: // layer cover
                {
                    terr.seek_r(416);
                }
                break;
                case 10: // layer 3d_effect
                {
                    terr.seek_r(72);
                }
                break;
                case 15: // layer sound
                {
                    terr.seek_r(276);
                }
                break;
                case 19: // layer 3d_effect_new
                {
                    terr.seek_r(96);
                }
                break;
                default: { // illegal format
                    std::string err = "DMap::get_cells() => Invalid DMap format layer type(" + std::to_string(type) + ")\n";
                    Error(GServerLogger, err.c_str());
                    return {};  
                }
                }
            }
            return std::move(cells);
        }

        std::vector<PortalInfo> DMap::get_passages()
        {
            if (!_first_passage || _n_passages == 0)
                return {};

            std::vector<PortalInfo> passages;
            auto idx = _first_passage;
            for (int n{}; n < _n_passages; n++) {
                passages.push_back(*idx);
                ++idx;
            }

            return passages;
        }

        std::uint8_t* DMap::get_layers()
        {
            return _first_layer;
        }

        bool DMap::_do_parse_terrain(std::filesystem::path t, DMapCells& cells, std::int32_t pos_x, std::int32_t pos_y)
        {
            if (!std::filesystem::exists(t)) {
                std::string err = "DMap::_do_parse_terrain() => File <" + t.string() + "> doesn't exist\n\tCurrentDirectory: " + std::filesystem::current_path().string() + "\n";
                Error(GServerLogger, err.c_str());
                return false;
            }

            Util::BinaryRW terr;
            if (!terr.open(t))
            {
                std::string err = "DMap::_do_parse_terrain() => Failed to open file<" + t.string() + ">\n";
                Error(GServerLogger, err.c_str());
                return false;
            }

            auto n_entries = terr.read<std::int32_t>();
            for (int n{}; n < n_entries; n++) {
                terr.seek_r(256);   // Ani file
                terr.seek_r(64);    // Ani title
                terr.seek_r(4);     // Ani.x
                terr.seek_r(4);     // Ani.y
                terr.seek_r(4);     // Ani.interval
                auto width  = terr.read<std::uint32_t>();
                auto height = terr.read<std::uint32_t>();
                terr.seek_r(4);     // Ani.thick
                auto offset_x = terr.read<std::int32_t>();
                auto offset_y = terr.read<std::int32_t>();
                terr.seek_r(4);     // Ani.data

                for (int r{}; r < height; r++) {
                    for (int c{}; c < width; c++) {
                        /*
                        * Conquer uses 16bit values each for the mask, terrain and elevation in the normal DMap file
                        * HOWEVER, they use 32bits each in the terrain layer, wasted some good hours beacuse of this :)
                        */
                        std::uint32_t mask     = terr.read<std::uint32_t>();
                        std::int32_t terrain   = terr.read<std::int32_t>();
                        std::int32_t elevation = terr.read<std::int32_t>();

                        size_t dmap_x = (pos_x + offset_x + c) - width;
                        size_t dmap_y = (pos_y + offset_y + r) - height;

                        /*if (dmap_x == 633 && dmap_y == 677) {
                            DebugBreak();
                        }*/

                        auto dmap_cell = cells.get_cell(dmap_x, dmap_y);
                        if (nullptr == dmap_cell) 
                        {
                            std::string err = "DMap::_do_parse_terrain() => got an invalid cell at(" + std::to_string(dmap_x) + ", " + std::to_string(dmap_y) + ")\n";
                            Error(GServerLogger, err.c_str());
                            return false;
                        }
                        
                        dmap_cell->accessible = mask != 1;
                        dmap_cell->surface = terrain;
                        dmap_cell->elevation = elevation;
                    }
                }
            }           
            return true;
        }

        void DMap::_do_read_header()
        {
            _version = _dmap.read<std::uint32_t>();
            _data = _dmap.read<std::uint32_t>();
            const char* puzzle_file = _dmap.cast<char>();
            _puzzle_file = std::string(puzzle_file, MAX_FILE_PATH);
            _dmap.seek_r(MAX_FILE_PATH);
            _width = _dmap.read<std::int32_t>();
            _height = _dmap.read<std::int32_t>();
            _first_cell = _dmap.cast<Cell>();

            // skip cells + checksums (each row has a check sum of 4 bytes)
            _dmap.seek_r(_width * _height * sizeof(Cell));  // skip cells
            _dmap.seek_r(_height * sizeof(std::uint32_t));  // skip checksums

            _n_passages = _dmap.read<std::int32_t>();
            _first_passage = _dmap.cast<PortalInfo>();
            
            // skip passage info
            _dmap.seek_r(_n_passages * sizeof(PortalInfo));  
            _n_layers = _dmap.read<std::int32_t>();
            _first_layer = _dmap.cast<std::uint8_t>();
        }
    }
}
