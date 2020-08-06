#pragma once

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace cgol
{
static inline std::string read_file(std::string_view filename)
{
	std::ifstream stream {filename.data()};
	if(!stream)
	{
		throw std::runtime_error("Error: Could not open file to read");
	}
	return std::string {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

static inline std::vector<std::string> split_string(const std::string& str,
													std::string_view delimiter)
{
	std::vector<std::string> strings;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + delimiter.size();
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}

static inline void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) { return !std::isspace(ch); }));
}

static inline void rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) { return !std::isspace(ch); }).base(),
			s.end());
}

static inline void trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

static inline std::string ltrim_copy(std::string s)
{
	ltrim(s);
	return s;
}

static inline std::string rtrim_copy(std::string s)
{
	rtrim(s);
	return s;
}

static inline std::string trim_copy(std::string s)
{
	trim(s);
	return s;
}

static inline std::string strip_left(const std::string& input_string, std::string_view chars)
{
	std::string result = input_string;
	result.erase(result.begin(), std::find_if(result.begin(), result.end(), [&chars](char ch) {
					 return (!std::isspace(ch) && (chars.find(ch) == std::string::npos));
				 }));
	return result;
}

static inline std::string strip_right(const std::string& input_string, std::string_view chars)
{
	std::string result = input_string;
	result.erase(std::find_if(result.rbegin(),
							  result.rend(),
							  [&chars](char ch) {
								  return (!std::isspace(ch) &&
										  (chars.find(ch) == std::string::npos));
							  })
					 .base(),
				 result.end());
	return result;
}

static inline std::size_t parse_digit_from_char(char c) noexcept
{
	return static_cast<std::size_t>(c - '0');
}

}	 // namespace cgol
