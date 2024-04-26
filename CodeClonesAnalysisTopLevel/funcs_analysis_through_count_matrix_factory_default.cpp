#include "funcs_analysis_through_count_matrix_factory_default.h"
#include "code_implementation_info_director_multithreaded.h"
#include "code_implementation_info_builder_default.h"
#include "code_traversers_sharing_units_factory.h"
#include "clang_funcs_traverser.h"
#include "vars_usage_analyzers_factory_default.h"
#include "input_format_error.h"
#include "translation_units_builder.h"

std::unique_ptr<code_clones_analysis_top_level::funcs_analysis_through_count_matrix_factory_default::
code_implementations_info_director_abstract> code_clones_analysis_top_level::
funcs_analysis_through_count_matrix_factory_default::create_director(const cmcd_config& config) const
{
	return std::make_unique<code_analysis_through_cm::code_implementation_info_director_multithreaded<traits>>(config.code_analysis_threads());
}

std::unique_ptr<code_clones_analysis_top_level::funcs_analysis_through_count_matrix_factory_default::
code_implementation_info_builder_abstract> code_clones_analysis_top_level::
funcs_analysis_through_count_matrix_factory_default::create_builder(const cmcd_config& config) const
{
	return std::make_unique<code_analysis_through_cm::code_implementation_info_builder_default<traits>>();
}

std::unique_ptr<code_clones_analysis_top_level::funcs_analysis_through_count_matrix_factory_default::
code_traversers_factory_abstract> code_clones_analysis_top_level::funcs_analysis_through_count_matrix_factory_default::
create_traversers_factory(const cmcd_config& config, const std::filesystem::path& project_directory) const
{
	if (config.framework() != "clang" && !config.framework().empty())
	{
		throw common_exceptions::input_format_error{ "funcs_analysis_through_count_matrix_factory_default::create_traversers_factory: "
											  "only clang framework supported now." };
	}

	auto translation_units = translation_units_builder{}.build(project_directory);

	auto factory = std::make_unique<code_analysis_through_cm::code_traversers_sharing_units_factory<
		traits, clang_c_adaptation::translation_unit_raii, clang_code_analysis::clang_funcs_traverser<conditions_count>>>(std::move(translation_units),
			[](clang_code_analysis::clang_funcs_traverser<conditions_count>& traverser)
			{
				auto analyzers = clang_code_analysis::vars_usage_analyzers_factory_default{}.create_analyzers();
				traverser.set_analyzers(std::move(analyzers));
			});

	return factory;
}
