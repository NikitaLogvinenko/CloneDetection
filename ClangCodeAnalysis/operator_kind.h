#pragma once

namespace clang_code_analysis
{
	enum class operator_kind
	{
		invalid,

		built_in_addition,
		built_in_subtraction,
		built_in_multiplication,
		built_in_division,
		built_in_modulus,
		built_in_less_than,
		built_in_greater_than,
		built_in_less_or_equal,
		built_in_greater_or_equal,
		built_in_equal,
		built_in_not_equal,
		built_in_addition_assignment,
		built_in_subtraction_assignment,
		built_in_multiplication_assignment,
		built_in_division_assignment,
		built_in_modulus_assignment,
		built_in_assignment,

		built_in_postfix_increment,
		built_in_prefix_increment,
		built_in_postfix_decrement,
		built_in_prefix_decrement,
		built_in_unary_plus,
		built_in_unary_minus,

		built_in_subscription,

		method_addition,
		method_subtraction,
		method_multiplication,
		method_division,
		method_modulus,
		method_less_than,
		method_greater_than,
		method_less_or_equal,
		method_greater_or_equal,
		method_equal,
		method_not_equal,
		method_assignment,
		method_addition_assignment,
		method_subtraction_assignment,
		method_multiplication_assignment,
		method_division_assignment,
		method_modulus_assignment,

		method_postfix_increment,
		method_prefix_increment,
		method_postfix_decrement,
		method_prefix_decrement,
		method_address_acquiring,
		method_dereference,
		method_unary_plus,
		method_unary_minus,

		method_subscription,

		another_built_in,
		another_method,
	};
}