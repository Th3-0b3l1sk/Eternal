#pragma once
#include <cstdint>
#include <memory>
#include <vector>

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

        class Grid
        {
            friend class MapData;
        private:
            typedef  Cell* row_t;
            typedef  row_t* grid_t;
            
        public:
            Grid() noexcept;
            Grid(uint32_t width, uint32_t height) ;
            Grid(uint32_t width, uint32_t height, uint8_t* grid) noexcept;

            ~Grid();
        public:
            size_t get_cell_count() const { return _width * _height; }
            size_t get_grid_size() const;

            Cell* get_cell(uint32_t row, uint32_t col);

        public:
            void set_cell(const Cell& cell, uint32_t row, uint32_t col);
            std::unique_ptr<Cell[]>&& let_ptr();

        private:
            uint8_t* _get_raw() { return (uint8_t*)_grid; }


        private:
            row_t _get_row(uint32_t row);
            row_t _get_col(uint32_t col);
            void _reset_grid(std::vector<uint8_t>&& new_grid);

        private:
            uint32_t _width;
            uint32_t _height;
            grid_t  _grid;
            bool _is_owner;
            std::vector<uint8_t> _grid_owner;
        };
    }
}
