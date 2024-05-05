#include "vars_usage_analyzers_factory_default.h"
#include "operator_classifier.h"
#include "local_var_analyzer.h"
#include "param_analyzer.h"
#include "field_analyzer.h"
#include "global_var_analyzer.h"
#include "vars_nested_in_condition_finder.h"
#include "nth_var_nested_in_condition_finder.h"
#include "from_nth_vars_nested_in_condition_finder.h"
#include "condition_nested_in_local_var_definition_finder.h"

namespace
{
	using namespace clang_code_analysis;

	using condition_index = code_analysis_through_cm::condition_index<vars_usage_analyzers_factory_default::conditions_count>;

	using cursor_predicate_t = std::function<bool(const CXCursor&)>;

	using vars_nested_in_condition_finder =
		vars_nested_in_condition_finder<vars_usage_analyzers_factory_default::conditions_count, cursor_predicate_t>;

	using first_var_nested_in_condition_finder =
		nth_var_nested_in_condition_finder<vars_usage_analyzers_factory_default::conditions_count, 1, cursor_predicate_t>;

	using vars_from_second_nested_in_condition_finder =
		from_nth_vars_nested_in_condition_finder<vars_usage_analyzers_factory_default::conditions_count, 2, cursor_predicate_t>;

	using condition_nested_in_local_var_definition_finder =
		condition_nested_in_local_var_definition_finder<vars_usage_analyzers_factory_default::conditions_count, cursor_predicate_t>;

	const std::unordered_set operators_implying_addition_subtraction{ {
		operator_kind::built_in_addition,
		operator_kind::built_in_subtraction,
		operator_kind::built_in_addition_assignment,
		operator_kind::built_in_subtraction_assignment,
		operator_kind::built_in_prefix_increment,
		operator_kind::built_in_postfix_increment,
		operator_kind::built_in_prefix_decrement,
		operator_kind::built_in_postfix_decrement,
		operator_kind::method_addition,
		operator_kind::method_subtraction,
		operator_kind::method_addition_assignment,
		operator_kind::method_subtraction_assignment,
		operator_kind::method_prefix_increment,
		operator_kind::method_postfix_increment,
		operator_kind::method_prefix_decrement,
		operator_kind::method_postfix_decrement
	} };
	
	const std::unordered_set operators_implying_multiplication_division{ {
		operator_kind::built_in_multiplication,
		operator_kind::built_in_division,
		operator_kind::built_in_multiplication_assignment,
		operator_kind::built_in_division_assignment,
		operator_kind::built_in_unary_plus,
		operator_kind::built_in_unary_minus,
		operator_kind::method_multiplication,
		operator_kind::method_division,
		operator_kind::method_multiplication_assignment,
		operator_kind::method_division_assignment,
		operator_kind::method_unary_plus,
		operator_kind::method_unary_minus
	} };

	const std::unordered_set operators_implying_modulus{ {
		operator_kind::built_in_modulus,
		operator_kind::built_in_modulus_assignment,
		operator_kind::method_modulus,
		operator_kind::method_modulus_assignment
	} };

	const std::unordered_set operators_implying_comparison{ {
		operator_kind::built_in_less_than,
		operator_kind::built_in_greater_than,
		operator_kind::built_in_less_or_equal,
		operator_kind::built_in_greater_or_equal,
		operator_kind::built_in_equal,
		operator_kind::built_in_not_equal,
		operator_kind::method_less_than,
		operator_kind::method_greater_than,
		operator_kind::method_less_or_equal,
		operator_kind::method_greater_or_equal,
		operator_kind::method_equal,
		operator_kind::method_not_equal
	} };
	
	const std::vector call_expr_checking{ cursor_predicate_t{&cursor_classifier::is_call_expr} };

	const std::vector literals_checking{ cursor_predicate_t{&cursor_classifier::is_literal} };

	const std::vector conditional_stmt_checking{
		cursor_predicate_t{&cursor_classifier::is_if_stmt},
		cursor_predicate_t{&cursor_classifier::is_switch_stmt},
		cursor_predicate_t{&cursor_classifier::is_ternary_operator_stmt}
	};

	const std::vector loop_checking{
		cursor_predicate_t{&cursor_classifier::is_while_stmt},
		cursor_predicate_t{&cursor_classifier::is_for_stmt},
		cursor_predicate_t{&cursor_classifier::is_for_range_stmt},
		cursor_predicate_t{&cursor_classifier::is_do_stmt}
	};
	
	const std::unordered_set subscript_operators{ {
		operator_kind::built_in_subscript,
		operator_kind::method_subscript
	} };

	const std::unordered_set changing_operators{ {
		operator_kind::built_in_addition_assignment,
		operator_kind::built_in_subtraction_assignment,
		operator_kind::built_in_multiplication_assignment,
		operator_kind::built_in_division_assignment,
		operator_kind::built_in_modulus_assignment,
		operator_kind::built_in_prefix_increment,
		operator_kind::built_in_postfix_increment,
		operator_kind::built_in_prefix_decrement,
		operator_kind::built_in_postfix_decrement,
		operator_kind::method_addition_assignment,
		operator_kind::method_subtraction_assignment,
		operator_kind::method_multiplication_assignment,
		operator_kind::method_division_assignment,
		operator_kind::method_modulus_assignment,
		operator_kind::method_prefix_increment,
		operator_kind::method_postfix_increment,
		operator_kind::method_prefix_decrement,
		operator_kind::method_postfix_decrement
	} };

