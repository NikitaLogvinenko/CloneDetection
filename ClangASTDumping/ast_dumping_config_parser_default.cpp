#include "ast_dumping_config_parser_default.h"
#include "line_parser.h"
#include "input_format_error.h"
#include "parameters_validation.h"
#include <format>

namespace
{
	using common_exceptions::input_format_error;

	const std::function is_directory_predicate =
		[](const std::filesystem::directory_entry& entry)
		{
			return entry.is_directory();
		};

	const std::unordered_set<std::string> sources_extensions{ ".c", ".cpp" };

	const std::function is_source_file_predicate =
		[](const std::filesystem::directory_entry& entry)
		{
			return sources_extensions.contains(entry.path().extension().string());
		};
}

clang_ast_dumping::ast_dumping_config clang_ast_dumping::ast_dumping_config_parser_default::parse(
	std::istream& input)
{
	ast_dumping_config config{};

	while (true)
	{
		auto line_parsing_result = utility::line_parser::parse_line(input);
		if (!line_parsing_result)
		{
			break;
		}

		std::vector<std::string>& parsed_tokens = line_parsing_result.value();
		const std::string& prefix = parsed_tokens.at(0);

		if (!tokens_processing_by_prefix.contains(prefix))
		{
			throw input_format_error(std::format(
				"ast_dumping_config_parser_default::parse: invalid prefix was encountered: {}.", prefix));
		}

		const auto processing = tokens_processing_by_prefix.at(prefix);
		processing(std::move(parsed_tokens), config);
	}

	return config;
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_dumps_dir(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t dumps_dir_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, dumps_dir_token_index + 1, 
		dumps_dir_prefix, "ast_dumping_config_parser_default::process_dumps_dir");

	if (tokens.size() > dumps_dir_token_index + 1)
	{
		throw input_format_error{ "ast_dumping_config_parser_default::process_dumps_dir: "
											  "several dumps directories were provided." };
	}

	config.set_dumps_dir(std::filesystem::absolute(tokens[dumps_dir_token_index]));
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_include_dir(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_dir_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_dir_token_index + 1, 
		include_dir_prefix, "ast_dumping_config_parser_default::process_include_dir");

	for (size_t token_index = first_dir_token_index; token_index < tokens.size(); ++token_index)
	{
		path dir = std::filesystem::absolute(tokens[token_index]);
		utility::throw_if_nonexistent_directory<input_format_error>(dir, "ast_dumping_config_parser_default::process_include_dir");
		config.add_include_dir(std::move(dir));
	}
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_include_dir_recursive(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_dir_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_dir_token_index + 1, 
		include_dir_recursive_prefix, "ast_dumping_config_parser_default::process_include_dir_recursive");

	for (size_t token_index = first_dir_token_index; token_index < tokens.size(); ++token_index)
	{
		path dir = std::filesystem::absolute(tokens[token_index]);
		utility::throw_if_nonexistent_directory<input_format_error>(dir, "ast_dumping_config_parser_default::process_include_dir_recursive");

		auto nested_dirs = get_nested_paths(std::filesystem::recursive_directory_iterator{ dir }, is_directory_predicate);

		config.add_include_dir(std::move(dir));

		for (auto& nested_path : nested_dirs)
		{
			config.add_include_dir(std::move(nested_path));
		}
	}
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_libs_dir(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_dir_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_dir_token_index + 1, 
		libs_dir_prefix, "ast_dumping_config_parser_default::process_libs_dir");

	for (size_t token_index = first_dir_token_index; token_index < tokens.size(); ++token_index)
	{
		path dir = std::filesystem::absolute(tokens[token_index]);
		utility::throw_if_nonexistent_directory<input_format_error>(dir, "ast_dumping_config_parser_default::process_libs_dir");
		config.add_lib_dir(std::move(dir));
	}
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_libs_dir_recursive(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_dir_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_dir_token_index + 1,
		libs_dir_recursive_prefix, "ast_dumping_config_parser_default::process_libs_dir_recursive");

	for (size_t token_index = first_dir_token_index; token_index < tokens.size(); ++token_index)
	{
		path dir = std::filesystem::absolute(tokens[token_index]);
		utility::throw_if_nonexistent_directory<input_format_error>(dir, "ast_dumping_config_parser_default::process_libs_dir_recursive");

		auto nested_dirs = get_nested_paths(std::filesystem::recursive_directory_iterator{ dir }, is_directory_predicate);

		config.add_lib_dir(std::move(dir));

		for (auto& nested_path : nested_dirs)
		{
			config.add_lib_dir(std::move(nested_path));
		}
	}
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_lib(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_lib_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_lib_token_index + 1,
		lib_prefix, "ast_dumping_config_parser_default::process_lib");

	for (size_t token_index = first_lib_token_index; token_index < tokens.size(); ++token_index)
	{
		path lib = std::filesystem::absolute(tokens[token_index]);
		utility::throw_if_nonexistent<input_format_error>(lib, "ast_dumping_config_parser_default::process_lib");
		config.add_lib(std::move(lib));
	}
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_sources_dir(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_dir_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_dir_token_index + 1,
		sources_dir_prefix, "ast_dumping_config_parser_default::process_sources_dir");

	for (size_t token_index = first_dir_token_index; token_index < tokens.size(); ++token_index)
	{
		path dir = std::filesystem::absolute(tokens[token_index]);
		utility::throw_if_nonexistent_directory<input_format_error>(dir, "ast_dumping_config_parser_default::process_sources_dir");

		for (auto nested_sources = get_nested_paths(
			std::filesystem::directory_iterator{ dir }, is_source_file_predicate); 
			auto& nested_source : nested_sources)
		{
			config.add_source(std::move(nested_source), {});
		}
	}
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_sources_dir_recursive(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_dir_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_dir_token_index + 1,
		sources_dir_recursive_prefix, "ast_dumping_config_parser_default::process_sources_dir_recursive");

	for (size_t token_index = first_dir_token_index; token_index < tokens.size(); ++token_index)
	{
		path dir = std::filesystem::absolute(tokens[token_index]);
		utility::throw_if_nonexistent_directory<input_format_error>(dir, "ast_dumping_config_parser_default::process_sources_dir_recursive");

		for (auto nested_sources = get_nested_paths(
			std::filesystem::recursive_directory_iterator{ dir }, is_source_file_predicate);
			auto & nested_source : nested_sources)
		{
			config.add_source(std::move(nested_source), {});
		}
	}
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_common_args(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t first_arg_token_index = prefix_tokens;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, first_arg_token_index + 1,
		common_args_prefix, "ast_dumping_config_parser_default::process_common_args");

	config.add_args({ 
		std::move_iterator{tokens.begin() + first_arg_token_index},
		std::move_iterator{tokens.end()} });
}

void clang_ast_dumping::ast_dumping_config_parser_default::process_source(std::vector<std::string> tokens,
	ast_dumping_config& config)
{
	constexpr size_t prefix_tokens = 1;
	constexpr size_t source_token_index = prefix_tokens;
	constexpr size_t first_arg_token_index = source_token_index + 1;

	utility::throw_if_not_enough_elements<input_format_error>(
		tokens, source_token_index + 1,
		source_prefix, "ast_dumping_config_parser_default::process_source");

	auto source_path = std::filesystem::absolute(tokens[source_token_index]);
	utility::throw_if_nonexistent<input_format_error>(source_path, "ast_dumping_config_parser_default::process_source");

	config.add_source(
		std::move(source_path),
		{std::move_iterator{tokens.begin() + first_arg_token_index}, std::move_iterator{tokens.end()} });
}
