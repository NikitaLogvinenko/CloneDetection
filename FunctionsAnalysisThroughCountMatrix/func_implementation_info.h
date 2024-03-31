#pragma once
#include "func_id.h"
#include "var_id.h"
#include "count_matrix.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class func_implementation_info final
	{
		code_analysis::func_id id_{};
		std::vector<code_analysis::var_id> vars_ids_{};
		cm::count_matrix<UsageConditionsCount> vars_usage_count_matrix_{};

	public:
		func_implementation_info() noexcept = default;

		func_implementation_info(const code_analysis::func_id id, std::vector<code_analysis::var_id> vars_ids, 
			cm::count_matrix<UsageConditionsCount> vars_usage_count_matrix)
			: id_(id), vars_ids_(std::move(vars_ids)), vars_usage_count_matrix_(std::move(vars_usage_count_matrix))
		{
			if (vars_ids_.size() != vars_usage_count_matrix_.vectors_count())
			{
				throw common_exceptions::incorrect_size_error("func_implementation_info::func_implementation_info:"
												  "vars_ids and vars_usage_count_matrix must have equal size.");
			}
		}

		[[nodiscard]] const code_analysis::func_id& id() const noexcept
		{
			return id_;
		}

		[[nodiscard]] const std::vector<code_analysis::var_id>& vars_ids() const noexcept
		{
			return vars_ids_;
		}

		[[nodiscard]] const cm::count_matrix<UsageConditionsCount>& vars_usage_count_matrix() const noexcept
		{
			return vars_usage_count_matrix_;
		}
	};
}