	[[nodiscard]] std::unique_ptr<vars_nested_in_condition_finder> create_vars_nested_in_operator_finder(
		const std::unordered_set<operator_kind>& appropriate_operators, const var_usage_conditions_default condition)
	{
		cursor_predicate_t predicate{
			[&appropriate_operators](const CXCursor& cursor)
			{
				const auto kind = operator_classifier::classify(cursor);
				return appropriate_operators.contains(kind);
			} };

		auto vars_nested_in_operator_finder = std::make_unique<vars_nested_in_condition_finder>(
			std::vector{ condition_index{static_cast<size_t>(condition) } }, std::move(predicate));

		return vars_nested_in_operator_finder;
	}
	
	[[nodiscard]] std::unique_ptr<vars_nested_in_condition_finder> create_vars_nested_in_condition_finder(
		const std::vector<cursor_predicate_t>& predicates, const var_usage_conditions_default condition)
	{
		cursor_predicate_t any_predicate{
			[predicates](const CXCursor& cursor)
			{
				for (const auto& predicate : predicates)
				{
					if (predicate(cursor))
					{
						return true;
					}
				}
				return false;
			} };

		auto vars_nested_in_condition_finder = std::make_unique<::vars_nested_in_condition_finder>(
			std::vector{ condition_index{static_cast<size_t>(condition) } }, std::move(any_predicate));
		return vars_nested_in_condition_finder;
	}
	
	[[nodiscard]] std::unique_ptr<first_var_nested_in_condition_finder> create_first_var_nested_in_operator_finder(
		const std::unordered_set<operator_kind>& appropriate_operators, const var_usage_conditions_default condition)
	{
		cursor_predicate_t predicate{
			[&appropriate_operators](const CXCursor& cursor)
			{
				const auto kind = operator_classifier::classify(cursor);
				return appropriate_operators.contains(kind);
			} };

		auto first_var_nested_in_operator_finder = std::make_unique<first_var_nested_in_condition_finder>(
			std::vector{ condition_index{static_cast<size_t>(condition) } }, std::move(predicate));

		return first_var_nested_in_operator_finder;
	}

	[[nodiscard]] std::unique_ptr<vars_from_second_nested_in_condition_finder> create_vars_from_second_nested_in_operator_finder(
		const std::unordered_set<operator_kind>& appropriate_operators, const var_usage_conditions_default condition)
	{
		cursor_predicate_t predicate{
			[&appropriate_operators](const CXCursor& cursor)
			{
				const auto kind = operator_classifier::classify(cursor);
				return appropriate_operators.contains(kind);
			} };

		auto vars_from_second_nested_in_operator_finder = std::make_unique<vars_from_second_nested_in_condition_finder>(
			std::vector{ condition_index{static_cast<size_t>(condition) } }, std::move(predicate));

		return vars_from_second_nested_in_operator_finder;
	}

	[[nodiscard]] std::unique_ptr<condition_nested_in_local_var_definition_finder> create_operators_in_local_var_definition_finder(
		const std::unordered_set<operator_kind>& appropriate_operators, const var_usage_conditions_default condition)
	{
		cursor_predicate_t predicate{
			[&appropriate_operators](const CXCursor& cursor)
			{
				const auto kind = operator_classifier::classify(cursor);
				return appropriate_operators.contains(kind);
			} };

		auto operators_in_local_var_definition_finder = std::make_unique<condition_nested_in_local_var_definition_finder>(
			std::vector{ condition_index{static_cast<size_t>(condition) } }, std::move(predicate));

		return operators_in_local_var_definition_finder;
	}

	[[nodiscard]] std::unique_ptr<condition_nested_in_local_var_definition_finder> create_condition_in_local_var_definition_finder(
		const std::vector<cursor_predicate_t>& predicates, const var_usage_conditions_default condition)
	{
		cursor_predicate_t any_predicate{
			[predicates](const CXCursor& cursor)
			{
				for (const auto& predicate : predicates)
				{
					if (predicate(cursor))
					{
						return true;
					}
				}
				return false;
			} };

		auto condition_in_local_var_definition_finder = std::make_unique<::condition_nested_in_local_var_definition_finder>(
			std::vector{ condition_index{static_cast<size_t>(condition) } }, std::move(any_predicate));
		return condition_in_local_var_definition_finder;
	}
}

