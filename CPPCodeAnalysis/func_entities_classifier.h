﻿#pragma once
#include "cxcursor_hash.h"
#include "cxcursors_equal.h"
#include "func_entity_type.h"
#include "var_origin_and_usage_counter_pair.h"
#include "common_checks.h"
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace cpp_code_analysis
{
	class func_entities_classifier final
	{
		std::unordered_map<
			CXCursor,
			var_origin_and_usage_counter_pair,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal> var_origin_and_usage_counter_by_decl_cursor_{};

		std::unordered_map<
			func_entity_type,
			std::unordered_set<
				CXCursor,
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>> cursors_by_entity_type_{};

		inline static const std::unordered_set<
			CXCursor,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal> no_cursors_by_entity_type{};

		inline static const std::unordered_map<CXCursorKind, func_entity_type> easy_determinable_entities{
			{ CXCursor_VarDecl, func_entity_type::local_var },
			{ CXCursor_IfStmt, func_entity_type::if_stmt },
			{ CXCursor_SwitchStmt, func_entity_type::switch_stmt },
			{ CXCursor_ConditionalOperator, func_entity_type::conditional_operator },
			{ CXCursor_ForStmt, func_entity_type::for_stmt },
			{ CXCursor_CXXForRangeStmt, func_entity_type::for_range_stmt },
			{ CXCursor_WhileStmt, func_entity_type::while_stmt },
			{ CXCursor_ArraySubscriptExpr, func_entity_type::array_subscript_expr }
		};

		inline static const std::unordered_map<std::string, func_entity_type> binary_operator_type_by_spelling{
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

		inline static const std::unordered_map<std::string, func_entity_type> unary_operator_type_by_spelling{
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

		inline static const std::string not_var_decl_msg{"determine_var_origin: cursor does not point to any type of variables declarations."};
		inline static const std::string operator_spelling_start{ "operator" };
		inline static const std::string square_brackets{ "[]" };
		inline static const std::string round_brackets{ "()" };

		static constexpr size_t unary_plus_minus_operator_direct_children = 2;
		static constexpr size_t binary_plus_minus_operator_direct_children = 3;

		static constexpr size_t initial_usage_counter_value_by_reference = 0;
		static constexpr size_t initial_usage_counter_value_by_declaration = 0;

	public:
		func_entities_classifier()
		{
			cursors_by_entity_type_.reserve(total_func_entities_types);
			for (size_t func_entity_type_index = 0; func_entity_type_index < total_func_entities_types; ++func_entity_type_index)
			{
				cursors_by_entity_type_.try_emplace(static_cast<func_entity_type>(func_entity_type_index), no_cursors_by_entity_type);
			}
		}

		bool try_insert(const CXCursor& cursor);

		[[nodiscard]] const auto& var_origin_and_usage_counter_by_decl_cursors() const noexcept
		{
			return var_origin_and_usage_counter_by_decl_cursor_;
		}

		[[nodiscard]] const std::unordered_set<
			CXCursor,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>& cursors_to_entities(const func_entity_type entity_type) const noexcept
		{
			return cursors_by_entity_type_.at(entity_type);
		}

	private:
		bool try_insert_reference_to_var(const CXCursor& cursor, const CXCursorKind& kind);
		bool try_insert_variable(const CXCursor& cursor);
		bool try_insert_cursor(const CXCursor& cursor, const CXCursorKind& kind);

		[[nodiscard]] static clang_c_adaptation::var_origin determine_var_origin(const CXCursor& cursor_to_var_decl);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types(const CXCursor& cursor, const CXCursorKind& kind);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_binary_operator(const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_unary_operator(const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_compound_assign(const CXCursor& cursor);
		[[nodiscard]] static std::vector<func_entity_type> determine_entity_types_of_call_expr(const CXCursor& cursor);

		[[nodiscard]] static CXChildVisitResult visitor_direct_children_counter(
			CXCursor cursor, CXCursor parent, const CXClientData void_ptr_to_size_t_counter)
		{
			clang_c_adaptation::common_checks::client_data_not_null_validation(void_ptr_to_size_t_counter);
			++*static_cast<size_t* const>(void_ptr_to_size_t_counter);
			return CXChildVisit_Continue;
		}
	};
}
