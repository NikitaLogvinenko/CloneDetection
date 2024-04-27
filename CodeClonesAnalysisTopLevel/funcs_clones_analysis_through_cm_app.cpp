#include "funcs_clones_analysis_through_cm_app.h"
#include "arguments_count_error.h"
#include "var_usage_conditions.h"
#include "cmcd_facade.h"
#include "cmcd_config_parser_default.h"
#include "funcs_analysis_through_count_matrix_factory_default.h"
#include "funcs_comparing_through_count_matrix_factory_default.h"
#include "cmcd_results_saver_factory_default.h"
#include "cmcd_output_provider_file.h"
#include <fstream>

namespace
{
	constexpr size_t conditions_count = clang_code_analysis::var_usage_conditions_total;

	enum class argv_names
	{
		// ReSharper disable once CppEnumeratorNeverUsed
		program_name,
		config_path
	};

	constexpr size_t expected_argc = static_cast<size_t>(argv_names::config_path) + 1;
}

void code_clones_analysis_top_level::funcs_clones_analysis_through_cm_app::exec(int argc, const char* const* argv)
{
	if (argc != expected_argc)
	{
		throw common_exceptions::arguments_count_error{ std::format("funcs_clones_analysis_through_cm_app::exec: {} argc expected, but {} passed.",
			expected_argc, argc) };
	}

	const auto config_filename = argv[static_cast<size_t>(argv_names::config_path)];
	std::ifstream config_input{ config_filename };
	utility::throw_if_not_open(config_input, config_filename, "funcs_clones_analysis_through_cm_app::exec");

	cmcd_facade<conditions_count>::exec(config_input, 
		std::make_unique<const cmcd_config_parser_default>(),
		std::make_unique<const funcs_analysis_through_count_matrix_factory_default<conditions_count>>(),
		std::make_unique<const funcs_comparing_through_count_matrix_factory_default<conditions_count>>(),
		std::make_unique<const cmcd_results_saver_factory_default<conditions_count>>(),
		std::make_unique<cmcd_output_provider_file>());
}
