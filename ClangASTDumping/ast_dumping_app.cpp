#include "ast_dumping_app.h"
#include "ast_dumping_director.h"
#include "ast_dumping_config_parser_default.h"
#include "ast_dumper_default.h"
#include "arguments_count_error.h"
#include "parameters_validation.h"
#include <format>
#include <fstream>

namespace
{
	enum class argv_names
	{
		// ReSharper disable once CppEnumeratorNeverUsed
		program_name,
		config_path
	};

	constexpr size_t expected_argc = static_cast<size_t>(argv_names::config_path) + 1;
}

void clang_ast_dumping::ast_dumping_app::exec(const int argc, const char* const* argv)
{
	if (argc != expected_argc)
	{
		throw common_exceptions::arguments_count_error{ std::format("ast_dumping_app::exec: {} argc expected, but {} passed.",
			expected_argc, argc) };
	}

	const auto config_filename = argv[static_cast<size_t>(argv_names::config_path)];
	std::ifstream config_input{ config_filename };
	utility::throw_if_not_open(config_input, config_filename, "ast_dumping_app::exec");

	ast_dumping_config_parser_default parser{};
	ast_dumper_default dumper{};

	ast_dumping_director::dump(config_input, parser, dumper);
}
