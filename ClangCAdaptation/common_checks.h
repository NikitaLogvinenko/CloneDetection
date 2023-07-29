#pragma once
#include "clang-c/Index.h"
#include <string>
#include <unordered_set>

namespace clang_c_adaptation
{
	class common_checks final
	{
		inline static const std::string null_client_data_msg{"Null client data passed to visitor."};

		inline static const std::unordered_set literals_kinds{
			CXCursor_IntegerLiteral, CXCursor_FloatingLiteral, CXCursor_ImaginaryLiteral,
				CXCursor_StringLiteral, CXCursor_CharacterLiteral, CXCursor_CXXBoolLiteralExpr};

		inline static const std::unordered_set var_decl_kinds{ CXCursor_VarDecl, CXCursor_FieldDecl, CXCursor_ParmDecl };

		inline static const std::unordered_set func_decl_kinds{
			CXCursor_CXXMethod, CXCursor_FunctionDecl, CXCursor_Constructor, CXCursor_Destructor, CXCursor_FunctionTemplate };

	public:
		common_checks() = delete;

		static void client_data_not_null_validation(CXClientData client_data);

		[[nodiscard]] static bool is_cursor_to_var_decl(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_referring_to_var_decl(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_to_func_declaration(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_to_func_definition(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_to_literal(const CXCursor& cursor) noexcept;
	};
}
