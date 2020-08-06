#pragma once
#include "Utils.hpp"

#include <iostream>
#include <utility>

namespace cgol
{
class RLE_Parser
{
	std::string m_rle_string;					 // Raw RLE file contents
	std::string m_name;							 // Name of the pattern
	std::vector<std::string> m_comments;		 // List of comments in the file
	std::string m_author;						 // Author of the pattern
	std::size_t m_size_x {0};					 // Number of columns in the pattern
	std::size_t m_size_y {0};					 // Number of rows in the pattern
	std::vector<std::size_t> m_rule_birth;		 // e.g., 3 in "B3"
	std::vector<std::size_t> m_rule_survival;	 // e.g., 23 in "S23"
	std::string m_pattern_raw;					 // Raw pattern, e.g., "25boo85boo$24bo..."
	std::vector<std::vector<unsigned char>>
		pattern_2d_array;	 // 2D vector repr. of the pattern_raw_

	// Parses attributes like name, author, comments, pattern size etc.
	void parse_attributes();

	// Parses the Conway Game of Life RLE pattern, e.g, "25boo85boo$24bo..."
	// into a 2D vector of characters
	void parse_pattern();

	// Prints the parsed pattern - Used only for debugging
	void print() const;

public:
	// Opens the RLE file and parses the pattern
	// The `grid_size_override` argument can be used to override the grid size
	// e.g., if the RLE file has a grid size of 38x11 and the override is 40x15,
	// then 40x15 will be used as the grid size - additional `b` dead cells will
	// be inserted where needed.
	void open(const std::string& rle_string,
			  std::pair<std::size_t, std::size_t> grid_size_override = {0, 0});

	// Returns the number of rows in the pattern
	// This value is either:
	// 1. the value parsed from file
	// 2. or the value provided by the user as override
	size_t rows() const;

	// Returns the number of cols in the pattern
	// Similar to rows()
	size_t cols() const;

	// Returns a 2D vector of the parssed pattern
	// e.g.,
	// [
	//   [baaaab],
	//   [ababab],
	//   [bbbbbb]
	// ]
	std::vector<std::vector<unsigned char>> pattern() const;
};

}	 // namespace cgol
