#pragma once
#include "code_entity_spelling.h"
#include "common_checks.h"
#include "wrong_cursor_kind_exception.h"


namespace clang_c_adaptation
{
	class func_spelling final : public code_entity_spelling
	{
		inline static const std::string not_func_msg{"func_spelling: cursor does not represent any kind of function declaration"};

	public:
		func_spelling() noexcept = default;
		explicit func_spelling(const CXCursor& cursor_to_func_decl) : code_entity_spelling(cursor_to_func_decl)
		{
			if (!common_checks::is_cursor_to_func_declaration(cursor_to_func_decl))
			{
				throw wrong_cursor_kind_exception(not_func_msg);
			}
		}

		[[nodiscard]] bool operator==(const func_spelling& other) const noexcept = default;
	};
}
