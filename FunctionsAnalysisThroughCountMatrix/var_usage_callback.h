#pragma once
#include "func_id.h"
#include "var_id.h"
#include "condition_index.h"
#include "empty_argument_error.h"
#include <functional>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class var_usage_callback final
	{
	public:
		using callback_t = std::function<void(const code_analysis::func_id&, const code_analysis::var_id&, condition_index<UsageConditionsCount>)>;

	private:
		callback_t callback_ =
			[](const code_analysis::func_id&, const code_analysis::var_id&, condition_index<UsageConditionsCount>) {};

	public:
		var_usage_callback() = default;

		explicit var_usage_callback(callback_t callback) : callback_(std::move(callback))
		{
			if (!callback_)
			{
				throw common_exceptions::empty_argument_error{"var_usage_callback::var_usage_callback: callback is empty."};
			}
		}
	};
}