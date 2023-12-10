#include "Map/MapData.h"
#include "Util/Packer.h"
#include <Windows.h>
#include <string>

namespace Eternal
{
    namespace Map
    {
        MapData::MapData(uint32_t map_id)
            : _map_id{ map_id }, _is_packed{ false }, _grid{ nullptr }
        , _packed_size{ 0 }
        {

        }

        MapData::MapData(MapData&& other)
            : _map_id{ other._map_id }, _is_packed{ other._is_packed },
            _packed_size{ other._packed_size }

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

        void MapData::load_data(const char* map_file)
        {
            try {
                auto file_in_mem_ptr = _load_file(map_file);
                if (!file_in_mem_ptr)
                    return;

                   /*
             +-------------+------------+-------------------------------------+
             | Offset hex  | Size       |  Purpose                            |
             +-------------+------------+-------------------------------------+
             | 00          | 8 bytes    | Magic Number                        |
             +-------------+------------+-------------------------------------+
             | 08          | 260 bytes  | The puzzle file path                | // irrelevant for the server (contains graphics info/dds)
             +-------------+------------+-------------------------------------+
             | 10C         | 4 bytes    | Map Width                           |
             +-------------+------------+-------------------------------------+
             | 110         | 4 bytes    | Map Height                          |
             +-------------+------------+-------------------------------------+
             | 114         | VARIBLE    | Map Entries                         |
             +-------------+------------+-------------------------------------+
             | 114 + VAR   | uint32_t   | num_of_portals                      |
             +-------------+------------+-------------------------------------+
             | $ + 0x4     | uint32_t[3]| num_of_portals arrays of 3 dwords   |
             +-------------+------------+-------------------------------------+
             | $ + VAR     | uint32_t   | num_of_objects                      |
             +-------------+------------+-------------------------------------+

                  */
                Util::BinaryRW binary_reader(file_in_mem_ptr.get());
                binary_reader.seek_r(sizeof(DMapHeader::file_version));
                binary_reader.seek_r(sizeof(DMapHeader::puzzle));
                auto width  = binary_reader.read<uint32_t>();
                auto height = binary_reader.read<uint32_t>();
                Cell* cells = nullptr;
                _grid = std::make_unique<Map::Grid>(width, height);

                for (uint16_t h{ 0 }; h < height; h++) {
                    uint32_t computed_checksum = {};
                    for (uint16_t w{ 0 }; w < width; w++) {
                        cells = binary_reader.cast<Cell>();
                        _grid->set_cell(*cells, h, w);
                        computed_checksum += (cells->accessible * (cells->surface + h + 1)) + ((cells->elevation + 2) * (w + 1 + cells->surface));
                        binary_reader.seek_r(sizeof(Cell));
                    }

                    auto row_checksum = binary_reader.read<uint32_t>();
                    if (computed_checksum != row_checksum) {
                        std::string err_code = "Invalid row checksum at h: " + std::to_string(h) + " total height: " + std::to_string(height) + "\n";
                        throw std::exception{ err_code.c_str() };
                    }
                }

                auto num_of_portals = binary_reader.read<uint32_t>();
                PortalInfo* portal = nullptr;
                for (uint32_t i{}; i < num_of_portals; i++) {
                    portal = binary_reader.cast<PortalInfo>();
                    _portals[portal->id] = *portal;
                    binary_reader.seek_r(sizeof(PortalInfo));
                }

                auto num_of_objects = binary_reader.read<uint32_t>();
                for (uint32_t i{}; i < num_of_objects; i++) {
                    auto obj_type = binary_reader.read<MapObject>();
                    switch (obj_type)
                    {
                    case MapObject::TERRAIN:
                    {
                        // scene files define new `properities` for the map cells
                        // and can override those defined in the DMap
                        // the scene file consists of small objects in the map like bridges ... 
                        auto scene_file = binary_reader.cast<char[260]>();
                        binary_reader.seek_r(sizeof(*scene_file));
                        auto start_x = binary_reader.read<uint32_t>();
                        auto start_y = binary_reader.read<uint32_t>();
                        try {
                            // TODO: check out this path
                            std::string scene_path = "../Scene/" + std::string(*scene_file);
                            Util::BinaryRW scene_reader(scene_path);

                            /*
                            * u32 amount @$;
                              char ani_file[256] @$;
                              char ani_title[64] @$;
                              s32 pos_offset_x @$;
                              s32 pos_offset_y @$;
                              u32 ani_interval @$;
                              u32 width @$;
                              u32 height @$;
                              s32 thick @$;
                              s32 scene_offset_x @$;
                              s32 scene_offset_y @$;
                              s32 height__ @$;
                            */
                            auto num_of_layers = scene_reader.read<int>();
                            auto ani_file = scene_reader.cast<char[256]>();
                            scene_reader.seek_r(UINT8_C(256));                   // ani_file ... not exactly 8 :D
                            scene_reader.seek_r(UINT8_C(64));                    // ani_title
                            scene_reader.seek_r(sizeof(int32_t));                // pos_offset_x
                            scene_reader.seek_r(sizeof(int32_t));                // pos_offset_y
                            scene_reader.seek_r(sizeof(uint32_t));               // ani_interval
                            auto width = scene_reader.read<uint32_t>();
                            auto height = scene_reader.read<uint32_t>();
                            scene_reader.seek_r(sizeof(int32_t));                // thicc ... :3
                            auto scene_offset_x = scene_reader.read<int32_t>();
                            auto scene_offset_y = scene_reader.read<int32_t>();
                            scene_reader.seek_r(sizeof(int32_t));                // height

                            Cell* cell = nullptr;
                            for (uint32_t h{ 0 }; h < height; h++) {
                                for (uint32_t w{ 0 }; w < width; w++) {
                                    cell = scene_reader.cast<Cell>();
                                    size_t pos_x = ((start_x + scene_offset_x) + w) - width;
                                    size_t pos_y = ((start_y + scene_offset_y) + h) - height;
                                    if (pos_x > UINT16_MAX || pos_y > UINT16_MAX)
                                        throw std::exception{ "Invalid cell position\n" };
                                    auto g_cell = _grid->get_cell(pos_x, pos_y);
                                    g_cell->accessible = cell->accessible;
                                    g_cell->elevation  = cell->elevation;
                                    scene_reader.seek_r(sizeof(Cell));
                                }
                            }
                        }

                        catch (std::exception& e) {
                            // TODO: implement the exception handler
                            OutputDebugString(e.what());
                            //throw;
                        }
                        break;
                    }

                    case MapObject::SOUND:
                    {
                        binary_reader.seek_r(276);
                        break;
                    }
                    case MapObject::COVER:
                    {
                        binary_reader.seek_r(416);
                        break;
                    }
                    case MapObject::EFFECT_3DNEW:
                    {
                        binary_reader.seek_r(96);
                        break;
                    }
                    case MapObject::EFFECT_3D:
                    {
                        binary_reader.seek_r(72);
                        break;
                    }

                    }
                }
            }
            catch (std::exception& what)
            {
                OutputDebugString(what.what());
                //throw;
            }

            return;
        }

