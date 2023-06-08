#pragma once
#include "var_linkage.h"
#include "clang-c/Index.h"
#include <string>
#include <memory>
#include <vector>

namespace clang_c_adaptation
{
	void cursor_not_null_and_valid(const CXCursor& cursor);

	void client_data_not_null_validation(const CXClientData& client_data);

	[[nodiscard]] std::string cxstring_to_string(std::unique_ptr<CXString> cxstring_ptr);

	[[nodiscard]] bool is_cursor_to_var_decl(const CXCursor& cursor) noexcept;

	[[nodiscard]] bool is_decl_ref_expr_to_var_decl(const CXCursor& cursor) noexcept;

	[[nodiscard]] bool is_cursor_to_func_declaration(const CXCursor& cursor) noexcept;

	[[nodiscard]] bool is_cursor_to_func_definition(const CXCursor& cursor) noexcept;

	[[nodiscard]] bool is_cursor_to_literal(const CXCursor& cursor) noexcept;

	[[nodiscard]] var_linkage determine_var_linkage(const CXCursor& cursor_to_var_decl);

	[[nodiscard]] std::vector<std::string> get_cursor_extent_tokens(const CXCursor& cursor);

	[[nodiscard]] std::string join(const std::vector<std::string>& strings, const std::string& sep = " ");

	[[nodiscard]] std::string get_binary_operator_spelling(const CXCursor& cursor_to_binary_op);

	[[nodiscard]] std::string get_unary_operator_spelling(const CXCursor& cursor_to_unary_op);

	[[nodiscard]] std::string get_compound_assign_spelling(const CXCursor& cursor_to_compound_assign);

	[[nodiscard]] CXChildVisitResult visitor_direct_children_counter(CXCursor cursor, CXCursor parent, CXClientData void_ptr_to_counter);
}