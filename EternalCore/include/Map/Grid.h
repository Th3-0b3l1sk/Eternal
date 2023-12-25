#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "Map/DMap.h"

namespace Eternal
{
    namespace Map
    {   
        class Grid
        {
            friend class MapData;
    
            struct CellGrid
            {
                CellGrid() :
                    width{ 0 }, height{ 0 }
                {

                }

                CellGrid(uint16_t w, uint16_t h)
                    : width{ w }, height{ h }
                {
                    grid.resize(width * height);
                }

                void resize_grid(uint16_t w, uint16_t h) {
                    height = h;
                    width = w;
                    grid.resize(width * height);
                }

                void set_grid(std::vector<Cell>&& new_grid)
                {
                    height = width = -1;    // invalid value
                    grid = std::move(new_grid);
                }

                void reset()
                {
                    grid.resize(0); grid.clear(); grid.shrink_to_fit();
                }

                void reset(std::vector<Cell>&& other)
                {
                    grid = std::move(other);
                }

                Cell* get_cell(uint16_t w, uint16_t h)
                {
                    if (w >= width || h >= height)
                        return nullptr;

                    return &grid[w + h * width];
                }

                inline size_t get_cell_count() const { return width * height; }
                inline size_t get_byte_count() const { return width * height * sizeof(Cell); }

                void set_sell(const Cell& c, uint16_t w, uint16_t h)
                {
                    grid[w + h * width] = c;
                }


                std::vector<Cell> grid;
                uint16_t width;
                uint16_t height;
            };

        public:
            Grid() noexcept;
            Grid(uint32_t width, uint32_t height) ;

            ~Grid();
        public:
            inline size_t get_cell_count() const { return _grid.get_cell_count(); }
            inline size_t get_byte_count() const { return _grid.get_byte_count(); }

            Cell* get_cell(uint32_t row, uint32_t col);
            void set_cell(const Cell& cell, uint32_t row, uint32_t col);
            void set_packed_info(uint32_t packed_size, std::vector<uint8_t >&& packed_data);
            void set_unpack_info(std::vector<Cell >&& packed_data);
            void reset_grid(std::vector<Cell>&& new_data, uint32_t width, uint32_t height);
            inline uint8_t* get_packed_data()  { return _packed_data.data(); }
            inline uint32_t get_packed_size() const { return _packed_size; }
            inline bool is_packed() const { return _is_packed; }

        private:
            CellGrid  _grid;
            bool _is_packed;
            uint32_t _packed_size;
            std::vector<uint8_t> _packed_data;
        };
    }
}