namespace clang_code_analysis
{
	vars_usage_analyzers_factory_default::conditions_analyzers_vector vars_usage_analyzers_factory_default::create_analyzers()
	{
		conditions_analyzers_vector analyzers{};


		auto local_var_analyzer_ptr = std::make_unique<local_var_analyzer<conditions_count>>(
			std::vector{ condition_index{static_cast<size_t>(var_usage_conditions_default::is_local_var) } });

		auto param_analyzer_ptr = std::make_unique<param_analyzer<conditions_count>>(
			std::vector{ condition_index{static_cast<size_t>(var_usage_conditions_default::is_param) } });

		auto field_analyzer_ptr = std::make_unique<field_analyzer<conditions_count>>(
			std::vector{ condition_index{static_cast<size_t>(var_usage_conditions_default::is_member_field) } });

		auto global_var_analyzer_ptr = std::make_unique<global_var_analyzer<conditions_count>>(
			std::vector{ condition_index{static_cast<size_t>(var_usage_conditions_default::is_global_var) } });

		analyzers.emplace_back(std::move(local_var_analyzer_ptr));
		analyzers.emplace_back(std::move(param_analyzer_ptr));
		analyzers.emplace_back(std::move(field_analyzer_ptr));
		analyzers.emplace_back(std::move(global_var_analyzer_ptr));


		auto vars_used_for_addition_subtraction_finder = create_vars_nested_in_operator_finder(
			operators_implying_addition_subtraction, var_usage_conditions_default::used_for_addition_or_subtraction);
		analyzers.emplace_back(std::move(vars_used_for_addition_subtraction_finder));
		
		auto vars_used_for_multiplication_division_finder = create_vars_nested_in_operator_finder(
			operators_implying_multiplication_division, var_usage_conditions_default::used_for_multiplication_or_division);
		analyzers.emplace_back(std::move(vars_used_for_multiplication_division_finder));

		auto vars_used_for_modulus_finder = create_vars_nested_in_operator_finder(
			operators_implying_modulus, var_usage_conditions_default::used_for_modulus);
		analyzers.emplace_back(std::move(vars_used_for_modulus_finder));

		auto vars_used_for_comparison_finder = create_vars_nested_in_operator_finder(
			operators_implying_comparison, var_usage_conditions_default::used_for_comparison);
		analyzers.emplace_back(std::move(vars_used_for_comparison_finder));

		
		auto vars_nested_in_call_expr_finder = create_vars_nested_in_condition_finder(
			call_expr_checking, var_usage_conditions_default::used_inside_call_expr);
		analyzers.emplace_back(std::move(vars_nested_in_call_expr_finder));

		auto vars_nested_in_conditional_stmt_finder = create_vars_nested_in_condition_finder(
			conditional_stmt_checking, var_usage_conditions_default::used_inside_conditional_statement);
		analyzers.emplace_back(std::move(vars_nested_in_conditional_stmt_finder));

		auto vars_nested_in_loop_finder = create_vars_nested_in_condition_finder(
			loop_checking, var_usage_conditions_default::used_inside_loop);
		analyzers.emplace_back(std::move(vars_nested_in_loop_finder));

		
		auto subscripted_vars_finder = create_first_var_nested_in_operator_finder(
			subscript_operators, var_usage_conditions_default::used_with_square_brackets);
		analyzers.emplace_back(std::move(subscripted_vars_finder));

		auto vars_changed_by_operators_finder = create_first_var_nested_in_operator_finder(
			changing_operators, var_usage_conditions_default::changed_by_operator);
		analyzers.emplace_back(std::move(vars_changed_by_operators_finder));

		
		auto vars_inside_square_brackets_finder = create_vars_from_second_nested_in_operator_finder(
			subscript_operators, var_usage_conditions_default::used_inside_square_brackets);
		analyzers.emplace_back(std::move(vars_inside_square_brackets_finder));


		auto addition_subtraction_in_local_var_definition_finder = create_operators_in_local_var_definition_finder(
			operators_implying_addition_subtraction, var_usage_conditions_default::defined_with_addition_or_subtraction);
		analyzers.emplace_back(std::move(addition_subtraction_in_local_var_definition_finder));

		auto multiplication_division_in_local_var_definition_finder = create_operators_in_local_var_definition_finder(
			operators_implying_multiplication_division, var_usage_conditions_default::defined_with_multiplication_or_division);
		analyzers.emplace_back(std::move(multiplication_division_in_local_var_definition_finder));

		auto modulus_in_local_var_definition_finder = create_operators_in_local_var_definition_finder(
			operators_implying_modulus, var_usage_conditions_default::defined_with_modulus);
		analyzers.emplace_back(std::move(modulus_in_local_var_definition_finder));

		auto comparison_in_local_var_definition_finder = create_operators_in_local_var_definition_finder(
			operators_implying_comparison, var_usage_conditions_default::defined_with_comparison);
		analyzers.emplace_back(std::move(comparison_in_local_var_definition_finder));


		auto call_expr_in_local_var_definition_finder = create_condition_in_local_var_definition_finder(
			call_expr_checking, var_usage_conditions_default::defined_with_call_expr);
		analyzers.emplace_back(std::move(call_expr_in_local_var_definition_finder));

		auto literals_in_local_var_definition_finder = create_condition_in_local_var_definition_finder(
			literals_checking, var_usage_conditions_default::defined_with_literals);
		analyzers.emplace_back(std::move(literals_in_local_var_definition_finder));

		return analyzers;
	}
}