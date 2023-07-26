#pragma once
#include "var_linkage.h"
#include "clang-c/Index.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_set>

namespace clang_c_adaptation
{
	class clang_c_types_handling final
	{
		inline static const std::string null_client_data_msg{"Null client data passed to visitor."};
		inline static const std::string invalid_cursor_msg{"Invalid CXCursor is encountered."};
		inline static const std::string not_var_decl_msg{"Cursor does not point to any type of variables declarations."};
		inline static const std::string cxstring_conversion_error_msg{"Failure during CXString to std::string conversion. CXString was just disposed. Exception: "};
		inline static const std::string cursor_tokenization_error_msg{"Failure during CXCursor tokenization. Memory leak was prevented. Exception: "};

		inline static const std::string empty_string{};
		inline static const std::string tokens_sep{};

		static constexpr size_t first_str_symbol = 0;

		inline static const std::unordered_set literals_kinds{
			CXCursor_IntegerLiteral, CXCursor_FloatingLiteral, CXCursor_ImaginaryLiteral,
				CXCursor_StringLiteral, CXCursor_CharacterLiteral, CXCursor_CXXBoolLiteralExpr};

		inline static const std::unordered_set var_decl_kinds{ CXCursor_VarDecl, CXCursor_FieldDecl, CXCursor_ParmDecl };

		inline static const std::unordered_set func_decl_kinds{
			CXCursor_CXXMethod, CXCursor_FunctionDecl, CXCursor_Constructor, CXCursor_Destructor, CXCursor_FunctionTemplate };

		static constexpr size_t reserved_space_for_two_children = 2;

	public:
		clang_c_types_handling() = delete;

		[[nodiscard]] static bool is_cursor_to_var_decl(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_referring_to_var_decl(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_to_func_declaration(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_to_func_definition(const CXCursor& cursor) noexcept;

		[[nodiscard]] static bool is_cursor_to_literal(const CXCursor& cursor) noexcept;

		[[nodiscard]] static var_linkage determine_var_linkage(const CXCursor& cursor_to_var_decl);


		[[nodiscard]] static std::string get_binary_operator_spelling(const CXCursor& cursor_to_binary_op);

		[[nodiscard]] static std::string get_unary_operator_spelling(const CXCursor& cursor_to_unary_op);

		[[nodiscard]] static std::string get_compound_assign_spelling(const CXCursor& cursor_to_compound_assign);

		[[nodiscard]] static CXChildVisitResult visitor_direct_children_counter(CXCursor cursor, CXCursor parent, CXClientData void_ptr_to_size_t_counter);

	private:
		static void client_data_not_null_validation(CXClientData client_data);

		[[nodiscard]] static std::vector<std::string> get_cursor_extent_tokens(const CXCursor& cursor);

		[[nodiscard]] static std::string join(const std::vector<std::string>& strings, const std::string& sep = " ");

		static CXChildVisitResult visitor_get_children_joined_tokens(CXCursor cursor, CXCursor, CXClientData vector_of_joined_tokens_void_ptr);

		static std::string get_spelling_of_cursor_between_two_children_parts(const CXCursor& cursor, CXCursorKind expected_kind);
	};
}