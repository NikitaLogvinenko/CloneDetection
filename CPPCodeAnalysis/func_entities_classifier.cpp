#include "func_entities_classifier.h"
#include "spelling_extractor.h"
#include "code_entity_spelling.h"
#include <stdexcept>

namespace cpp_code_analysis
{
	bool func_entities_classifier::try_insert(const CXCursor& cursor)
	{
		const auto kind = clang_getCursorKind(cursor);
		const bool inserted_reference_to_variable = try_insert_reference_to_var(cursor, kind) || try_insert_variable(cursor);
		const bool inserted_cursor = try_insert_cursor(cursor, kind);
		return inserted_reference_to_variable || inserted_cursor;
	}

	bool func_entities_classifier::try_insert_reference_to_var(const CXCursor& cursor, const CXCursorKind& kind)
	{
		if (kind != CXCursor_DeclRefExpr && kind != CXCursor_MemberRefExpr)
		{
			return false;
		}
		if (const CXCursor referenced_cursor = clang_getCursorReferenced(cursor);
			clang_c_adaptation::common_checks::is_cursor_to_var_decl(referenced_cursor))
		{
			const auto [iterator, inserted] = 
				var_origin_and_usage_counter_by_decl_cursor_.try_emplace(
					referenced_cursor,
					determine_var_origin(referenced_cursor),
					initial_usage_counter_value_by_reference);
			auto& origin_and_usage_counter_pair = iterator->second;
			++origin_and_usage_counter_pair.used_n_times();
			return inserted;
		}
		return false;
	}

	bool func_entities_classifier::try_insert_variable(const CXCursor& cursor)
	{
		if (!clang_c_adaptation::common_checks::is_cursor_to_var_decl(cursor))
		{
			return false;
		}
		const auto origin = determine_var_origin(cursor);
		var_origin_and_usage_counter_by_decl_cursor_.try_emplace(cursor, origin, initial_usage_counter_value_by_declaration);
		return true;
	}

	bool func_entities_classifier::try_insert_cursor(const CXCursor& cursor, const CXCursorKind& kind)
	{
		const std::vector<func_entity_type> entity_types = determine_entity_types(cursor, kind);
		if (entity_types.empty())
		{
			return false;
		}
		for (const auto& entity_type : entity_types)
		{
			cursors_by_entity_type_.at(entity_type).emplace(cursor);
		}
		return true;
	}

	clang_c_adaptation::var_origin func_entities_classifier::determine_var_origin(const CXCursor& cursor_to_var_decl)
	{
		switch (clang_getCursorKind(cursor_to_var_decl))
		{
		case CXCursor_ParmDecl:
			return clang_c_adaptation::var_origin::func_param;
		case CXCursor_FieldDecl:
			return clang_c_adaptation::var_origin::member_field;
		case CXCursor_VarDecl:
		{
			switch (clang_getCursorLinkage(cursor_to_var_decl))
			{
			case CXLinkage_NoLinkage:
				return clang_c_adaptation::var_origin::local_var;
			case CXLinkage_Internal:
			case CXLinkage_UniqueExternal:
			case CXLinkage_External:
				return clang_c_adaptation::var_origin::global_var;
			case CXLinkage_Invalid:
				return clang_c_adaptation::var_origin::unknown;
			}
		}
		default:
			throw std::invalid_argument(not_var_decl_msg);
		}
	}


	std::vector<func_entity_type> func_entities_classifier::determine_entity_types(const CXCursor& cursor,
	                                                                         const CXCursorKind& kind)
	{
		if (easy_determinable_entities.contains(kind))
		{
			return { easy_determinable_entities.at(kind) };
		}
		if (kind == CXCursor_BinaryOperator)
		{
			return determine_entity_types_of_binary_operator(cursor);
		}
		if (kind == CXCursor_UnaryOperator)
		{
			return determine_entity_types_of_unary_operator(cursor);
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

	std::vector<func_entity_type> func_entities_classifier::determine_entity_types_of_binary_operator(const CXCursor& cursor)
	{
		if (const std::string spelling = clang_c_adaptation::spelling_extractor::get_binary_operator_spelling(cursor);
			binary_operator_type_by_spelling.contains(spelling))
		{
			return { binary_operator_type_by_spelling.at(spelling) };
		}
		return {};
	}

	std::vector<func_entity_type> func_entities_classifier::determine_entity_types_of_unary_operator(const CXCursor& cursor)
	{
		if (const std::string spelling = clang_c_adaptation::spelling_extractor::get_unary_operator_spelling(cursor);
			unary_operator_type_by_spelling.contains(spelling))
		{
			return { unary_operator_type_by_spelling.at(spelling) };
		}
		return {};
	}

	std::vector<func_entity_type> func_entities_classifier::determine_entity_types_of_compound_assign(const CXCursor& cursor)
	{
		if (const std::string spelling = clang_c_adaptation::spelling_extractor::get_compound_assign_spelling(cursor);
			compound_assign_type_by_spelling.contains(spelling))
		{
			return { compound_assign_type_by_spelling.at(spelling) };
		}
		return {};
	}

	std::vector<func_entity_type> func_entities_classifier::determine_entity_types_of_call_expr(const CXCursor& cursor)
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
		if (binary_operator_type_by_spelling.contains(operator_spelling_end) && !unary_operator_type_by_spelling.contains(operator_spelling_end))
		{
			return { func_entity_type::any_call_expr, binary_operator_type_by_spelling.at(operator_spelling_end) };
		}
		if (unary_operator_type_by_spelling.contains(operator_spelling_end) && !binary_operator_type_by_spelling.contains(operator_spelling_end))
		{
			return { func_entity_type::any_call_expr, unary_operator_type_by_spelling.at(operator_spelling_end) };
		}

		if (unary_operator_type_by_spelling.contains(operator_spelling_end) && binary_operator_type_by_spelling.contains(operator_spelling_end))
		{
			size_t direct_children_counter = 0;
			clang_visitChildren(cursor, visitor_direct_children_counter, &direct_children_counter);
			if (direct_children_counter == unary_plus_minus_operator_direct_children)
			{
				return { func_entity_type::any_call_expr, unary_operator_type_by_spelling.at(operator_spelling_end) };
			}
			if (direct_children_counter == binary_plus_minus_operator_direct_children)
			{
				return { func_entity_type::any_call_expr, binary_operator_type_by_spelling.at(operator_spelling_end) };
			}
		}

		return { func_entity_type::any_call_expr };
	}
}
