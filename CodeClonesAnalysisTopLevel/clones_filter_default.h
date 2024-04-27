#pragma once
#include "clones_filter_abstract.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class clones_filter_default final : public clones_filter_abstract<ConditionsCount>
	{
		static constexpr double one_hundred_percentages = 100;

	public:
		[[nodiscard]] bool clones(const funcs_pair_comparing_result<ConditionsCount>& result,
			const cmcd_results_saver_config& config) const override
		{
			if (result.first_func_descriptor() == result.second_func_descriptor())
			{
				return false;
			}

			auto min_similarity = config.min_similarity();
			if (min_similarity > 1)
			{
				min_similarity /= one_hundred_percentages;
			}
			if (result.comparing_result().similarity() < min_similarity)
			{
				return false;
			}

			return result.first_func_implementation().nested_entities_ids().size() >= config.min_variables()
				&& result.second_func_implementation().nested_entities_ids().size() >= config.min_variables();
		}
	};
}