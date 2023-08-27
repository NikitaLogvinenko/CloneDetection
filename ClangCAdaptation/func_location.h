#pragma once
#include "code_entity_location.h"
#include "common_checks.h"
#include "wrong_cursor_kind_exception.h"

namespace clang_c_adaptation
{
	class func_location final
	{
		code_entity_location location_{};

	public:
		func_location() noexcept = default;

		explicit func_location(const CXCursor& cursor_to_func_decl) : location_(cursor_to_func_decl)
		{
			if (!common_checks::is_cursor_to_func_declaration(cursor_to_func_decl))
			{
				throw wrong_cursor_kind_exception(
					"func_location: cursor does not represent any kind of function declaration");
			}
		}

		[[nodiscard]] const code_entity_location& to_code_entity_location() const noexcept
		{
			return location_;
		}

		[[nodiscard]] bool operator==(const func_location& other) const noexcept = default;
	};
}
