#include "clang_c_types_handling.h"
#include "cxstring_wrapper.h"
#include <stdexcept>
#include <numeric>

namespace clang_c_adaptation
{
	void clang_c_types_handling::client_data_not_null_validation(const CXClientData client_data)
	{
		if (client_data == nullptr)
		{
			throw std::invalid_argument(null_client_data_msg);
		}
	}

	bool clang_c_types_handling::is_cursor_to_var_decl(const CXCursor& cursor) noexcept
	{
		return var_decl_kinds.contains(clang_getCursorKind(cursor));
	}

	bool clang_c_types_handling::is_cursor_referring_to_var_decl(const CXCursor& cursor) noexcept
	{
		if (clang_getCursorKind(cursor) != CXCursor_DeclRefExpr)
		{
			return false;
		}
		return is_cursor_to_var_decl(clang_getCursorReferenced(cursor));
	}

	bool clang_c_types_handling::is_cursor_to_func_declaration(const CXCursor& cursor) noexcept
	{
		return func_decl_kinds.contains(clang_getCursorKind(cursor));
	}

	bool clang_c_types_handling::is_cursor_to_func_definition(const CXCursor& cursor) noexcept
	{
		return is_cursor_to_func_declaration(cursor) && clang_isCursorDefinition(cursor) != 0;
	}

	bool clang_c_types_handling::is_cursor_to_literal(const CXCursor& cursor) noexcept
	{
		return literals_kinds.contains(clang_getCursorKind(cursor));
	}

	var_origin clang_c_types_handling::determine_var_origin(const CXCursor& cursor_to_var_decl)
	{
		switch (clang_getCursorKind(cursor_to_var_decl))
		{
		case CXCursor_ParmDecl:
			return var_origin::func_param;
		case CXCursor_FieldDecl:
			return var_origin::member_field;
		case CXCursor_VarDecl:
		{
			switch(clang_getCursorLinkage(cursor_to_var_decl))
			{
			case CXLinkage_NoLinkage:
				return var_origin::local_var;
			case CXLinkage_Internal:
			case CXLinkage_UniqueExternal:
			case CXLinkage_External:
				return var_origin::global_var;
			case CXLinkage_Invalid:
				return var_origin::unknown;
			}
		}
		default:
			throw std::invalid_argument(not_var_decl_msg);
		}
	}

	std::vector<std::string> clang_c_types_handling::get_cursor_extent_tokens(const CXCursor& cursor)
	{
		const CXSourceRange extent = clang_getCursorExtent(cursor);
		CXToken* tokens{};
		unsigned tokens_n = 0;
		if (clang_Range_isNull(extent))
		{
			return {};
		}
		const auto translation_unit = clang_Cursor_getTranslationUnit(cursor);
		std::vector<std::string> tokens_vector{};
		try
		{
			clang_tokenize(translation_unit, extent, &tokens, &tokens_n);
			tokens_vector.reserve(tokens_n);
			for (unsigned token_index = 0; token_index < tokens_n; ++token_index)
			{
				tokens_vector.emplace_back(cxstring_wrapper(clang_getTokenSpelling(translation_unit, tokens[token_index])).c_str());
			}
		}
		catch (const std::exception& ex)
		{
			clang_disposeTokens(translation_unit, tokens, tokens_n);
			throw std::runtime_error(cursor_tokenization_error_msg + ex.what());
		}
		clang_disposeTokens(translation_unit, tokens, tokens_n);
		return tokens_vector;
	}

	std::string clang_c_types_handling::join(const std::vector<std::string>& strings, const std::string& sep)
	{
		if (strings.empty())
		{
			return {};
		}
		const std::string result = std::accumulate(
			strings.cbegin(), strings.cend(), empty_string,
			[&sep](std::string&& prev_part, const std::string& new_part) 
			{ return std::move(prev_part) + new_part + sep; });
		return result.substr(first_str_symbol, result.size() - sep.size());
	}

