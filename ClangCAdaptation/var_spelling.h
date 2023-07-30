#pragma once
#include "code_entity_spelling.h"
#include "common_checks.h"
#include <wrong_cursor_kind_exception.h>

namespace clang_c_adaptation
{
	class var_spelling final : public code_entity_spelling
	{
		inline static const std::string not_variable_msg{"var_spelling: cursor does not represent any kind of variables"};

	public:
		var_spelling() noexcept = default;
		explicit var_spelling(const CXCursor& cursor_to_var_decl) : code_entity_spelling(cursor_to_var_decl)
		{
			if (!common_checks::is_cursor_to_var_decl(cursor_to_var_decl))
			{
				throw wrong_cursor_kind_exception(not_variable_msg);
			}
		}

		[[nodiscard]] bool operator==(const var_spelling& other) const noexcept = default;
	};
}