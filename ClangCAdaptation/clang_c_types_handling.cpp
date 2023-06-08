#include "clang_c_types_handling.h"
#include <stdexcept>
#include <unordered_set>

namespace clang_c_adaptation
{
	namespace
	{
		const std::string null_cursor_msg("Can not process null CXCursor");
		const std::string null_client_data_msg("Null client data passed to visitor");
		const std::string invalid_cursor_msg("Invalid CXCursor is encountered");
		const std::string not_var_decl_msg("Cursor does not point to any type of variables declarations");
		const std::string cxstring_conversion_error_msg("Failure during CXString to std::string conversion. CXString was just disposed. Exception: ");
		const std::string cursor_tokenization_error_msg("Failure during CXCursor tokenization. Exception: ");

		const std::unordered_set literals_kinds{
			CXCursor_IntegerLiteral, CXCursor_FloatingLiteral, CXCursor_ImaginaryLiteral,
			CXCursor_StringLiteral, CXCursor_CharacterLiteral, CXCursor_CXXBoolLiteralExpr};
	}

	void cursor_not_null_and_valid(const CXCursor& cursor)
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

	void client_data_not_null_validation(const CXClientData& client_data)
	{
		if (client_data == nullptr)
		{
			throw std::invalid_argument(null_client_data_msg);
		}
	}

	std::string cxstring_to_string(const std::unique_ptr<CXString> cxstring_ptr)
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

	bool is_cursor_to_var_decl(const CXCursor& cursor) noexcept
	{
		const auto kind = clang_getCursorKind(cursor);
		return kind == CXCursor_VarDecl || kind == CXCursor_FieldDecl || kind == CXCursor_ParmDecl;
	}

	bool is_decl_ref_expr_to_var_decl(const CXCursor& cursor) noexcept
	{
		if (clang_getCursorKind(cursor) != CXCursor_DeclRefExpr)
		{
			return false;
		}
		return is_cursor_to_var_decl(clang_getCursorReferenced(cursor));
	}

	bool is_cursor_to_func_declaration(const CXCursor& cursor) noexcept
	{
		return clang_getCursorKind(cursor) == CXCursor_CXXMethod ||
				clang_getCursorKind(cursor) == CXCursor_FunctionDecl ||
				clang_getCursorKind(cursor) == CXCursor_Constructor ||
				clang_getCursorKind(cursor) == CXCursor_Destructor ||
				clang_getCursorKind(cursor) == CXCursor_FunctionTemplate;
	}

	bool is_cursor_to_func_definition(const CXCursor& cursor) noexcept
	{
		return is_cursor_to_func_declaration(cursor) && clang_isCursorDefinition(cursor);
	}

	bool is_cursor_to_literal(const CXCursor& cursor) noexcept
	{
		return literals_kinds.contains(clang_getCursorKind(cursor));
	}

	var_linkage determine_var_linkage(const CXCursor& cursor_to_var_decl)
	{
		const auto kind = clang_getCursorKind(cursor_to_var_decl);
		if (kind == CXCursor_ParmDecl)
		{
			return var_linkage::func_param;
		}
		if (kind == CXCursor_FieldDecl)
		{
			return var_linkage::field;
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
				return var_linkage::static_var;
			}
			return var_linkage::unknown;
		}
		throw std::invalid_argument(not_var_decl_msg);
	}

	std::vector<std::string> get_cursor_extent_tokens(const CXCursor& cursor)
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
			clang_tokenize(clang_Cursor_getTranslationUnit(cursor), extent, &tokens, &tokens_n);
			std::vector<std::string> tokens_vector{};
			tokens_vector.reserve(tokens_n);
			for (unsigned token_index = 0; token_index < tokens_n; ++token_index)
			{
				tokens_vector.emplace_back(cxstring_to_string(
					std::make_unique<CXString>(clang_getTokenSpelling(clang_Cursor_getTranslationUnit(cursor), tokens[token_index]))));
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

	std::string join(const std::vector<std::string>& strings, const std::string& sep)
	{
		if (strings.empty())
		{
			return {};
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

	namespace
	{
		CXChildVisitResult visitor_get_children_joined_tokens(const CXCursor cursor, const CXCursor /*parent*/, const CXClientData vector_of_joined_tokens_void_ptr)
		{
			if (vector_of_joined_tokens_void_ptr == nullptr)
			{
				throw std::invalid_argument(null_client_data_msg);
			}
			static_cast<std::vector<std::string>* const>(vector_of_joined_tokens_void_ptr)->push_back(join(get_cursor_extent_tokens(cursor), ""));
			return CXChildVisit_Continue;
		}

		constexpr size_t reserved_space_for_two_children = 2;
		std::string get_spelling_of_cursor_between_two_children_parts(const CXCursor& cursor, const CXCursorKind expected_kind)
		{
			if (clang_getCursorKind(cursor) != expected_kind)
			{
				return "";
			}
			const std::string all_tokens = join(get_cursor_extent_tokens(cursor), {});
			std::vector<std::string> vector_of_joined_tokens{};
			vector_of_joined_tokens.reserve(reserved_space_for_two_children);
			clang_visitChildren(cursor, visitor_get_children_joined_tokens, &vector_of_joined_tokens);
			const std::string& left_part = vector_of_joined_tokens[0];
			const std::string& right_part = vector_of_joined_tokens[1];
			if (!all_tokens.starts_with(left_part) || !all_tokens.ends_with(right_part) || left_part.size() + right_part.size() >= all_tokens.size())
			{
				return "";
			}
			return all_tokens.substr(left_part.size(), all_tokens.size() - left_part.size() - right_part.size());
		}
	}

	std::string get_binary_operator_spelling(const CXCursor& cursor_to_binary_op)
	{
		return get_spelling_of_cursor_between_two_children_parts(cursor_to_binary_op, CXCursor_BinaryOperator);
	}

	std::string get_unary_operator_spelling(const CXCursor & cursor_to_unary_op)
	{
		if (clang_getCursorKind(cursor_to_unary_op) != CXCursor_UnaryOperator)
		{
			return "";
		}
		const std::string all_tokens = join(get_cursor_extent_tokens(cursor_to_unary_op), {});
		std::vector<std::string> vector_of_joined_tokens{};
		clang_visitChildren(cursor_to_unary_op, visitor_get_children_joined_tokens, &vector_of_joined_tokens);
		const std::string& part_without_operator = vector_of_joined_tokens[0];
		if (all_tokens.ends_with(part_without_operator))
		{
			return all_tokens.substr(0, all_tokens.size() - part_without_operator.size());
		}
		if (all_tokens.starts_with(part_without_operator))
		{
			return all_tokens.substr(part_without_operator.size());
		}
		return "";
	}

	std::string get_compound_assign_spelling(const CXCursor& cursor_to_compound_assign)
	{
		return get_spelling_of_cursor_between_two_children_parts(cursor_to_compound_assign, CXCursor_CompoundAssignOperator);
	}

	CXChildVisitResult visitor_direct_children_counter(const CXCursor cursor, const CXCursor /*parent*/, const CXClientData void_ptr_to_counter)
	{
		if (void_ptr_to_counter == nullptr)
		{
			throw std::invalid_argument(null_client_data_msg);
		}

		++*static_cast<size_t* const>(void_ptr_to_counter);
		return CXChildVisit_Continue;
	}
}
