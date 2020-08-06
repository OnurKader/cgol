#include "RLE_Parser.hpp"

using namespace cgol;

void RLE_Parser::parse_attributes()
{
	const auto lines = split_string(m_rle_string, "\n");
	for(auto&& line: lines)
	{
		// Name of the pattern
		if(line.starts_with("#N"))
		{
			m_name = strip_left(line, "#N ");
		}
		// Comments accompanying the pattern
		else if(line.starts_with("#C") || line.starts_with("#c"))
		{
			const auto comment = strip_left(line, "#Cc ");
			m_comments.push_back(comment);
		}
		// Authorship of the pattern
		else if(line.starts_with("#O"))
		{
			m_author = strip_left(line, "#O ");
		}
		// Grid sizes and rules
		else if(line.starts_with("x"))
		{
			auto data = split_string(line, ",");
			for(auto& d: data)
			{
				trim(d);
				// Grid sizes
				if(d.starts_with("x"))
				{
					const auto kvpair = split_string(d, "=");
					if(m_size_x == 0)
						m_size_x = std::stoul(kvpair[1]);
				}
				else if(d.starts_with("y"))
				{
					const auto kvpair = split_string(d, "=");
					if(m_size_y == 0)
						m_size_y = std::stoul(kvpair[1]);
				}
				// Rules
				else if(d.starts_with("rule"))
				{
					const auto kvpair = split_string(d, "=");
					auto rule = kvpair[1];
					trim(rule);
					for(auto r: split_string(rule, "/"))
					{
						if(r.starts_with("B"))
						{
							auto digits = strip_left(r, "B");
							for(const auto& digit: digits)
							{
								m_rule_birth.push_back(parse_digit_from_char(digit));
							}
						}
						if(r.starts_with("S"))
						{
							const auto digits = strip_left(r, "S");
							for(auto digit: digits)
							{
								m_rule_survival.push_back(parse_digit_from_char(digit));
							}
						}
					}
				}
			}
		}
		// Pattern line
		else
		{
			m_pattern_raw += strip_right(line, " \n\r\t");
		}
	}
}

void RLE_Parser::parse_pattern()
{
	auto pattern_rows = split_string(strip_right(m_pattern_raw, "!"), "$");
	for(std::size_t y = 0; y < pattern_rows.size(); y++)
	{
		pattern_2d_array.push_back({});
		std::string tmp_num_str;
		for(auto c: pattern_rows[y])
		{
			if(std::isdigit(c))
			{
				tmp_num_str += c;
			}
			else
			{
				std::size_t num_cells = 0;
				if(tmp_num_str.empty())
				{
					num_cells = 1;
				}
				else
				{
					num_cells = std::stoul(tmp_num_str);
				}
				for(std::size_t n = 0; n < num_cells; n++)
				{
					pattern_2d_array[y].push_back(static_cast<unsigned char>(c));
				}
				// Reset count until another number is encountered
				tmp_num_str.clear();
			}
		}
		// Fill in empty spaces at end of each row
		for(std::size_t i = pattern_2d_array[y].size(); i < m_size_x; i++)
		{
			pattern_2d_array[y].push_back('b');
		}
	}
	// Fill in empty lines till size_y_
	for(std::size_t i = pattern_rows.size(); i < m_size_y; i++)
	{
		pattern_2d_array.push_back({});
		for(std::size_t j = 0; j < m_size_x; j++)
		{
			pattern_2d_array[i].push_back('b');
		}
	}
}

void RLE_Parser::open(const std::string& rle_string,
					  std::pair<std::size_t, std::size_t> grid_size_override)
{
	m_rle_string = read_file(rle_string);
	m_size_y = grid_size_override.first;
	m_size_x = grid_size_override.second;
	parse_attributes();
	parse_pattern();
}

void RLE_Parser::print() const
{
	for(const auto& line: pattern_2d_array)
	{
		for(auto c: line)
		{
			std::cout << c;
		}
		std::cout << '\n';
	}
}

std::size_t RLE_Parser::rows() const { return m_size_y; }

std::size_t RLE_Parser::cols() const { return m_size_x; }

std::vector<std::vector<unsigned char>> RLE_Parser::pattern() const { return pattern_2d_array; }
