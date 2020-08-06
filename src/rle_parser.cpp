#include "rle_parser.hpp"
using namespace cgol;

void rle_parser::parse_attributes()
{
	const auto lines = split_string(rle_string_, "\n");
	for(const auto& line: lines)
	{
		// Name of the pattern
		if(starts_with(line, "#N"))
		{
			name_ = strip_left(line, "#N ");
		}
		// Comments accompanying the pattern
		else if(starts_with(line, "#C") or starts_with(line, "#c"))
		{
			const auto comment = strip_left(line, "#Cc ");
			comments_.push_back(comment);
		}
		// Authorship of the pattern
		else if(starts_with(line, "#O"))
		{
			author_ = strip_left(line, "#O ");
		}
		// Grid sizes and rules
		else if(starts_with(line, "x"))
		{
			auto data = split_string(line, ",");
			for(auto& d: data)
			{
				trim(d);
				// Grid sizes
				if(starts_with(d, "x"))
				{
					auto kvpair = split_string(d, "=");
					if(size_x_ == 0)
						size_x_ = std::stoi(kvpair[1]);
				}
				else if(starts_with(d, "y"))
				{
					auto kvpair = split_string(d, "=");
					if(size_y_ == 0)
						size_y_ = std::stoi(kvpair[1]);
				}
				// Rules
				else if(starts_with(d, "rule"))
				{
					auto kvpair = split_string(d, "=");
					auto rule = kvpair[1];
					trim(rule);
					for(auto r: split_string(rule, "/"))
					{
						if(starts_with(r, "B"))
						{
							auto digits = strip_left(r, "B");
							for(const auto& digit: digits)
							{
								rule_birth_.push_back(parse_digit_from_char(digit));
							}
						}
						if(starts_with(r, "S"))
						{
							auto digits = strip_left(r, "S");
							for(const auto& digit: digits)
							{
								rule_survival_.push_back(parse_digit_from_char(digit));
							}
						}
					}
				}
			}
		}
		// Pattern line
		else
		{
			pattern_raw_ += strip_right(line, " \n\r\t");
		}
	}
}

void rle_parser::parse_pattern()
{
	auto pattern_rows = split_string(strip_right(pattern_raw_, "!"), "$");
	for(size_t y = 0; y < pattern_rows.size(); y++)
	{
		pattern_2d_array.push_back({});
		std::string tmp_num_str = "";
		for(auto c: pattern_rows[y])
		{
			if(isdigit(c))
			{
				tmp_num_str += c;
			}
			else
			{
				size_t num_cells = 0;
				if(tmp_num_str == "")
				{
					num_cells = 1;
				}
				else
				{
					num_cells = static_cast<size_t>(std::stoi(tmp_num_str));
				}
				for(size_t n = 0; n < num_cells; n++)
				{
					pattern_2d_array[y].push_back(c);
				}
				// Reset count until another number is encountered
				tmp_num_str = "";
			}
		}
		// Fill in empty spaces at end of each row
		for(size_t i = pattern_2d_array[y].size(); i < size_x_; i++)
		{
			pattern_2d_array[y].push_back('b');
		}
	}
	// Fill in empty lines till size_y_
	for(size_t i = pattern_rows.size(); i < size_y_; i++)
	{
		pattern_2d_array.push_back({});
		for(size_t j = 0; j < size_x_; j++)
		{
			pattern_2d_array[i].push_back('b');
		}
	}
}

void rle_parser::open(const std::string& rle_string, std::pair<size_t, size_t> grid_size_override)
{
	rle_string_ = read_file(rle_string);
	size_y_ = grid_size_override.first;
	size_x_ = grid_size_override.second;
	parse_attributes();
	parse_pattern();
}

void rle_parser::print() const
{
	for(auto line: pattern_2d_array)
	{
		for(auto c: line)
		{
			std::cout << c;
		}
		std::cout << "\n";
	}
}

size_t rle_parser::rows() const { return size_y_; }

size_t rle_parser::cols() const { return size_x_; }

std::vector<std::vector<unsigned char>> rle_parser::pattern() const { return pattern_2d_array; }
