#pragma once

namespace clang_code_analysis
{
	enum class func_nested_entity
	{
		variable,

		any_call_expr,

		plus_operator,
		minus_operator,
		multiplication_operator,
		division_operator,
		unary_plus_operator,
		unary_minus_operator,
		modulus_operator,
		comparison_operator,
		assignment_operator,

		plus_assignment_operator,
		minus_assignment_operator,
		increment_operator,
		decrement_operator,
		multiplication_assignment_operator,
		division_assignment_operator,
		modulus_assignment_operator,

		square_brackets_operator,
		array_subscript_expr,

		if_stmt,
		switch_stmt,
		conditional_operator,

		do_stmt,
		for_stmt,
		for_range_stmt,
		while_stmt
	};

	constexpr size_t total_func_nested_entities = static_cast<size_t>(func_nested_entity::while_stmt) + 1;
}
