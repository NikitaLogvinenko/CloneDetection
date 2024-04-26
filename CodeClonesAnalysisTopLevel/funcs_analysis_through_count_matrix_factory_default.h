#pragma once
#include "funcs_analysis_through_count_matrix_factory_abstract.h"
#include "code_implementation_info_director_multithreaded.h"
#include "code_implementation_info_builder_default.h"
#include "code_traversers_sharing_units_factory.h"
#include "clang_funcs_traverser.h"
#include "vars_usage_analyzers_factory_default.h"
#include "input_format_error.h"
#include "translation_units_builder.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class funcs_analysis_through_count_matrix_factory_default final
		: public funcs_analysis_through_count_matrix_factory_abstract<ConditionsCount>
	{
	public:
		using traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;

		using code_implementations_info_director_abstract = code_analysis_through_cm::code_implementations_info_director_abstract<traits>;

		using code_implementation_info_builder_abstract = code_analysis_through_cm::code_implementation_info_builder_abstract<traits>;

		using code_traversers_factory_abstract = code_analysis_through_cm::code_traversers_factory_abstract<traits>;

		[[nodiscard]] std::unique_ptr<code_implementations_info_director_abstract> create_director(
			const cmcd_config& config) const override
		{
			return std::make_unique<code_analysis_through_cm::code_implementation_info_director_multithreaded<traits>>(config.code_analysis_threads());
		}

		[[nodiscard]] std::unique_ptr<code_implementation_info_builder_abstract> create_builder(
			const cmcd_config& config) const override
		{
			return std::make_unique<code_analysis_through_cm::code_implementation_info_builder_default<traits>>();
		}

		[[nodiscard]] std::unique_ptr<code_traversers_factory_abstract> create_traversers_factory(
			const cmcd_config& config, const std::filesystem::path& project_directory) const override
		{
			if (config.framework() != "clang" && !config.framework().empty())
			{
				throw common_exceptions::input_format_error{ "funcs_analysis_through_count_matrix_factory_default::create_traversers_factory: "
													  "only clang framework supported now." };
			}

			auto translation_units = clang_c_adaptation::translation_units_builder{}.build(project_directory);

			auto factory = std::make_unique<code_analysis_through_cm::code_traversers_sharing_units_factory<
				traits, clang_c_adaptation::translation_unit_raii, clang_code_analysis::clang_funcs_traverser<ConditionsCount>>>(std::move(translation_units),
					[](clang_code_analysis::clang_funcs_traverser<ConditionsCount>& traverser)
					{
						auto analyzers = clang_code_analysis::vars_usage_analyzers_factory_default{}.create_analyzers();
						traverser.set_analyzers(std::move(analyzers));
					});

			return factory;
		}
	};
}
