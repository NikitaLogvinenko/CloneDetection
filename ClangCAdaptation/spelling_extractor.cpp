#include "spelling_extractor.h"
#include "cxstring_wrapper.h"
#include "wrong_cursor_kind_exception.h"
#include "unsuccessfull_tokenization_exception.h"
#include <numeric>

namespace clang_c_adaptation
{
	std::vector<std::string> spelling_extractor::get_cursor_extent_tokens(const CXCursor& cursor)
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
			throw unsuccessfull_tokenization_exception(ex);
		}
		clang_disposeTokens(translation_unit, tokens, tokens_n);
		return tokens_vector;
	}

	std::string spelling_extractor::join(const std::vector<std::string>& strings, const std::string& sep)
	{
		if (strings.empty())
		{
			return {};
		}
		const std::string result = std::accumulate(
			strings.cbegin(), strings.cend(), empty_string,
			[&sep](std::string&& prev_part, const std::string& new_part)
			{ return std::move(prev_part) + new_part + sep; });
		return result.substr(zero_index, result.size() - sep.size());
	}

	std::string spelling_extractor::get_binary_operator_spelling(const CXCursor& cursor_to_binary_op)
	{
		return get_spelling_of_cursor_between_two_children_parts(cursor_to_binary_op, CXCursor_BinaryOperator);
	}

	std::string spelling_extractor::get_unary_operator_spelling(const CXCursor& cursor_to_unary_op)
	{
		const auto entire_and_subtrees_spellings = get_entire_spelling_and_subtrees_spellings(
			cursor_to_unary_op, CXCursor_UnaryOperator, one_child);
		const std::string& entire_spelling = entire_and_subtrees_spellings[0];
		const std::string& part_without_operator = entire_and_subtrees_spellings[1];
		if (entire_spelling.ends_with(part_without_operator))
		{
			return entire_spelling.substr(zero_index, entire_spelling.size() - part_without_operator.size());
		}
		if (entire_spelling.starts_with(part_without_operator))
		{
			return entire_spelling.substr(part_without_operator.size());
		}
		throw wrong_cursor_kind_exception(unexpected_cursor_to_unary_op_msg);
	}

	std::string spelling_extractor::get_compound_assign_spelling(const CXCursor& cursor_to_compound_assign)
	{
		return get_spelling_of_cursor_between_two_children_parts(cursor_to_compound_assign, CXCursor_CompoundAssignOperator);
	}

	CXChildVisitResult spelling_extractor::visitor_append_subtrees_spellings(
		const CXCursor cursor, CXCursor, const CXClientData ptr_to_vector)
	{
		common_checks::client_data_not_null_validation(ptr_to_vector);

		const auto vector_append_to = static_cast<std::vector<std::string>*const>(ptr_to_vector);
		vector_append_to->push_back(join(get_cursor_extent_tokens(cursor), {}));
		return CXChildVisit_Continue;
	}

	std::string spelling_extractor::get_spelling_of_cursor_between_two_children_parts(const CXCursor& cursor,
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
				throw wrong_cursor_kind_exception(cursor_is_not_between_two_children_msg);
			}
			return entire_spelling.substr(left_subtree.size(),
				entire_spelling.size() - left_subtree.size() - right_subtree.size());
		}
	}

	std::vector<std::string> spelling_extractor::get_entire_spelling_and_subtrees_spellings(const CXCursor& cursor,
		const CXCursorKind expected_cursor_kind, const size_t children_count)
	{
		if (clang_getCursorKind(cursor) != expected_cursor_kind)
		{
			throw wrong_cursor_kind_exception(wrong_cursor_type_msg);
		}

		std::vector<std::string> entire_and_subtrees_spellings{};
		const size_t total_spellings = 1 + children_count;
		entire_and_subtrees_spellings.reserve(total_spellings);
		entire_and_subtrees_spellings.emplace_back(
			join(get_cursor_extent_tokens(cursor), tokens_sep));
		clang_visitChildren(cursor, visitor_append_subtrees_spellings, &entire_and_subtrees_spellings);

		if (entire_and_subtrees_spellings.size() != total_spellings)
		{
			throw wrong_cursor_kind_exception(wrong_children_count_msg);
		}

		return entire_and_subtrees_spellings;
	}
}
