#pragma once

enum class var_usage_condition
{
	is_param,
	is_field,
	is_local_var,
	is_static_var,

	used_n_times,
	used_as_param,
	used_for_sum_or_diff,
	used_for_multiplication_or_division,
	used_for_comparison,
	used_inside_if_or_switch_statement,
	used_inside_first_level_loop,
	used_inside_second_level_loop,
	used_inside_third_or_deeper_level_loop,

	defined_with_sum_or_difference,
	defined_with_multiplication_or_division,
	defined_with_comparison,
	defined_with_func,
	defined_with_literals
};

constexpr size_t total_conditions = static_cast<size_t>(var_usage_condition::defined_with_literals) + 1;