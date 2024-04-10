#pragma once
#include "clang-c/Index.h"
#include <unordered_set>

namespace clang_code_analysis
{
	class cursor_classifier final
	{
		inline static const std::unordered_set literals_kinds{
			CXCursor_IntegerLiteral, CXCursor_FloatingLiteral, CXCursor_ImaginaryLiteral,
			CXCursor_StringLiteral, CXCursor_CharacterLiteral, CXCursor_CXXBoolLiteralExpr
		};

		inline static const std::unordered_set var_declaration_kinds{ CXCursor_VarDecl, CXCursor_FieldDecl,
			CXCursor_ParmDecl, CXCursor_NonTypeTemplateParameter };

		inline static const std::unordered_set func_declaration_kinds{
			CXCursor_CXXMethod, CXCursor_FunctionDecl, CXCursor_Constructor, CXCursor_Destructor, CXCursor_FunctionTemplate
		};

	public:
		cursor_classifier() = delete;

		[[nodiscard]] static bool is_var_declaration(const CXCursor& cursor)
		{
			return var_declaration_kinds.contains(clang_getCursorKind(cursor));
		}

		[[nodiscard]] static bool is_reference_to_var_declaration(const CXCursor& cursor)
		{
			if (const auto cursor_kind = clang_getCursorKind(cursor);
				!is_reference_expression(cursor_kind))
			{
				return false;
			}

			return is_var_declaration(clang_getCursorReferenced(cursor));
		}

		[[nodiscard]] static bool is_func_declaration(const CXCursor& cursor)
		{
			return func_declaration_kinds.contains(clang_getCursorKind(cursor));
		}

		[[nodiscard]] static bool is_func_definition(const CXCursor& cursor)
		{
			return is_func_declaration(cursor) && clang_isCursorDefinition(cursor) != 0;
		}

		[[nodiscard]] static bool is_local_var_declaration(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_VarDecl && clang_getCursorLinkage(cursor) == CXLinkage_NoLinkage;
		}

		[[nodiscard]] static bool is_param_declaration(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_ParmDecl;
		}

		[[nodiscard]] static bool is_field_declaration(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_FieldDecl;
		}

		[[nodiscard]] static bool is_reference_to_field_declaration(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_MemberRefExpr && is_field_declaration(clang_getCursorReferenced(cursor));
		}

		[[nodiscard]] static bool is_if_stmt(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_IfStmt;
		}

		[[nodiscard]] static bool is_switch_stmt(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_SwitchStmt;
		}

		[[nodiscard]] static bool is_ternary_operator_stmt(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_ConditionalOperator;
		}

		[[nodiscard]] static bool is_for_stmt(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_ForStmt;
		}

		[[nodiscard]] static bool is_for_range_stmt(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_CXXForRangeStmt;
		}

		[[nodiscard]] static bool is_while_stmt(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_WhileStmt;
		}

		[[nodiscard]] static bool is_do_stmt(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_DoStmt;
		}

		[[nodiscard]] static bool is_call_expr(const CXCursor& cursor)
		{
			return clang_getCursorKind(cursor) == CXCursor_CallExpr;
		}

		[[nodiscard]] static bool is_literal(const CXCursor& cursor)
		{
			return literals_kinds.contains(clang_getCursorKind(cursor));
		}

	private:
		[[nodiscard]] static bool is_reference_expression(const CXCursorKind& cursor_kind) noexcept
		{
			return cursor_kind == CXCursor_DeclRefExpr || cursor_kind == CXCursor_MemberRefExpr;
		}
	};
}
