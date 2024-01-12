#pragma once
#include "func_implementation_info.h"
#include "var_usage_condition_descriptor.h"
#include <unordered_map>

namespace functions_analysis
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_builder final
	{
	public:
		using func_info = func_implementation_info<UsageConditionsCount>;

	private:
		using usage_conditions_counters = std::vector<cm::counted_value>;
		using counters_by_var_descriptor = std::unordered_map<var_descriptor, usage_conditions_counters>;

		std::unordered_map<func_descriptor, counters_by_var_descriptor> vars_info_by_func_descriptor_{};

	public:
		void add_condition(var_usage_condition_descriptor<UsageConditionsCount> usage_condition);

		[[nodiscard]] std::vector<func_info> build_and_reset();

	private:
		[[nodiscard]] func_info construct_func_info(func_descriptor func, counters_by_var_descriptor counters_by_vars) const;
	};
}
