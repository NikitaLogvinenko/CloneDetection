#pragma once
#include "clang-c/Index.h"
#include "nullptr_error.h"
#include <unordered_set>

namespace clang_c_adaptation
{
	class common_checks final
	{
		inline static const std::unordered_set literals_kinds{
			CXCursor_IntegerLiteral, CXCursor_FloatingLiteral, CXCursor_ImaginaryLiteral,
			CXCursor_StringLiteral, CXCursor_CharacterLiteral, CXCursor_CXXBoolLiteralExpr
		};

		inline static const std::unordered_set var_decl_kinds{ CXCursor_VarDecl, CXCursor_FieldDecl,
			CXCursor_ParmDecl, CXCursor_NonTypeTemplateParameter };

		inline static const std::unordered_set func_decl_kinds{
			CXCursor_CXXMethod, CXCursor_FunctionDecl, CXCursor_Constructor, CXCursor_Destructor, CXCursor_FunctionTemplate
		};

	public:
		common_checks() = delete;

		static void throw_if_null(const CXClientData client_data, const std::string& error_msg)
		{
			if (client_data == nullptr)
			{
				throw common_exceptions::nullptr_error(error_msg);
			}
		}

		[[nodiscard]] static bool is_var_declaration(const CXCursor& cursor) noexcept
		{
			return var_decl_kinds.contains(clang_getCursorKind(cursor));
		}

		[[nodiscard]] static bool is_reference_expression(const CXCursorKind& cursor_kind) noexcept
		{
			return cursor_kind == CXCursor_DeclRefExpr || cursor_kind == CXCursor_MemberRefExpr;
		}

		[[nodiscard]] static bool is_reference_to_var_declaration(const CXCursor& cursor) noexcept
		{
			if (const auto cursor_kind = clang_getCursorKind(cursor); !is_reference_expression(cursor_kind))
			{
				return false;
			}
			
			return is_var_declaration(clang_getCursorReferenced(cursor));
		}

		[[nodiscard]] static bool is_func_declaration(const CXCursor& cursor) noexcept
		{
			return func_decl_kinds.contains(clang_getCursorKind(cursor));
		}

		[[nodiscard]] static bool is_func_definition(const CXCursor& cursor) noexcept
		{
			return is_func_declaration(cursor) && clang_isCursorDefinition(cursor) != 0;
		}

		[[nodiscard]] static bool is_local_var_declaration(const CXCursor& cursor) noexcept
		{
			return clang_getCursorKind(cursor) == CXCursor_VarDecl && clang_getCursorLinkage(cursor) == CXLinkage_NoLinkage;
		}

		[[nodiscard]] static bool is_param_declaration(const CXCursor& cursor) noexcept
		{
			return clang_getCursorKind(cursor) == CXCursor_ParmDecl;
		}

		[[nodiscard]] static bool is_field_declaration(const CXCursor& cursor) noexcept
		{
			return clang_getCursorKind(cursor) == CXCursor_FieldDecl;
		}

		[[nodiscard]] static bool is_reference_to_field_declaration(const CXCursor& cursor) noexcept
		{
			return clang_getCursorKind(cursor) == CXCursor_MemberRefExpr && is_field_declaration(clang_getCursorReferenced(cursor));
		}

		[[nodiscard]] static bool is_literal(const CXCursor& cursor) noexcept
		{
			return literals_kinds.contains(clang_getCursorKind(cursor));
		}
	};
}
