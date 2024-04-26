#pragma once
#include <string>

namespace code_clones_analysis_top_level
{
	class cmcd_results_saver_config final
	{
		double min_similarity_{};
		size_t min_variables_{};

	public:
		constexpr cmcd_results_saver_config() noexcept = default;

		constexpr cmcd_results_saver_config(const double min_similarity, const size_t min_variables) noexcept
			: min_similarity_(min_similarity), min_variables_(min_variables) {}

		[[nodiscard]] constexpr double min_similarity() const noexcept
		{
			return min_similarity_;
		}

		[[nodiscard]] constexpr size_t min_variables() const noexcept
		{
			return min_variables_;
		}
	};
}