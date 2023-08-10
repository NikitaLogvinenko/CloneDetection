#pragma once
#include "clang-c/Index.h"
#include "nullptr_data_exception.h"
#include <unordered_set>

namespace clang_c_adaptation
{
	class common_checks final
	{
		inline static const std::unordered_set literals_kinds{
			CXCursor_IntegerLiteral, CXCursor_FloatingLiteral, CXCursor_ImaginaryLiteral,
				CXCursor_StringLiteral, CXCursor_CharacterLiteral, CXCursor_CXXBoolLiteralExpr};

		inline static const std::unordered_set var_decl_kinds{ CXCursor_VarDecl, CXCursor_FieldDecl, CXCursor_ParmDecl };

		inline static const std::unordered_set func_decl_kinds{
			CXCursor_CXXMethod, CXCursor_FunctionDecl, CXCursor_Constructor, CXCursor_Destructor, CXCursor_FunctionTemplate };

	public:
		common_checks() = delete;

		static void throw_if_null(const CXClientData client_data)
		{
			if (client_data == nullptr)
			{
				throw nullptr_data_exception("Null client data passed to visitor.");
			}
		}

		[[nodiscard]] static bool is_cursor_to_var_decl(const CXCursor& cursor) noexcept
		{
			return var_decl_kinds.contains(clang_getCursorKind(cursor));
		}

		[[nodiscard]] static bool is_cursor_referring_to_var_decl(const CXCursor& cursor) noexcept
		{
			if (clang_getCursorKind(cursor) != CXCursor_DeclRefExpr)
			{
				return false;
			}
			return is_cursor_to_var_decl(clang_getCursorReferenced(cursor));
		}

		[[nodiscard]] static bool is_cursor_to_func_declaration(const CXCursor& cursor) noexcept
		{
			return func_decl_kinds.contains(clang_getCursorKind(cursor));
		}

		[[nodiscard]] static bool is_cursor_to_func_definition(const CXCursor& cursor) noexcept
		{
			return is_cursor_to_func_declaration(cursor) && clang_isCursorDefinition(cursor) != 0;
		}

		[[nodiscard]] static bool is_cursor_to_literal(const CXCursor& cursor) noexcept
		{
			return literals_kinds.contains(clang_getCursorKind(cursor));
		}
	};
}
