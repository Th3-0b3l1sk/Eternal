#pragma once
#include <cstdint>
#include <memory>

namespace Eternal
{
    namespace Map
    {
        struct Cell;
        class Grid
        {
        private:
            typedef  Cell* row_t;
            typedef  row_t* grid_t;
            
        public:
            Grid(uint32_t width, uint32_t height);
            Grid(uint32_t width, uint32_t height, uint8_t* grid);

            ~Grid() {
                _grid = nullptr;
            }
        public:
            size_t get_size() const { return _width * _height; }
            Cell* get_cell(uint32_t row, uint32_t col);

        public:
            void set_cell(const Cell& cell, uint32_t row, uint32_t col);


        public:
            std::unique_ptr<Cell[]>&& let_ptr();

        private:
            row_t _get_row(uint32_t row);
            row_t _get_col(uint32_t col);

        private:
            uint32_t _width;
            uint32_t _height;
            grid_t  _grid;
            bool _is_owner;
            std::unique_ptr<Cell[]> _grid_owner;
        };
    }
}
