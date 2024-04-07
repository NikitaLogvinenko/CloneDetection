#include "operator_classifier.h"
#include "unordered_map"
#include "code_entity_spelling_creator.h"
#include "cursor_classifier.h"
#include "unexpected_behaviour_error.h"
#include <string>

namespace
{
	using namespace clang_code_analysis;

	const std::string operator_spelling_start{ "operator" };

	const std::unordered_map<CXBinaryOperatorKind, operator_kind> built_in_binary_operator_kinds_map{
		{ CXBinaryOperator_Add, operator_kind::built_in_addition },
		{ CXBinaryOperator_Sub, operator_kind::built_in_subtraction },
		{ CXBinaryOperator_Mul, operator_kind::built_in_multiplication },
		{ CXBinaryOperator_Div, operator_kind::built_in_division },
		{ CXBinaryOperator_Rem, operator_kind::built_in_modulus },
		{ CXBinaryOperator_LT, operator_kind::built_in_less_than },
		{ CXBinaryOperator_GT, operator_kind::built_in_greater_than },
		{ CXBinaryOperator_LE, operator_kind::built_in_less_or_equal },
		{ CXBinaryOperator_GE, operator_kind::built_in_greater_or_equal },
		{ CXBinaryOperator_EQ, operator_kind::built_in_equal },
		{ CXBinaryOperator_NE, operator_kind::built_in_not_equal },
		{ CXBinaryOperator_AddAssign, operator_kind::built_in_addition_assignment },
		{ CXBinaryOperator_SubAssign, operator_kind::built_in_subtraction_assignment },
		{ CXBinaryOperator_MulAssign, operator_kind::built_in_multiplication_assignment },
		{ CXBinaryOperator_DivAssign, operator_kind::built_in_division_assignment },
		{ CXBinaryOperator_RemAssign, operator_kind::built_in_modulus_assignment },
		{ CXBinaryOperator_Assign, operator_kind::built_in_assignment }
	};

	const std::unordered_map<CXUnaryOperatorKind, operator_kind> built_in_unary_operator_kinds_map{
		{ CXUnaryOperator_PostInc, operator_kind::built_in_postfix_increment},
		{ CXUnaryOperator_PreInc, operator_kind::built_in_prefix_increment },
		{ CXUnaryOperator_PostDec, operator_kind::built_in_postfix_decrement },
		{ CXUnaryOperator_PreDec, operator_kind::built_in_prefix_decrement },
		{ CXUnaryOperator_Plus, operator_kind::built_in_unary_plus},
		{ CXUnaryOperator_Minus, operator_kind::built_in_unary_minus }
	};

	const std::unordered_map<std::string, operator_kind> binary_operator_method_by_spelling{
		{"+", operator_kind::method_addition },
		{"-", operator_kind::method_subtraction },
		{"*", operator_kind::method_multiplication },
		{"/", operator_kind::method_division },
		{"%", operator_kind::method_modulus },
		{"<", operator_kind::method_less_than},
		{">", operator_kind::method_greater_than},
		{"<=", operator_kind::method_less_or_equal},
		{">=", operator_kind::method_greater_or_equal},
		{"==", operator_kind::method_equal},
		{"!=", operator_kind::method_not_equal},
		{ "+=", operator_kind::method_addition_assignment },
		{ "-=", operator_kind::method_subtraction_assignment },
		{ "*=", operator_kind::method_multiplication_assignment },
		{ "/=", operator_kind::method_division_assignment},
		{ "%=", operator_kind::method_modulus_assignment},
		{ "=", operator_kind::method_assignment}
	};

	const std::unordered_map<std::string, std::vector<operator_kind>> unary_operator_method_by_spelling{
		{ "++", { operator_kind::method_postfix_increment, operator_kind::method_prefix_increment } },
		{ "--", { operator_kind::method_postfix_decrement, operator_kind::method_prefix_decrement } },
		{ "+", { operator_kind::method_unary_plus } },
		{ "-", { operator_kind::method_unary_minus } }
	};

	constexpr size_t unary_plus_minus_method_direct_children = 2;
	constexpr size_t binary_plus_minus_method_direct_children = 3;
	constexpr size_t prefix_increment_decrement_method_direct_children = 2;
	constexpr size_t postfix_increment_decrement_method_direct_children = 3;

	// ReSharper disable once CppPassValueParameterByConstReference
	[[nodiscard]] CXChildVisitResult visitor_direct_children_counter(
		const CXCursor cursor, CXCursor /*parent*/, const CXClientData void_ptr_to_size_t_counter)
	{
		const auto counter_ptr = static_cast<size_t* const>(void_ptr_to_size_t_counter);
		size_t& counter = *counter_ptr;
		++counter;
		return CXChildVisit_Continue;
	}

