#include "line_parser.h"
#include "input_format_error.h"

std::optional<std::vector<std::string>> utility::line_parser::parse_line(std::istream& input,
	const char args_delimiter, const char multi_word_arg_boundary)
{
	input >> std::ws;
	std::string line{};
	if (!std::getline(input, line))
	{
		return {};
	}

	std::vector<std::string> args{};
	auto arg_begin_cursor = line.cbegin();
	size_t current_index = 0;

	while (current_index != line.size())
	{
		if (*arg_begin_cursor == args_delimiter)
		{
			++arg_begin_cursor;
			++current_index;
			continue;
		}

		auto arg_end_cursor = arg_begin_cursor;

		if (*arg_begin_cursor != multi_word_arg_boundary)
		{
			if (const size_t arg_end_index = line.find_first_of(args_delimiter, current_index);
				arg_end_index == std::string_view::npos)
			{
				arg_end_cursor = line.cend();
			}

			else
			{
				arg_end_cursor = line.cbegin() + arg_end_index;
			}
		}

		else
		{
			++arg_begin_cursor;
			++current_index;

			if (const size_t arg_end_index = line.find_first_of(multi_word_arg_boundary, current_index);
				arg_end_index == std::string_view::npos)
			{
				throw common_exceptions::input_format_error{ "line_parser::parse_line: "
												"multi word delimiter is not closed." };
			}
			else
			{
				arg_end_cursor = line.cbegin() + arg_end_index;
			}
		}

		args.emplace_back(arg_begin_cursor, arg_end_cursor);
		current_index = arg_end_cursor - line.cbegin();
		arg_begin_cursor = arg_end_cursor;

		if (arg_begin_cursor != line.cend())
		{
			++arg_begin_cursor;
			++current_index;
		}
	}

	return args;
}