        const PortalInfo& MapData::get_portal(uint32_t portal_id) const
        {
            if (_portals.find(portal_id) != _portals.end())
                return _portals.at(portal_id);
            else
                throw std::exception{ "The portal doesn't exist\n" };
        }

        void MapData::pack()
        {
            if (_is_packed)
                return;

            // failed to load the map file
            if (nullptr == _grid)
                return;

            using Util::Packer;
            auto grid_size = _get_grid_size();
            auto packed_size = Packer::get_packed_size(grid_size);
            auto packed_grid = std::vector<uint8_t>(packed_size);
            auto result = Packer::pack((char*)_grid->_get_raw(), (char*)packed_grid.data(), grid_size, packed_size);
            if (result > 0) {
                packed_grid.resize(result);
                packed_grid.shrink_to_fit();
            }
            else
                ;   // TODO: handle error

            _packed_size = result;
            _grid->_reset_grid(std::move(packed_grid));
            _is_packed = true;
        }


        void MapData::unpack()
        {
            if (!_is_packed)
                return;
            
            using Util::Packer;
            auto unpacked_size = _get_grid_size();
            auto unpacked_grid = std::vector<uint8_t>(unpacked_size);
            auto result = Packer::unpack((char*)_grid->_get_raw(), (char*)unpacked_grid.data(), _packed_size, unpacked_size);
            if (result > 0) {
                unpacked_grid.resize(result);
                unpacked_grid.shrink_to_fit();
            }
            else
                ;   // TODO: handle error

            _packed_size = 0;
            _grid->_reset_grid(std::move(unpacked_grid));
            _is_packed = false;
        }
    }
}
