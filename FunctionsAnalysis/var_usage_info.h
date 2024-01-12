#pragma once
#include "count_vector.h"
#include "var_descriptor.h"

namespace functions_analysis
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class var_usage_info final
	{
	public:
		using usage_conditions_counters = cm::count_vector<UsageConditionsCount>;

	private:
		var_descriptor descriptor_{};
		usage_conditions_counters conditions_counters_{};

	public:
		var_usage_info() noexcept = default;

		var_usage_info(var_descriptor descriptor, usage_conditions_counters conditions_counters) noexcept
			: descriptor_(std::move(descriptor)), conditions_counters_(std::move(conditions_counters)) {}

		[[nodiscard]] const var_descriptor& descriptor() const noexcept
		{
			return descriptor_;
		}

		[[nodiscard]] const usage_conditions_counters& conditions_counters() const noexcept
		{
			return conditions_counters_;
		}
	};
}