	std::string clang_c_types_handling::get_binary_operator_spelling(const CXCursor& cursor_to_binary_op)
	{
		return get_spelling_of_cursor_between_two_children_parts(cursor_to_binary_op, CXCursor_BinaryOperator);
	}

	std::string clang_c_types_handling::get_unary_operator_spelling(const CXCursor& cursor_to_unary_op)
	{
		const auto entire_and_subtrees_spellings = get_entire_spelling_and_subtrees_spellings(
			cursor_to_unary_op, CXCursor_UnaryOperator, one_child);
		const std::string& entire_spelling = entire_and_subtrees_spellings[0];
		const std::string& part_without_operator = entire_and_subtrees_spellings[1];
		if (entire_spelling.ends_with(part_without_operator))
		{
			return entire_spelling.substr(first_str_symbol, entire_spelling.size() - part_without_operator.size());
		}
		if (entire_spelling.starts_with(part_without_operator))
		{
			return entire_spelling.substr(part_without_operator.size());
		}
		throw std::runtime_error(unexpected_cursor_to_unary_op_msg);
	}

	std::string clang_c_types_handling::get_compound_assign_spelling(const CXCursor& cursor_to_compound_assign)
	{
		return get_spelling_of_cursor_between_two_children_parts(cursor_to_compound_assign, CXCursor_CompoundAssignOperator);
	}

	CXChildVisitResult clang_c_types_handling::visitor_direct_children_counter(CXCursor, CXCursor, const CXClientData void_ptr_to_size_t_counter)
	{
		client_data_not_null_validation(void_ptr_to_size_t_counter);

		++*static_cast<size_t* const>(void_ptr_to_size_t_counter);
		return CXChildVisit_Continue;
	}

	CXChildVisitResult clang_c_types_handling::visitor_append_subtrees_spellings(const CXCursor cursor, CXCursor,
	const CXClientData ptr_to_vector)
	{
		client_data_not_null_validation(ptr_to_vector);

		const auto vector_append_to = static_cast<std::vector<std::string>* const>(ptr_to_vector);
		vector_append_to->push_back(join(get_cursor_extent_tokens(cursor), {}));
		return CXChildVisit_Continue;
	}

	std::string clang_c_types_handling::get_spelling_of_cursor_between_two_children_parts(const CXCursor& cursor,
		const CXCursorKind expected_kind)
	{
		{
			const auto entire_and_subtrees_spellings = get_entire_spelling_and_subtrees_spellings(
				cursor, expected_kind, two_children);
			const std::string& entire_spelling = entire_and_subtrees_spellings[0];
			const std::string& left_subtree = entire_and_subtrees_spellings[1];
			const std::string& right_subtree = entire_and_subtrees_spellings[2];
			if (!entire_spelling.starts_with(left_subtree) || !entire_spelling.ends_with(right_subtree) 
				|| left_subtree.size() + right_subtree.size() >= entire_spelling.size())
			{
				throw std::runtime_error(cursor_is_not_between_two_children_msg);
			}
			return entire_spelling.substr(left_subtree.size(),
				entire_spelling.size() - left_subtree.size() - right_subtree.size());
		}
	}

	std::vector<std::string> clang_c_types_handling::get_entire_spelling_and_subtrees_spellings(const CXCursor& cursor,
		const CXCursorKind expected_cursor_kind, const size_t children_count)
	{
		if (clang_getCursorKind(cursor) != expected_cursor_kind)
		{
			throw std::invalid_argument(wrong_cursor_type_msg);
		}

		std::vector<std::string> entire_and_subtrees_spellings{};
		const size_t total_spellings = 1 + children_count;
		entire_and_subtrees_spellings.reserve(total_spellings);
		entire_and_subtrees_spellings.emplace_back(
			join(get_cursor_extent_tokens(cursor), tokens_sep));
		clang_visitChildren(cursor, visitor_append_subtrees_spellings, &entire_and_subtrees_spellings);

		if (entire_and_subtrees_spellings.size() != total_spellings)
		{
			throw std::invalid_argument(wrong_children_count_msg);
		}

		return entire_and_subtrees_spellings;
	}
}
