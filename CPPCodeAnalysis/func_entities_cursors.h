#pragma once
#include "cxcursor_hash.h"
#include "cxcursors_equal.h"
#include "func_entity_type.h"
#include "var_linkage_usage_counter_pair.h"
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace cpp_code_analysis
{
	class func_entities_cursors final
	{
		std::unordered_map<
			CXCursor,
			var_linkage_usage_counter_pair,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal> all_used_vars_with_linkage_and_usage_counter_{};

		std::unordered_map<
			func_entity_type,
			std::unordered_set<
				CXCursor,
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>> cursors_by_entity_type_{};

		inline static const std::unordered_set<
			CXCursor,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal> empty_cursors_by_entity_type{};

		inline static const std::unordered_map<CXCursorKind, func_entity_type> entities_by_kind_not_requiring_processing{
			{ CXCursor_VarDecl, func_entity_type::local_var },
			{ CXCursor_IfStmt, func_entity_type::if_stmt },
			{ CXCursor_SwitchStmt, func_entity_type::switch_stmt },
			{ CXCursor_ConditionalOperator, func_entity_type::conditional_operator },
			{ CXCursor_ForStmt, func_entity_type::for_stmt },
			{ CXCursor_CXXForRangeStmt, func_entity_type::for_range_stmt },
			{ CXCursor_WhileStmt, func_entity_type::while_stmt },
			{ CXCursor_ArraySubscriptExpr, func_entity_type::array_subscript_expr }
		};

		inline static const std::unordered_map<std::string, func_entity_type> binary_op_type_by_spelling{
				{ "+", func_entity_type::plus_operator },
				{ "-", func_entity_type::minus_operator },
				{ "*", func_entity_type::multiplication_operator },
				{ "/", func_entity_type::division_operator },
				{ "%", func_entity_type::modulus_operator },
				{ "==", func_entity_type::comparison_operator },
				{ "!=", func_entity_type::comparison_operator },
				{ ">", func_entity_type::comparison_operator },
				{ "<", func_entity_type::comparison_operator },
				{ ">=", func_entity_type::comparison_operator },
				{ "<=", func_entity_type::comparison_operator },
				{ "=", func_entity_type::assignment_operator }
		};

		inline static const std::unordered_map<std::string, func_entity_type> unary_op_type_by_spelling{
			{ "++", func_entity_type::increment_operator },
			{ "--", func_entity_type::decrement_operator },
			{ "+", func_entity_type::unary_plus_operator },
			{ "-", func_entity_type::unary_minus_operator }
		};

		inline static const std::unordered_map<std::string, func_entity_type> compound_assign_type_by_spelling{
			{ "+=", func_entity_type::plus_assignment_operator },
			{ "-=", func_entity_type::minus_assignment_operator },
			{ "*=", func_entity_type::multiplication_assignment_operator },
			{ "/=", func_entity_type::division_assignment_operator },
			{ "%=", func_entity_type::modulus_assignment_operator }
		};

		inline static const std::string operator_spelling_start{ "operator" };
		inline static const std::string square_brackets{ "[]" };
		inline static const std::string round_brackets{ "()" };

		inline static constexpr size_t unary_plus_minus_operator_children_count = 2;
		inline static constexpr size_t binary_plus_minus_operator_children_count = 3;

		inline static constexpr size_t initial_usage_counter_value_by_reference = 0;
		inline static constexpr size_t initial_usage_counter_value_by_declaration = 0;

	public:
		func_entities_cursors() noexcept = default;
		bool try_insert(const CXCursor& cursor);

		[[nodiscard]] const auto& all_used_vars_with_linkage_and_usage_counter() const noexcept
		{
			return all_used_vars_with_linkage_and_usage_counter_;
		}

		[[nodiscard]] const std::unordered_set<
			CXCursor,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>& cursors_to_entities(const func_entity_type entity_type) const noexcept
		{
			if (cursors_by_entity_type_.contains(entity_type))
			{
				return cursors_by_entity_type_.at(entity_type);
			}
			return empty_cursors_by_entity_type;
		}

	private:
		bool try_insert_reference_to_var(const CXCursor& cursor, const CXCursorKind& kind);
		bool try_insert_variable(const CXCursor& cursor);
		bool try_insert_cursor(const CXCursor& cursor, const CXCursorKind& kind);

		[[nodiscard]] static std::vector<func_entity_type> define_entity_types(const CXCursor& cursor, const CXCursorKind& kind);
		[[nodiscard]] static std::vector<func_entity_type> define_entity_types_of_binary_op(const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> define_entity_types_of_unary_op(const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> define_entity_types_of_compound_assign(const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> define_entity_types_of_call_expr(const CXCursor& cursor);
	};
}
