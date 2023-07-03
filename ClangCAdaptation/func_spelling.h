#pragma once
#include "code_entity_spelling.h"
#include <stdexcept>

namespace clang_c_adaptation
{
	class func_spelling final : public code_entity_spelling
	{
		inline static const std::string not_func_msg{"func_spelling: cursor does not represent any kind of function declaration"};

	public:
		func_spelling() noexcept = default;
		explicit func_spelling(const CXCursor& cursor_to_func_decl) : code_entity_spelling(cursor_to_func_decl)
		{
			if (!clang_c_types_handling::is_cursor_to_func_declaration(cursor_to_func_decl))
			{
				throw std::invalid_argument(not_func_msg);
			}
		}

		[[nodiscard]] bool operator==(const func_spelling& other) const noexcept = default;
	};
}