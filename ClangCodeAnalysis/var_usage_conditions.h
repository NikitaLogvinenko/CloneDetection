#pragma once

namespace clang_code_analysis
{
	enum class var_usage_conditions_default
	{
		is_local_var,
		is_param,
		is_member_field,
		is_global_var,

		changed_by_operator,
		used_inside_call_expr,
		used_with_square_brackets,
		used_inside_square_brackets,

		used_for_addition_or_subtraction,
		used_for_multiplication_or_division,
		used_for_modulus,
		used_for_comparison,

		used_inside_conditional_statement,
		used_inside_loop,

		defined_with_call_expr,
		defined_with_addition_or_subtraction,
		defined_with_multiplication_or_division,
		defined_with_modulus,
		defined_with_comparison,
		defined_with_literals
	};

	constexpr size_t var_usage_conditions_total = static_cast<size_t>(var_usage_conditions_default::defined_with_literals) + 1;
}
