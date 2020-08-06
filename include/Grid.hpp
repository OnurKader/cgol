#pragma once
#include "RLE_Parser.hpp"
#include "termcolor.hpp"

#include <string>
#include <utility>
#include <vector>

namespace cgol
{
class Grid
{
	std::vector<std::vector<unsigned char>> m_grid;	   // Underlying 2D grid, 1 = alive, 0 = dead
	std::size_t m_rows;								   // Number of rows in the grid
	std::size_t m_cols;								   // Number of cols in the grid
	RLE_Parser m_parser;							   // RLE parser to parse pattern files

public:
	// Construct a Game of Life grid from an .rle pattern file
	// The grid size is used to establish the dimensions of the play area
	explicit Grid(const std::string& rle_filename,
				  const std::pair<std::size_t, std::size_t>& grid_size);

	// Returns the number of rows
	// Used in main.cpp to move cursor up `rows_` times before printing again
	size_t rows() const;

	// Prints the Game of Life grid
	void print(std::ostream& stream = std::cout) const;

	// Updates the Game of Life grid
	void tick();
};

}	 // namespace cgol
