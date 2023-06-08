#include "clang_c_types_handling.h"
#include <stdexcept>

namespace clang_c_adaptation
{
	void clang_c_types_handling::cursor_not_null_and_valid(const CXCursor& cursor)
	{
		if (clang_Cursor_isNull(cursor))
		{
			throw std::invalid_argument(null_cursor_msg);
		}
		if (clang_isInvalid(clang_getCursorKind(cursor)))
		{
			throw std::invalid_argument(invalid_cursor_msg);
		}
	}

	void clang_c_types_handling::client_data_not_null_validation(const CXClientData& client_data)
	{
		if (client_data == nullptr)
		{
			throw std::invalid_argument(null_client_data_msg);
		}
	}

	std::string clang_c_types_handling::cxstring_to_string(const std::unique_ptr<CXString> cxstring_ptr)
	{
		try
		{
			std::string str{clang_getCString(*cxstring_ptr)};
			clang_disposeString(*cxstring_ptr);
			return str;
		}
		catch (const std::exception& ex)
		{
			clang_disposeString(*cxstring_ptr);
			throw std::runtime_error(cxstring_conversion_error_msg + ex.what());
		}
	}

	bool clang_c_types_handling::is_cursor_to_var_decl(const CXCursor& cursor) noexcept
	{
		return var_decl_kinds.contains(clang_getCursorKind(cursor));
	}

	bool clang_c_types_handling::is_decl_ref_expr_to_var_decl(const CXCursor& cursor) noexcept
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
		return is_cursor_to_func_declaration(cursor) && clang_isCursorDefinition(cursor);
	}

	bool clang_c_types_handling::is_cursor_to_literal(const CXCursor& cursor) noexcept
	{
		return literals_kinds.contains(clang_getCursorKind(cursor));
	}

	var_linkage clang_c_types_handling::determine_var_linkage(const CXCursor& cursor_to_var_decl)
	{
		const auto kind = clang_getCursorKind(cursor_to_var_decl);
		if (kind == CXCursor_ParmDecl)
		{
			return var_linkage::func_param;
		}
		if (kind == CXCursor_FieldDecl)
		{
			return var_linkage::member_field;
		}
		if (kind == CXCursor_VarDecl)
		{
			const auto clang_linkage = clang_getCursorLinkage(cursor_to_var_decl);
			if (clang_linkage == CXLinkage_NoLinkage)
			{
				return var_linkage::local_var;
			}
			if (clang_linkage == CXLinkage_Internal)
			{
				return var_linkage::global_var;
			}
			if (clang_linkage == CXLinkage_External)
			{
				return var_linkage::static_field;
			}
			return var_linkage::unknown;
		}
		throw std::invalid_argument(not_var_decl_msg);
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
		try
		{
			const auto tu = clang_Cursor_getTranslationUnit(cursor);
			clang_tokenize(tu, extent, &tokens, &tokens_n);
			std::vector<std::string> tokens_vector{};
			tokens_vector.reserve(tokens_n);
			for (unsigned token_index = 0; token_index < tokens_n; ++token_index)
			{
				tokens_vector.emplace_back(cxstring_to_string(
					std::make_unique<CXString>(clang_getTokenSpelling(tu, tokens[token_index]))));
			}
			clang_disposeTokens(clang_Cursor_getTranslationUnit(cursor), tokens, tokens_n);
			return tokens_vector;
		}
		catch (const std::exception& ex)
		{
			clang_disposeTokens(clang_Cursor_getTranslationUnit(cursor), tokens, tokens_n);
			throw std::runtime_error(cursor_tokenization_error_msg + ex.what());
		}
	}

	std::string clang_c_types_handling::join(const std::vector<std::string>& strings, const std::string& sep)
	{
		if (strings.empty())
		{
			return empty_string;
		}

		std::string result{};
		for (size_t str_index = 0; str_index + 1 < strings.size(); ++str_index)
		{
			result += strings[str_index];
			result += sep;
		}
		result += strings[strings.size() - 1];
		return result;
	}

	std::string clang_c_types_handling::get_binary_operator_spelling(const CXCursor& cursor_to_binary_op)
	{
		return get_spelling_of_cursor_between_two_children_parts(cursor_to_binary_op, CXCursor_BinaryOperator);
	}

	std::string clang_c_types_handling::get_unary_operator_spelling(const CXCursor& cursor_to_unary_op)
	{
		if (clang_getCursorKind(cursor_to_unary_op) != CXCursor_UnaryOperator)
		{
			return empty_string;
		}
		const std::string all_tokens = join(get_cursor_extent_tokens(cursor_to_unary_op), tokens_sep);
		std::vector<std::string> vector_of_joined_tokens{};
		clang_visitChildren(cursor_to_unary_op, visitor_get_children_joined_tokens, &vector_of_joined_tokens);
		const std::string& part_without_operator = vector_of_joined_tokens[0];
		if (all_tokens.ends_with(part_without_operator))
		{
			return all_tokens.substr(first_str_symbol, all_tokens.size() - part_without_operator.size());
		}
		if (all_tokens.starts_with(part_without_operator))
		{
			return all_tokens.substr(part_without_operator.size());
		}
		return empty_string;
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

	CXChildVisitResult clang_c_types_handling::visitor_get_children_joined_tokens(const CXCursor cursor, const CXCursor,
		const CXClientData vector_of_joined_tokens_void_ptr)
	{
		{
			client_data_not_null_validation(vector_of_joined_tokens_void_ptr);
			if (vector_of_joined_tokens_void_ptr == nullptr)
			{
				throw std::invalid_argument(null_client_data_msg);
			}
			static_cast<std::vector<std::string>* const>(vector_of_joined_tokens_void_ptr)->push_back(join(get_cursor_extent_tokens(cursor), {}));
			return CXChildVisit_Continue;
		}
	}

	std::string clang_c_types_handling::get_spelling_of_cursor_between_two_children_parts(const CXCursor& cursor,
		const CXCursorKind expected_kind)
	{
		{
			if (clang_getCursorKind(cursor) != expected_kind)
			{
				return empty_string;
			}
			const std::string all_tokens = join(get_cursor_extent_tokens(cursor), tokens_sep);

			std::vector<std::string> vector_of_joined_tokens{};
			vector_of_joined_tokens.reserve(reserved_space_for_two_children);
			clang_visitChildren(cursor, visitor_get_children_joined_tokens, &vector_of_joined_tokens);

			const std::string& left_part = vector_of_joined_tokens[0];
			const std::string& right_part = vector_of_joined_tokens[1];
			if (!all_tokens.starts_with(left_part) || !all_tokens.ends_with(right_part) || left_part.size() + right_part.size() >= all_tokens.size())
			{
				return empty_string;
			}
			return all_tokens.substr(left_part.size(), all_tokens.size() - left_part.size() - right_part.size());
		}
	}
}
