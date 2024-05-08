#pragma once
#include <vector>
#include <string>
#include <istream>
#include <optional>

namespace utility
{
	class line_parser final
	{
	public:
		line_parser() = delete;

		static std::optional<std::vector<std::string>> parse_line(std::istream& input, 
			char args_delimiter=' ', char multi_word_arg_boundary = '"');
	};
}
