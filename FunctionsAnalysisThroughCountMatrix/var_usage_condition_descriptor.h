#pragma once
#include "var_descriptor.h"
#include "func_descriptor.h"
#include "condition_index.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class var_usage_condition_descriptor final
	{
		code_analysis::var_descriptor var_{};
		code_analysis::func_descriptor func_{};
		condition_index<UsageConditionsCount> index_{};

	public:
		var_usage_condition_descriptor() noexcept = default;

		var_usage_condition_descriptor(code_analysis::var_descriptor var, code_analysis::func_descriptor func,
			const condition_index<UsageConditionsCount> index) noexcept
			: var_(std::move(var)), func_(std::move(func)), index_(index) {}

		[[nodiscard]] std::pair<code_analysis::var_descriptor&&, code_analysis::func_descriptor&&> var_and_func() && noexcept
		{
			return { std::move(var_), std::move(func_) };
		}

		[[nodiscard]] condition_index<UsageConditionsCount> index() const noexcept
		{
			return index_;
		}
	};
}
