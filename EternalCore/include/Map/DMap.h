#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>
#include "Util/BinaryRW.h"

namespace Eternal
{
    namespace Map
    {
#pragma pack(push, 1)
        struct Cell
        {
            ~Cell() = default;

            Cell() noexcept
                : accessible{}, surface{}, elevation{}
            {

            }

            Cell(const Cell& other) noexcept
            {
                accessible = other.accessible;
                surface = other.surface;
                elevation = other.elevation;
            }

            uint16_t accessible;
            uint16_t surface;
            int16_t  elevation;
        };
#pragma pack(pop)

        struct PortalInfo
        {
            uint32_t x;
            uint32_t y;
            uint32_t id;
        };

        class DMap
        {
        public:
            struct DMapCells
            {
                friend class DMap;
                DMapCells(std::int32_t width, std::int32_t height)
                    : _width{ width }, _height{ height }
                {
                    _cells.reserve(width * height);
                }

                DMapCells() = delete;

                Cell* get_cell(std::int32_t col, std::int32_t row)
                {
                    if (col >= _width || row >= _height)
                        return nullptr;

                    auto idx = row * _width + col;
                    if (idx >= _cells.size())
                        return nullptr;
                    return &_cells[row * _width + col];
                }

            private:
                std::vector<Cell> _cells;
                std::int32_t _width;
                std::int32_t _height;
            };

            struct DMapPassages
            {
                DMapPassages(PortalInfo* ptr, std::int32_t count)
                    : _ptr{ ptr }, _n{ count }
                {

                }

                DMapPassages() = delete;

                PortalInfo* get_passage(std::int32_t idx) {
                    if (idx >= _n)
                        return nullptr;
                    
                    return &_ptr[idx];
                }


            private:
                PortalInfo* _ptr;
                std::int32_t _n;
            };

        public:
            DMap() noexcept;
            DMap(const std::filesystem::path& dmap);
            ~DMap() = default;

        public:
            bool open(const std::filesystem::path& dmap);
            std::vector<Cell> get_cells();
            std::vector<PortalInfo> get_passages();
            std::uint8_t* get_layers();

        private:
            bool _do_parse_terrain(std::filesystem::path terr, DMapCells& cells, std::int32_t pos_x, std::int32_t pos_y);
            void _do_read_header();

        public:
            inline const std::uint32_t get_version()   const { return _version; }
            inline const std::uint32_t get_data()      const { return _data; }
            inline const std::string&  get_puzzle()    const { return _puzzle_file; }
            inline const std::int32_t  get_width()     const { return _width; }
            inline const std::int32_t  get_height()    const { return _height; }
            inline const std::int32_t  get_n_passages() const { return _n_passages; }
            inline const std::int32_t  get_n_layers()   const { return _n_layers; }

        private:
            Util::BinaryRW _dmap;
            std::uint32_t _version;
            std::uint32_t _data;
            std::string _puzzle_file;
            std::int32_t _width;
            std::int32_t _height;
            Cell* _first_cell;
            std::int32_t _n_passages;
            PortalInfo* _first_passage;
            std::int32_t _n_layers;
            std::uint8_t* _first_layer;

        };
    }
}
