#include "common_checks.h"
#include "nullptr_data_exception.h"

namespace clang_c_adaptation
{
	void common_checks::client_data_not_null_validation(const CXClientData client_data)
	{
		if (client_data == nullptr)
		{
			throw nullptr_data_exception(null_client_data_msg);
		}
	}

	bool common_checks::is_cursor_to_var_decl(const CXCursor& cursor) noexcept
	{
		return var_decl_kinds.contains(clang_getCursorKind(cursor));
	}

	bool common_checks::is_cursor_referring_to_var_decl(const CXCursor& cursor) noexcept
	{
		if (clang_getCursorKind(cursor) != CXCursor_DeclRefExpr)
		{
			return false;
		}
		return is_cursor_to_var_decl(clang_getCursorReferenced(cursor));
	}

	bool common_checks::is_cursor_to_func_declaration(const CXCursor& cursor) noexcept
	{
		return func_decl_kinds.contains(clang_getCursorKind(cursor));
	}

	bool common_checks::is_cursor_to_func_definition(const CXCursor& cursor) noexcept
	{
		return is_cursor_to_func_declaration(cursor) && clang_isCursorDefinition(cursor) != 0;
	}

	bool common_checks::is_cursor_to_literal(const CXCursor& cursor) noexcept
	{
		return literals_kinds.contains(clang_getCursorKind(cursor));
	}
}
