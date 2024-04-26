#pragma once
#include "funcs_comparing_through_count_matrix_factory_abstract.h"
#include "var_usage_conditions.h"

namespace code_clones_analysis_top_level
{
	class funcs_comparing_through_count_matrix_factory_default final
		: public funcs_comparing_through_count_matrix_factory_abstract<clang_code_analysis::var_usage_conditions_total>
	{
	public:
		static constexpr size_t conditions_count = clang_code_analysis::var_usage_conditions_total;

		using traits = funcs_pairwise_comparing_traits<conditions_count>;

		[[nodiscard]] std::unique_ptr<funcs_pairwise_comparer_through_cm> create_funcs_comparer(
			const cmcd_config& config) const override;
	};
}
