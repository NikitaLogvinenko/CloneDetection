#pragma once
#include "func_descriptor.h"
#include "var_usage_info.h"
#include "count_matrix.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class func_implementation_info final
	{
	public:
		using vars_usage_vector = std::vector<var_usage_info<UsageConditionsCount>>;
		using count_matrix = cm::count_matrix<UsageConditionsCount>;

	private:
		code_analysis::func_descriptor descriptor_{};
		vars_usage_vector vars_usage_detailed_{};
		count_matrix vars_usage_count_matrix_{};

	public:
		func_implementation_info() noexcept = default;

		func_implementation_info(code_analysis::func_descriptor descriptor, vars_usage_vector vars_usage_detailed,
			count_matrix vars_usage_count_matrix) noexcept : descriptor_(std::move(descriptor)),
		vars_usage_detailed_(std::move(vars_usage_detailed)), vars_usage_count_matrix_(std::move(vars_usage_count_matrix)) {}

		[[nodiscard]] const code_analysis::func_descriptor& descriptor() const noexcept
		{
			return descriptor_;
		}

		[[nodiscard]] const vars_usage_vector& vars_usage_detailed() const noexcept
		{
			return vars_usage_detailed_;
		}

		[[nodiscard]] const count_matrix& vars_usage_count_matrix() const noexcept
		{
			return vars_usage_count_matrix_;
		}
	};
}