	[[nodiscard]] size_t count_direct_children(const CXCursor& cursor)
	{
		size_t direct_children_counter = 0;
		clang_visitChildren(cursor, visitor_direct_children_counter, &direct_children_counter);
		return direct_children_counter;
	}

	operator_kind classify_if_built_in(const CXCursor& cursor)
	{
		if (clang_getCursorKind(cursor) == CXCursor_ArraySubscriptExpr)
		{
			return operator_kind::built_in_subscript;
		}

		const auto built_in_binary_operator_kind = clang_getCursorBinaryOperatorKind(cursor);

		if (built_in_binary_operator_kinds_map.contains(built_in_binary_operator_kind))
		{
			return built_in_binary_operator_kinds_map.at(built_in_binary_operator_kind);
		}
		if (built_in_binary_operator_kind == CXBinaryOperator_Invalid)
		{
			return operator_kind::another_built_in;
		}

		const auto built_in_unary_operator_kind = clang_getCursorUnaryOperatorKind(cursor);
		if (built_in_unary_operator_kinds_map.contains(built_in_unary_operator_kind))
		{
			return built_in_binary_operator_kinds_map.at(built_in_binary_operator_kind);
		}
		if (built_in_unary_operator_kind == CXUnaryOperator_Invalid)
		{
			return operator_kind::another_built_in;
		}

		return operator_kind::invalid;
	}

	operator_kind classify_if_method_operator(const CXCursor& cursor)
	{
		if (!cursor_classifier::is_call_expr(cursor))
		{
			return operator_kind::invalid;
		}

		const std::string spelling = code_entity_spelling_creator::create(cursor).to_string();
		if (!spelling.starts_with(operator_spelling_start))
		{
			return operator_kind::invalid;
		}

		const std::string operator_spelling_end = spelling.substr(operator_spelling_start.size());
		if (operator_spelling_end == "[]")
		{
			return operator_kind::method_subscript;
		}

		if (binary_operator_method_by_spelling.contains(operator_spelling_end) 
			&& !unary_operator_method_by_spelling.contains(operator_spelling_end))
		{
			return binary_operator_method_by_spelling.at(operator_spelling_end);
		}

		if (unary_operator_method_by_spelling.contains(operator_spelling_end) 
			&& !binary_operator_method_by_spelling.contains(operator_spelling_end))
		{
			if (const auto& possible_kinds = unary_operator_method_by_spelling.at(operator_spelling_end); 
				possible_kinds.size() == 1)
			{
				return possible_kinds[0];
			}

			const size_t direct_children_counter = count_direct_children(cursor);
			if (direct_children_counter == postfix_increment_decrement_method_direct_children)
			{
				if (operator_spelling_end == "++")
				{
					return operator_kind::built_in_postfix_increment;
				}

				if (operator_spelling_end == "--")
				{
					return operator_kind::built_in_postfix_decrement;
				}

				throw common_exceptions::unexpected_behaviour_error{
					"classify_if_method_operator: unhandled case of two unary operators with the same spelling." };
			}

			if (direct_children_counter == prefix_increment_decrement_method_direct_children)
			{
				if (operator_spelling_end == "++")
				{
					return operator_kind::built_in_prefix_increment;
				}

				if (operator_spelling_end == "--")
				{
					return operator_kind::built_in_prefix_decrement;
				}

				throw common_exceptions::unexpected_behaviour_error{
					"classify_if_method_operator: unhandled case of two unary operators with the same spelling." };
			}

			throw common_exceptions::unexpected_behaviour_error{
					"classify_if_method_operator: unhandled case of two unary operators with the same spelling." };
		}

		if (unary_operator_method_by_spelling.contains(operator_spelling_end) 
			&& binary_operator_method_by_spelling.contains(operator_spelling_end))
		{
			const size_t direct_children_counter = count_direct_children(cursor);

			if (direct_children_counter == unary_plus_minus_method_direct_children)
			{
				return unary_operator_method_by_spelling.at(operator_spelling_end).at(0);
			}

			if (direct_children_counter == binary_plus_minus_method_direct_children)
			{
				return binary_operator_method_by_spelling.at(operator_spelling_end);
			}

			throw common_exceptions::unexpected_behaviour_error{
				"classify_if_method_operator: unhandled case of two operators with the same spelling in unary and binary maps." };
		}

		return operator_kind::another_method;
	}
}

operator_kind operator_classifier::classify(const CXCursor& cursor)
{
	if (const auto built_in_kind = classify_if_built_in(cursor); 
		built_in_kind != operator_kind::invalid)
	{
		return built_in_kind;
	}

	return classify_if_method_operator(cursor);
}