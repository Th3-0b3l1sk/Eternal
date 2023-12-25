#include "Map/Grid.h"

namespace Eternal
{
    namespace Map
    {
        Grid::Grid() noexcept
            : _is_packed{ false }, _packed_size{ 0 }
        {

        }

        Grid::Grid(uint32_t width, uint32_t height)
            : Grid()
        {
            _grid.resize_grid(width, height);
        }

        Grid::~Grid()
        {
            _grid.reset();
        }

        
        Cell* Grid::get_cell(uint32_t col, uint32_t row)
        {
            if (_is_packed) return nullptr;

            return _grid.get_cell(col, row);
        }

        void Grid::set_cell(const Cell& cell, uint32_t col, uint32_t row)
        {
            if(_is_packed)
                throw std::exception{ "Attempting to set the cells of a packed grid\n" };

            auto grid_cell = get_cell(col, row);
            if (grid_cell == nullptr)    // will never buuut 
                return;

            *grid_cell = cell;
        }
        void Grid::set_packed_info(uint32_t packed_size, std::vector<uint8_t>&& packed_data)
        {
            _grid.reset();

            _is_packed = true;
            _packed_size= packed_size;
            _packed_data = std::move(packed_data);
        }
        void Grid::set_unpack_info(std::vector<Cell>&& packed_data)
        {
            _grid.reset(std::move(packed_data));

            _is_packed = false;
            _packed_size = 0;
            _packed_data.resize(0); _packed_data.clear(); _packed_data.shrink_to_fit();
        }
        void Grid::reset_grid(std::vector<Cell>&& new_data, uint32_t width, uint32_t height)
        {
            set_unpack_info(std::move(new_data));
            _grid.width  = width;
            _grid.height = height;
        }
    }
}
