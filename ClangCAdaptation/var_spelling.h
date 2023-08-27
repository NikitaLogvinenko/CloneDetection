#pragma once
#include "code_entity_spelling.h"
#include "common_checks.h"
#include <wrong_cursor_kind_exception.h>

namespace clang_c_adaptation
{
	class var_spelling final
	{
		code_entity_spelling spelling_{};

	public:
		var_spelling() noexcept = default;

		explicit var_spelling(const CXCursor& cursor_to_var_decl) : spelling_(cursor_to_var_decl)
		{
			if (!common_checks::is_cursor_to_var_decl(cursor_to_var_decl))
			{
				throw wrong_cursor_kind_exception(
					"var_spelling: cursor does not represent any kind of variables");
			}
		}

		[[nodiscard]] const code_entity_spelling& to_code_entity_spelling() const noexcept
		{
			return spelling_;
		}

		[[nodiscard]] bool operator==(const var_spelling& other) const noexcept = default;
	};
}
