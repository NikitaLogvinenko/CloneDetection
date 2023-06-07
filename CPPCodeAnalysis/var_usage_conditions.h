#pragma once

namespace cpp_code_analysis
{
	enum class var_usage_condition
	{
		is_param,
		is_local_var,
		is_field,
		is_static_var,
		is_global_var,


		used_n_times,
		changed_minimum_n_times,
		used_inside_call_expr,
		used_inside_square_brackets,
		used_with_square_brackets,

		used_for_sum_or_diff,
		used_for_multiplication_or_division,
		used_for_modulus,
		used_for_comparison,

		used_inside_conditional_statement,
		used_inside_loop,


		defined,
		defined_with_call_expr,
		defined_with_sum_or_difference,
		defined_with_multiplication_or_division,
		defined_with_modulus,
		defined_with_comparison,
		defined_with_literals
	};

	inline constexpr size_t default_conditions_total = static_cast<size_t>(var_usage_condition::defined_with_literals) + 1;
}
