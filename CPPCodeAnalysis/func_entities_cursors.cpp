﻿#include "func_entities_cursors.h"
#include "clang_c_types_handling.h"
#include "code_entity_spelling.h"

namespace cpp_code_analysis
{
	bool func_entities_cursors::try_insert(const CXCursor& cursor)
	{
		const auto kind = clang_getCursorKind(cursor);
		const bool inserted_reference_to_variable = try_insert_reference_to_var(cursor, kind) || try_insert_variable(cursor);
		const bool inserted_cursor = try_insert_cursor(cursor, kind);
		return inserted_reference_to_variable || inserted_cursor;
	}

	bool func_entities_cursors::try_insert_reference_to_var(const CXCursor& cursor, const CXCursorKind& kind)
	{
		if (kind != CXCursor_DeclRefExpr && kind != CXCursor_MemberRefExpr)
		{
			return false;
		}
		if (const CXCursor referenced_cursor = clang_getCursorReferenced(cursor);
			clang_c_adaptation::is_cursor_to_var_decl(referenced_cursor))
		{
			const auto [iterator, inserted] = all_used_vars_with_linkage_and_usage_counter_.try_emplace(referenced_cursor,
				clang_c_adaptation::determine_var_linkage(referenced_cursor), initial_usage_counter_value_by_reference);
			auto& linkage_usage_counter_pair = iterator->second;
			++linkage_usage_counter_pair.used_n_times();
			return inserted;
		}
		return false;
	}

	bool func_entities_cursors::try_insert_variable(const CXCursor& cursor)
	{
		if (!clang_c_adaptation::is_cursor_to_var_decl(cursor))
		{
			return false;
		}
		const auto linkage_type = clang_c_adaptation::determine_var_linkage(cursor);
		all_used_vars_with_linkage_and_usage_counter_.try_emplace(cursor, linkage_type, initial_usage_counter_value_by_declaration);
		return true;
	}

	bool func_entities_cursors::try_insert_cursor(const CXCursor& cursor, const CXCursorKind& kind)
	{
		const std::vector<func_entity_type> entity_types = determine_entity_types(cursor, kind);
		if (entity_types.empty())
		{
			return false;
		}
		for (const auto& entity_type : entity_types)
		{
			if (const auto [iterator, inserted_new_cursors_vector] = 
					cursors_by_entity_type_.try_emplace(
						entity_type, 
						std::unordered_set<
							CXCursor,
							clang_c_adaptation::cxcursor_hash,
							clang_c_adaptation::cxcursors_equal>{cursor});
				!inserted_new_cursors_vector)
			{
				auto& cursors_set = iterator->second;
				cursors_set.emplace(cursor);
			}
		}
		return true;
	}

	std::vector<func_entity_type> func_entities_cursors::determine_entity_types(const CXCursor& cursor,
	                                                                         const CXCursorKind& kind)
	{
		if (entities_by_kind_not_requiring_processing.contains(kind))
		{
			return { entities_by_kind_not_requiring_processing.at(kind) };
		}
		if (kind == CXCursor_BinaryOperator)
		{
			return determine_entity_types_of_binary_op(cursor);
		}
		if (kind == CXCursor_UnaryOperator)
		{
			return determine_entity_types_of_unary_op(cursor);
		}
		if (kind == CXCursor_CompoundAssignOperator)
		{
			return determine_entity_types_of_compound_assign(cursor);
		}
		if (kind == CXCursor_CallExpr)
		{
			return determine_entity_types_of_call_expr(cursor);
		}
		return {};
	}

	std::vector<func_entity_type> func_entities_cursors::determine_entity_types_of_binary_op(const CXCursor& cursor)
	{
		if (const std::string spelling = clang_c_adaptation::get_binary_operator_spelling(cursor);
			binary_op_type_by_spelling.contains(spelling))
		{
			return { binary_op_type_by_spelling.at(spelling) };
		}
		return {};
	}

	std::vector<func_entity_type> func_entities_cursors::determine_entity_types_of_unary_op(const CXCursor& cursor)
	{
		if (const std::string spelling = clang_c_adaptation::get_unary_operator_spelling(cursor);
			unary_op_type_by_spelling.contains(spelling))
		{
			return { unary_op_type_by_spelling.at(spelling) };
		}
		return {};
	}

	std::vector<func_entity_type> func_entities_cursors::determine_entity_types_of_compound_assign(const CXCursor& cursor)
	{
		if (const std::string spelling = clang_c_adaptation::get_compound_assign_spelling(cursor);
			compound_assign_type_by_spelling.contains(spelling))
		{
			return { compound_assign_type_by_spelling.at(spelling) };
		}
		return {};
	}

	std::vector<func_entity_type> func_entities_cursors::determine_entity_types_of_call_expr(const CXCursor& cursor)
	{
		const std::string call_expr_spelling = clang_c_adaptation::code_entity_spelling(cursor).to_string();
		if (!call_expr_spelling.starts_with(operator_spelling_start))
		{
			return { func_entity_type::any_call_expr };
		}

		const std::string operator_spelling_end = call_expr_spelling.substr(operator_spelling_start.size());
		if (operator_spelling_end == square_brackets)
		{
			return { func_entity_type::any_call_expr, func_entity_type::square_brackets_operator };
		}
		if (compound_assign_type_by_spelling.contains(operator_spelling_end))
		{
			return { func_entity_type::any_call_expr, compound_assign_type_by_spelling.at(operator_spelling_end) };
		}
		if (binary_op_type_by_spelling.contains(operator_spelling_end) && !unary_op_type_by_spelling.contains(operator_spelling_end))
		{
			return { func_entity_type::any_call_expr, binary_op_type_by_spelling.at(operator_spelling_end) };
		}
		if (unary_op_type_by_spelling.contains(operator_spelling_end) && !binary_op_type_by_spelling.contains(operator_spelling_end))
		{
			return { func_entity_type::any_call_expr, unary_op_type_by_spelling.at(operator_spelling_end) };
		}

		if (unary_op_type_by_spelling.contains(operator_spelling_end) && binary_op_type_by_spelling.contains(operator_spelling_end))
		{
			size_t direct_children_counter = 0;
			clang_visitChildren(cursor, clang_c_adaptation::visitor_direct_children_counter, &direct_children_counter);
			if (direct_children_counter == unary_plus_minus_operator_children_count)
			{
				return { func_entity_type::any_call_expr, unary_op_type_by_spelling.at(operator_spelling_end) };
			}
			if (direct_children_counter == binary_plus_minus_operator_children_count)
			{
				return { func_entity_type::any_call_expr, binary_op_type_by_spelling.at(operator_spelling_end) };
			}
		}

		return { func_entity_type::any_call_expr };
	}
}
