﻿#include "cmcd_config_parser_default.h"
#include "line_parser.h"
#include "input_format_error.h"
#include "parameters_validation.h"
#include "nested_paths_provider.h"
#include <format>
#include <unordered_set>

namespace
{
	using common_exceptions::input_format_error;

	using nested_paths_provider = utility::nested_paths_provider;
	const auto& is_directory_predicate = nested_paths_provider::is_directory_predicate;

	constexpr size_t tokens_per_line = 2;
	constexpr size_t param_name_token = 0;
	constexpr size_t param_value_token = 1;
}

code_clones_analysis_top_level::cmcd_config code_clones_analysis_top_level::
cmcd_config_parser_default::parse(std::istream& input) const
{
	cmcd_config config{};

	while (true)
	{
		auto line_parsing_result = utility::line_parser::parse_line(input);
		if (!line_parsing_result)
		{
			break;
		}

		std::vector<std::string>& parsed_tokens = line_parsing_result.value();

		if (parsed_tokens.size() != tokens_per_line)
		{
			throw input_format_error{ std::format("cmcd_config_parser_default::parse: {} tokens expected, but {} passed.",
				tokens_per_line, parsed_tokens.size()) };
		}

		const std::string& param_name = parsed_tokens.at(param_name_token);

		std::string param_value = parsed_tokens.at(param_value_token);
		if (!arg_processing_by_prefix.contains(param_name))
		{
			process_additional_argument(param_name, param_value, config);
		}
		else
		{
			const auto processing = arg_processing_by_prefix.at(param_name);
			processing(std::move( param_value), config);
		}
	}

	return config;
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_output(std::string token,
	cmcd_config& config)
{
	config.set_results_output(std::move(token));
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_framework(std::string token,
	cmcd_config& config)
{
	config.set_framework(std::move(token));
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_first_project_dir(std::string token,
	cmcd_config& config)
{
	std::filesystem::path dir = std::filesystem::absolute(token);
	utility::throw_if_nonexistent_directory<input_format_error>(dir, "cmcd_config_parser_default::process_first_project_dir");
	config.add_first_project_dir(std::move(dir));
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_second_project_dir(std::string token,
	cmcd_config& config)
{
	std::filesystem::path dir = std::filesystem::absolute(token);
	utility::throw_if_nonexistent_directory<input_format_error>(dir, "cmcd_config_parser_default::process_second_project_dir");
	config.add_second_project_dir(std::move(dir));
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_first_project_dir_recursive(std::string token,
	cmcd_config& config)
{
	std::filesystem::path dir = std::filesystem::absolute(token);
	utility::throw_if_nonexistent_directory<input_format_error>(dir, "cmcd_config_parser_default::process_first_project_dir_recursive");


	auto nested_dirs = nested_paths_provider::get_nested_paths(std::filesystem::recursive_directory_iterator{ dir }, is_directory_predicate);

	config.add_first_project_dir(std::move(dir));

	for (auto& nested_path : nested_dirs)
	{
		config.add_first_project_dir(std::move(nested_path));
	}
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_second_project_dir_recursive(std::string token,
	cmcd_config& config)
{
	std::filesystem::path dir = std::filesystem::absolute(token);
	utility::throw_if_nonexistent_directory<input_format_error>(dir, "cmcd_config_parser_default::process_second_project_dir_recursive");


	auto nested_dirs = nested_paths_provider::get_nested_paths(std::filesystem::recursive_directory_iterator{ dir }, is_directory_predicate);

	config.add_second_project_dir(std::move(dir));

	for (auto& nested_path : nested_dirs)
	{
		config.add_second_project_dir(std::move(nested_path));
	}
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_excluded_dir(std::string token,
	cmcd_config& config)
{
	std::filesystem::path dir = std::filesystem::absolute(token);
	utility::throw_if_nonexistent_directory<input_format_error>(dir, "cmcd_config_parser_default::process_excluded_dir");
	config.add_excluded_dir(std::move(dir));
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_excluded_source(std::string token,
	cmcd_config& config)
{
	std::filesystem::path source = std::filesystem::absolute(token);
	utility::throw_if_nonexistent<input_format_error>(source, "cmcd_config_parser_default::process_excluded_source");
	config.add_excluded_source(std::move(source));
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_min_similarity(std::string token,
                                                                                        cmcd_config& config)
{
	const double min_similarity = utility::convert_argument<double, input_format_error>(token,
		std::format("cmcd_config_parser_default::process_min_similarity: "
			"can not convert {} to double.", token));
	config.set_min_similarity(min_similarity);
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_min_variables(std::string token,
	cmcd_config& config)
{
	const size_t min_variables = utility::convert_argument<size_t, input_format_error>(token,
		std::format("cmcd_config_parser_default::process_min_variables: "
			"can not convert {} to size_t.", token));
	config.set_min_variables(min_variables);
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_code_analysis_threads(std::string token,
	cmcd_config& config)
{
	const size_t code_analysis_threads = utility::convert_argument<size_t, input_format_error>(token,
		std::format("cmcd_config_parser_default::process_code_analysis_threads: "
			"can not convert {} to size_t.", token));
	config.set_code_analysis_threads(code_analysis_threads);
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_count_matrices_comparing_threads(std::string token, 
	cmcd_config& config)
{
	const size_t count_matrices_comparing_threads = utility::convert_argument<size_t, input_format_error>(token,
		std::format("cmcd_config_parser_default::process_count_matrices_comparing_threads: "
			"can not convert {} to size_t.", token));
	config.set_count_matrices_comparing_threads(count_matrices_comparing_threads);
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_matrices_comparing_algorithm(std::string token, 
	cmcd_config& config)
{
	config.set_matrices_comparing_algorithm(std::move(token));
}

void code_clones_analysis_top_level::cmcd_config_parser_default::process_additional_argument(
	const std::string& argument_name, const std::string& token, cmcd_config& config)
{
	config.add_param(argument_name, token);
}
