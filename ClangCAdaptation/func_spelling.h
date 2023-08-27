#pragma once
#include "code_entity_spelling.h"
#include "common_checks.h"
#include "wrong_cursor_kind_exception.h"


namespace clang_c_adaptation
{
	class func_spelling final
	{
		code_entity_spelling spelling_{};

	public:
		func_spelling() noexcept = default;

		explicit func_spelling(const CXCursor& cursor_to_func_decl) : spelling_(cursor_to_func_decl)
		{
			if (!common_checks::is_cursor_to_func_declaration(cursor_to_func_decl))
			{
				throw wrong_cursor_kind_exception(
					"func_spelling: cursor does not represent any kind of function declaration");
			}
		}

		[[nodiscard]] const code_entity_spelling& to_code_entity_spelling() const noexcept
		{
			return spelling_;
		}

		[[nodiscard]] bool operator==(const func_spelling& other) const noexcept = default;
	};
}
