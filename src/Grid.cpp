#include "Grid.hpp"

using namespace cgol;

Grid::Grid(const std::string& rle_filename, const std::pair<size_t, size_t>& grid_size) :
	m_rows {grid_size.first}, m_cols {grid_size.second}, m_parser {}
{
	m_parser.open(rle_filename, {m_rows, m_cols});
	m_rows = m_parser.rows();
	m_cols = m_parser.cols();
	const auto pattern = m_parser.pattern();

	for(size_t i = 0; i < m_rows; i++)
	{
		m_grid.push_back({});
		for(size_t j = 0; j < m_cols; j++)
		{
			if(pattern[i][j] == 'o')
			{
				m_grid[i].push_back(1);
			}
			else if(pattern[i][j] == 'b')
			{
				m_grid[i].push_back(0);
			}
		}
	}
}

size_t Grid::rows() const { return m_rows; }

void Grid::print(std::ostream& os) const
{
	os << termcolor::bold;
	for(size_t i = 0; i < m_rows; i++)
	{
		for(size_t j = 0; j < m_cols; j++)
		{
			const auto cell = m_grid[i][j];
			if(cell == 1)
			{
				os << "██";
			}
			else
			{
				os << "  ";
			}
		}
		os << "\n";
	}
	os << termcolor::reset;
}

void Grid::tick()
{
	auto result = m_grid;

	const int rows = static_cast<int>(m_rows);
	const int cols = static_cast<int>(m_cols);

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			std::size_t neighbours = 0;

			if(j - 1 >= 0)
			{
				neighbours += m_grid[i][j - 1];
				if(i - 1 >= 0)
				{
					neighbours += m_grid[i - 1][j - 1];
				}
				if(i + 1 < rows)
				{
					neighbours += m_grid[i + 1][j - 1];
				}
			}

			if(j + 1 < cols)
			{
				neighbours += m_grid[i][j + 1];
				if((i - 1) >= 0)
				{
					neighbours += m_grid[i - 1][j + 1];
				}
				if((i + 1) < rows)
				{
					neighbours += m_grid[i + 1][j + 1];
				}
			}

			if(i - 1 >= 0)
			{
				neighbours += m_grid[i - 1][j];
			}

			if(i + 1 < rows)
			{
				neighbours += m_grid[i + 1][j];
			}

			if(m_grid[i][j] == 1)
			{
				// Cell is alive
				if(neighbours < 2 or neighbours > 3)
				{
					// Any live cell with fewer than two live neighbours dies, as if by
					// underpopulation. Any live cell with more than three live
					// neighbours dies, as if by overpopulation.
					result[i][j] = 0;
				}
				else
				{
					// Any live cell with two or three live neighbours lives on to the
					// next generation.
				}
			}
			else
			{
				// Cell is dead
				if(neighbours == 3)
				{
					// Any dead cell with exactly three live neighbours becomes a live
					// cell, as if by reproduction.
					result[i][j] = 1;
				}
			}
		}
	}
	m_grid = result;
}
