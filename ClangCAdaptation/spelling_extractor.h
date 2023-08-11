#pragma once
#include "common_checks.h"

namespace clang_c_adaptation
{
	class spelling_extractor final
	{
	public:
		spelling_extractor() = delete;

		[[nodiscard]] static std::string get_binary_operator_spelling(const CXCursor& cursor_to_binary_op);

		[[nodiscard]] static std::string get_unary_operator_spelling(const CXCursor& cursor_to_unary_op);

		[[nodiscard]] static std::string get_compound_assign_spelling(const CXCursor& cursor_to_compound_assign);

	private:
		[[nodiscard]] static std::vector<std::string> get_cursor_extent_tokens(const CXCursor& cursor);

		[[nodiscard]] static std::string join(const std::vector<std::string>& strings, const std::string& sep = " ");

		static CXChildVisitResult visitor_append_subtrees_spellings(CXCursor cursor, CXCursor, CXClientData ptr_to_vector);

		static std::string get_spelling_of_cursor_between_two_children_parts(const CXCursor& cursor, CXCursorKind expected_kind);

		static std::vector<std::string> get_entire_spelling_and_subtrees_spellings(
			const CXCursor& cursor, CXCursorKind expected_cursor_kind, size_t children_count);
	};
}
