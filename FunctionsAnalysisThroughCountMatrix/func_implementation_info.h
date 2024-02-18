#pragma once
#include "func_descriptor.h"
#include "var_descriptor.h"
#include "count_matrix.h"
#include <unordered_map>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class func_implementation_info final
	{
	public:
		using count_vector = cm::count_vector<UsageConditionsCount>;
		using count_matrix = cm::count_matrix<UsageConditionsCount>;
		using code_analysis::var_descriptor;

	private:
		code_analysis::func_descriptor descriptor_{};
		std::vector<var_descriptor> vars_descriptors_{};
		count_matrix vars_usage_count_matrix_{};

	public:
		func_implementation_info() noexcept = default;

		func_implementation_info(code_analysis::func_descriptor descriptor, std::vector<var_descriptor> vars_descriptors,
			count_matrix vars_usage_count_matrix)
		: descriptor_(std::move(descriptor)), vars_descriptors_(std::move(vars_descriptors)),
		vars_usage_count_matrix_(std::move(vars_usage_count_matrix))
		{
			if (vars_descriptors_.size() != vars_usage_count_matrix_.vectors_count())
			{
				throw common_exceptions::incorrect_size_error("func_implementation_info::func_implementation_info:"
												  "vars_descriptors and vars_usage_count_matrix must have equal sizes.");
			}
		}

		[[nodiscard]] const code_analysis::func_descriptor& descriptor() const noexcept
		{
			return descriptor_;
		}

		[[nodiscard]] const std::vector<var_descriptor>& vars_descriptors() const noexcept
		{
			return vars_descriptors_;
		}

		[[nodiscard]] const count_matrix& vars_usage_count_matrix() const noexcept
		{
			return vars_usage_count_matrix_;
		}
	};
}
