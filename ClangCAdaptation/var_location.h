#pragma once
#include "code_entity_location.h"
#include <stdexcept>

namespace clang_c_adaptation
{
	class var_location final : public code_entity_location
	{
		inline static const std::string not_var_msg{"var_location: cursor does not represent any kind of variables"};

	public:
		var_location() noexcept = default;
		explicit var_location(const CXCursor& cursor_to_var_decl) : code_entity_location(cursor_to_var_decl)
		{
			if (!is_cursor_to_var_decl(cursor_to_var_decl))
			{
				throw std::invalid_argument(not_var_msg);
			}
		}

		[[nodiscard]] bool operator==(const var_location& other) const noexcept = default;
	};
}