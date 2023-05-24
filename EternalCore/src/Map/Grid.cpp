#include "Map/Grid.h"
#include "Map/MapData.h"

namespace Eternal
{
    namespace Map
    {
        Grid::Grid() noexcept
            :
            _width{ 0 }, _height{ 0 }, _is_owner{ false },
            _grid{ nullptr }
        {

        }

        Grid::Grid(uint32_t width, uint32_t height)
            : _width{ width }, _height{ height }, _grid{ nullptr },
            _is_owner{ true }
        {
            _grid_owner.resize(width * height * sizeof(Cell));
            _grid = (grid_t)_grid_owner.data();
        }
        Grid::Grid(uint32_t width, uint32_t height, uint8_t* grid) noexcept
            : _width{ width }, _height{ height }, _grid{ (grid_t)grid },
            _is_owner{ false }
        {
            
        }

        Grid::~Grid()
        {
            _grid = nullptr;
            _grid_owner.resize(0);
            _grid_owner.shrink_to_fit();
        }

        Grid::row_t Grid::_get_row(uint32_t row)
        {
            if (row >= _height)
                return nullptr;

            return (row_t)((uint32_t)_grid + (row * _width * sizeof(Cell)));
        }

        Grid::row_t Grid::_get_col(uint32_t col)
        {
            if (col >= _width)
                return nullptr;

            return (row_t)((uint32_t)_grid + (col * sizeof(Cell)));
        }

        void Grid::_reset_grid(std::vector<uint8_t>&& new_grid)
        {
            _grid_owner = std::move(new_grid);
            _grid = (grid_t)_grid_owner.data();
        }

        size_t Grid::get_grid_size() const
        {
            return _width * _height * sizeof(Cell);
        }

        Cell* Grid::get_cell(uint32_t row, uint32_t col)
        {
            auto cell_row = _get_row(row);
            if (cell_row == nullptr)    
                return nullptr;
            
            return (Cell*)((uint32_t)cell_row + col * sizeof(Cell));
        }

        void Grid::set_cell(const Cell& cell, uint32_t row, uint32_t col)
        {
            if (row >= _height || col >= _width)
                throw std::exception{ "Invalid cell position\n" };

            auto grid_cell = get_cell(row, col);
            if (grid_cell == nullptr)    // will never buuut 
                return;

            *grid_cell = cell;
        }
    }
}
