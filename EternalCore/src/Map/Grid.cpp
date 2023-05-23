#include "Map/Grid.h"
#include "Map/MapData.h"

namespace Eternal
{
    namespace Map
    {
        Grid::Grid(uint32_t width, uint32_t height)
            : _width{ width }, _height{ height }, _grid{ nullptr },
            _is_owner{ true }, _grid_owner{ nullptr }
        {
            _grid_owner = std::make_unique<Cell[]>(_width * _height);
            _grid = (grid_t)_grid_owner.get();
        }
        Grid::Grid(uint32_t width, uint32_t height, uint8_t* grid)
            : _width{ width }, _height{ height }, _grid{ (grid_t)grid },
            _is_owner{ false }, _grid_owner{ nullptr }
        {
            
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

        std::unique_ptr<Cell[]>&& Grid::let_ptr()
        {
            if (_is_owner) {
                _is_owner = false;
                _grid = nullptr;
                return std::move(_grid_owner);
            }
            
            throw std::exception{ "Attempting to move a ptr that's not owned by the grid\n" };
        }
    }
}
