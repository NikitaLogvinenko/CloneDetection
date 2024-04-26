#pragma once
#include "funcs_analysis_through_count_matrix_factory_abstract.h"
#include "var_usage_conditions.h"

namespace code_clones_analysis_top_level
{
	class funcs_analysis_through_count_matrix_factory_default final
		: public funcs_analysis_through_count_matrix_factory_abstract<clang_code_analysis::var_usage_conditions_total>
	{
	public:
		static constexpr size_t conditions_count = clang_code_analysis::var_usage_conditions_total;

		using traits = code_analysis_through_cm::funcs_analysis_traits<conditions_count>;

		using code_implementations_info_director_abstract = code_analysis_through_cm::code_implementations_info_director_abstract<traits>;

		using code_implementation_info_builder_abstract = code_analysis_through_cm::code_implementation_info_builder_abstract<traits>;

		using code_traversers_factory_abstract = code_analysis_through_cm::code_traversers_factory_abstract<traits>;

		[[nodiscard]] std::unique_ptr<code_implementations_info_director_abstract> create_director(
			const cmcd_config& config) const override;

		[[nodiscard]] std::unique_ptr<code_implementation_info_builder_abstract> create_builder(
			const cmcd_config& config) const override;

		[[nodiscard]] std::unique_ptr<code_traversers_factory_abstract> create_traversers_factory(
			const cmcd_config& config, const std::filesystem::path& project_directory) const override;
	};
}
