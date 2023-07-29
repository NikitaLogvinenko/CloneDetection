#pragma once
#include "common_checks.h"

namespace clang_c_adaptation
{
	class spelling_extractor final
	{
		inline static const std::string cursor_tokenization_error_msg{"Failure during CXCursor tokenization. Memory leak was prevented. Exception: "};
		inline static const std::string wrong_cursor_type_msg{"Actual cursor kind differ from expected."};
		inline static const std::string wrong_children_count_msg{"Wrong children count. Can not get spelling."};
		inline static const std::string unexpected_cursor_to_unary_op_msg{"Unexpected cursor was passed to get_unary_operator_spelling method"};
		inline static const std::string cursor_is_not_between_two_children_msg{"Cursor is not between two children msg. Unable to get its spelling."};

		inline static const std::string empty_string{};
		inline static const std::string tokens_sep{};

		static constexpr size_t zero_index = 0;

		static constexpr size_t two_children = 2;
		static constexpr size_t one_child = 1;

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
